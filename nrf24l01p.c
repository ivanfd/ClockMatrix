#include "nrf24l01p.h"

uint8_t pay_len;  // ������� �������
 const uint8_t nrf_tx_address[5] = {0x19, 0x79, 0x27, 0x09, 0x04};
 const uint8_t nrf_rx_address[5] = {0x19, 0x79, 0x27, 0x09, 0x04};

/**********************************************
 *      ����������� ������
 *  channel -  ����� ������
 *  pay_length -  ������� ������  
 *********************************************/

void nrf24_init(uint8_t channel, uint8_t pay_length) {

    SPI_CSN = 1;
    SPI_CE = 0;
    pay_len = pay_length;
    
    nrf24_tx_address(&nrf_tx_address);
    nrf24_rx_address(&nrf_rx_address);
    
    nrf24_write_reg(NRF24_RF_CH, channel); // ������ ����� ������

    nrf24_write_reg(NRF24_RX_PW_P0, 0x00);
    nrf24_write_reg(NRF24_RX_PW_P1, pay_length);
    nrf24_write_reg(NRF24_RX_PW_P2, 0x00);
    nrf24_write_reg(NRF24_RX_PW_P3, 0x00);
    nrf24_write_reg(NRF24_RX_PW_P4, 0x00);
    nrf24_write_reg(NRF24_RX_PW_P5, 0x00);

    // 250 Kbps, ���������: 0dbm
    nrf24_write_reg(NRF24_RF_SETUP, (1 << RF_DR_LOW) | (0 << RF_DR) | ((0x03) << RF_PWR));

    // CRC ����������, ������� CRC 1 ����
    nrf24_write_reg(NRF24_CONFIG, CONFIG_SET);

    // Auto Acknowledgment
    nrf24_write_reg(NRF24_EN_AA, (1 << ENAA_P0) | (1 << ENAA_P1) | (0 << ENAA_P2) | (0 << ENAA_P3) | (0 << ENAA_P4) | (0 << ENAA_P5));

    // Enable RX addresses
    nrf24_write_reg(NRF24_EN_RXADDR, (1 << ERX_P0) | (1 << ERX_P1) | (0 << ERX_P2) | (0 << ERX_P3) | (0 << ERX_P4) | (0 << ERX_P5));

    // �������� �����������: 1000 ��� 15 �����
    nrf24_write_reg(NRF24_SETUP_RETR, (0x04 << ARD) | (0x0F << ARC));

// Dynamic length configurations: No dynamic length
    nrf24_write_reg(NRF24_DYNPD,(0<<DPL_P0)|(0<<DPL_P1)|(0<<DPL_P2)|(0<<DPL_P3)|(0<<DPL_P4)|(0<<DPL_P5));
    
        // Start listening
    nrf24_powerUpRx();
    
}


/*******************************
    ����� ����������
******************************/

void nrf24_powerUpRx()
{     
    SPI_CSN = 0;
    spi_rw(FLUSH_RX);   // �������� ��������� �����
    SPI_CSN = 1;

    nrf24_write_reg(NRF24_STATUS,(1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT)); //������� �������� �����������
    
    SPI_CE = 0;    
    nrf24_write_reg(NRF24_CONFIG,CONFIG_SET|((1<<PWR_UP)|(1<<PRIM_RX)));    
    SPI_CE = 1;
    __delay_us(135);
}


/*******************************
    ����� ��������
******************************/
void nrf24_powerUpTx()
{
    //nrf24_write_reg(FLUSH_TX, 0); //flush Tx

    nrf24_write_reg(NRF24_STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));

    nrf24_write_reg(NRF24_CONFIG,CONFIG_SET|((1<<PWR_UP)|(0<<PRIM_RX)));
        __delay_us(135);
}


void nrf24_powerDown()
{
    SPI_CE = 0;
    nrf24_write_reg(NRF24_CONFIG,CONFIG_SET);
}


/***************************************
 ������ ��� "value" � ����� "reg"
 ***************************************/
void nrf24_write_reg(uint8_t reg, uint8_t value) {
    SPI_CSN = 0; // CSN low, init SPI transaction
    spi_rw(W_REGISTER | reg); // select register
    spi_rw(value); // ..and write value to it..
    SPI_CSN = 1; // CSN high again
}     

/***************************************
 ������ ��� "value" � ������ "reg"
 ***************************************/
uint8_t nrf24_read_reg(uint8_t reg) {
    uint8_t value;
    SPI_CSN = 0; // CSN low, initialize SPI communication...
    spi_rw(reg); // Select register to read from..
    value = spi_rw(0); // ..then read register value
    SPI_CSN = 1; // CSN high, terminate SPI communication

    return (value); // return register value
}
/***************************************
 ������ length ����� � ������
 ***************************************/
void nrf24_write_buf(uint8_t reg, uint8_t *pBuf, uint8_t length) {
    uint8_t i;

    SPI_CSN = 0; // Set CSN low, init SPI tranaction
    spi_rw(reg); // Select register to write to and read status UINT8
    for (i = 0; i < length; i++) // then write all UINT8 in buffer(*pBuf) 
        spi_rw(*pBuf++);
    SPI_CSN = 1; // Set CSN high again      

}

/**************************************************         
	���� 'length' � �������� 'reg'         
**************************************************/
void nrf24_read_buf(uint8_t reg, uint8_t *pBuf, uint8_t length) {
    uint8_t status, i;

    SPI_CSN = 0; // Set CSN l
    status = spi_rw(reg); // Select register to write, and read status UINT8

    for (i = 0; i < length; i++)
        pBuf[i] = spi_rw(0); // Perform SPI_RW to read UINT8 from RFM70 

    SPI_CSN = 1; // Set CSN high again

}   


/* ������������ ������ ���������� */
void nrf24_tx_address(uint8_t *adr) {
    //RX_ADDR_PO - ������� ���� ����� �����, �� ������ �����������
    nrf24_write_buf(W_REGISTER | NRF24_RX_ADDR_P0, adr, NRF24_ADDR_LEN);
    nrf24_write_buf(W_REGISTER | NRF24_TX_ADDR, adr, NRF24_ADDR_LEN);
}

/* ������������ ������ �������� */
void nrf24_rx_address(uint8_t *adr) {

    nrf24_write_buf(W_REGISTER | NRF24_RX_ADDR_P1, adr, NRF24_ADDR_LEN);
}

/************************************
           �������� ����� �� �����
 ***********************************/
void nrf24_send(uint8_t *value) {

    SPI_CE = 0;
    nrf24_powerUpTx();
    SPI_CSN = 0; // ������� ����� �������
    spi_rw(FLUSH_TX); //...........
    SPI_CSN = 1; //

    nrf24_write_buf(W_TX_PAYLOAD, value, pay_len);
    SPI_CE = 1;
}

/******************************
 *    ��� ������� �������.
 *  ������� 0, ���� �������� ���
 * ����������� ��� ������������.
 *****************************/
uint8_t nrf24_isSending() {
    uint8_t status;

    // ������ �������� ����
    status = nrf24_getStatus();

    if ((status & ((1 << TX_DS) | (1 << MAX_RT)))) {
        return 0; /* false */
    }

    return 1; /* true */

}



/********************************
 *  �������� ������ STATUS
********************************/
uint8_t nrf24_getStatus() {
    uint8_t r_status;
    SPI_CSN = 0;
    r_status = spi_rw(NRF_NOP);
    SPI_CSN = 1;
    return r_status;
}


/***************************************
 *   ��������� ���� ���������� ������
 **************************************/

uint8_t nrf24_messageStatus() {
    uint8_t r_status;

    r_status = nrf24_getStatus();

    // ����� �������� ������
    if ((r_status & ((1 << TX_DS)))) {
        return NRF24_TRANSMISSON_OK;
    }        
        // ���������� ��� ������������???
    else if ((r_status & ((1 << MAX_RT)))) {
        return NRF24_MESSAGE_LOST;
    }
        // ������� �� ��� ��������
    else {
        return 0xFF;
    }
}

/* ***************************************
 *    ������� ������� ������������
 *      ������
 ****************************************/
uint8_t nrf24_retransmissionCount()
{
    uint8_t r_count;
    r_count = nrf24_read_reg(NRF24_OBSERVE_TX);
    r_count = r_count & 0x0F;
    return r_count;
}



/********************************************
 *    ������ ��� � �����
********************************************/
void nrf24_getData(uint8_t* data) 
{
                                 
    nrf24_read_buf(R_RX_PAYLOAD, data, pay_len);

    // ������� ������� �����������
    nrf24_write_reg(NRF24_STATUS,(1<<RX_DR));   
}



/* ������� ������� ������ */
uint8_t nrf24_payloadLength()
{
    uint8_t status;
    SPI_CSN = 0;
    spi_rw(R_RX_PL_WID);
    status = spi_rw(0x00);
    SPI_CSN = 1;
    return status;
}


/* ����������, �� ������� ��������� ����� */
uint8_t nrf24_rxFifoEmpty()
{
    uint8_t fifoStatus;

    fifoStatus = nrf24_read_reg(NRF24_FIFO_STATUS);
    
    return (fifoStatus & (1 << RX_EMPTY));
}


/* ���������� �� ������� ��� ��� ������� �� ������ */
/* ������� 1, ���� ������� */
uint8_t nrf24_dataReady() 
{
    uint8_t status = nrf24_getStatus();


    if ( status & (1 << RX_DR) ) 
    {
        return 1;
    }

    return !nrf24_rxFifoEmpty();;
}