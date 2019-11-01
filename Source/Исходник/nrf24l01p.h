
#ifndef NRF24L01P_H
#define	NRF24L01P_H

/* ���� ��� ���������� �� ������ ����������� � ���� spi.h!!!!!!*/

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "spi.h"
#include "main.h"

//

#define CONFIG_SET    ((1 << EN_CRC) | (0 << CRCO))
#define NRF24_ADDR_LEN 5
//#define PAY_LEN 2

#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST   1


// ������� �������
#define R_REGISTER          0b00000000  //������� ������� � �������. ���� 0-4 - ������ �������.
#define W_REGISTER          0b00100000  //������� ������ � ������. ���� 0-4 - ������ �������.
#define R_RX_PAYLOAD        0b01100001  //������ � RX-Payload: 1- 32 ���. 
#define W_TX_PAYLOAD        0b10100000  //������ � TX-Payload: 1- 32 ���.
#define FLUSH_TX            0b11100001  //*������� TX FIFO, ��������������� � ����� TX.
#define FLUSH_RX            0b11100010  //*������� RX FIFO, ��������������� � ����� RX.
#define REUSE_TX_PL         0b11100011  //�������� ������������ �������� ��������.
#define R_RX_PL_WID         0b01100000  //��������� ������ ������.
#define W_ACK_PAYLOAD       0b10101000  //
#define W_TX_PAYLOAD_NOACK  0b10110000  //
#define NRF_NOP                 0b11111111  //

// ������� ������

// ������ ������������
#define NRF24_CONFIG 0x00       // ������ �������    
#define MASK_RX_DR  6           // ��������\�������� ����������� �� RX_DR. 0 - on; 1 - 1 off.
#define MASK_TX_DS  5           // ��������\�������� ����������� �� TX_DS. 0 - on; 1 - 1 off.
#define MASK_MAX_RT 4           // ��������\�������� ����������� �� MAX_RT. 0 - on; 1 - 1 off.
#define EN_CRC      3           // CRC. �����, ���� �������� ��� ���� � ��� EN_AA
#define CRCO        2           // ����� CRC. 0 - 1 ����; 1 - 2 �����.
#define PWR_UP      1           // 1 - POWER_UP; 0 - POWER_DOWN;
#define PRIM_RX     0           // 1 - ������; 0 - ��������.

#define NRF24_EN_AA       0x01  // ������ ����. ����������������
/* enable auto acknowledgment */
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0

#define NRF24_EN_RXADDR   0x02  // ����. ������ RX
/* enable rx addresses */
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0

#define NRF24_SETUP_AW    0x03  // ������ ������. 01 - 3 �����; 10 - 4 �����; 11 - 5 ����.
#define NRF24_SETUP_RETR  0x04  // ������������ ����������� �������� ��������
/* setup of auto re-transmission */
#define ARD         4 /* 4 bits */
#define ARC         0 /* 4 bits */

#define NRF24_RF_CH       0x05  // ���������� ��������� ����� 
#define NRF24_RF_SETUP    0x06  // ������ ����������� RF
/* RF setup register */
#define RF_DR_LOW   5
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      1 /* 2 bits */ 



#define NRF24_STATUS      0x07  // ������ ����� ������. ��� � ����� ������� ������ ��������� �� SPI ���� ������� ���������� �����.
#define RX_DR             6     // ³������� ����������� �� �������. ��� �������� �������� 1.
#define TX_DS             5     // ��������� �������� �� ����������� ����� TX FIFO. ��������������, ���� �����, ���������. ���� AUTO_ACK ����������, ��� �� �������������� ���� ���, ���� � ACK ��������.
#define MAX_RT            4     // ����������� ����������� ������ ������� ��������. �������� 1, ��� �������� ��. ���� �������� MAX_RT, �� ���� �������� ��� ���������� ����������.
#define RX_P_NO_2         3
#define RX_P_NO_1         2
#define RX_P_NO_0         1
#define TX_FULL           0     // ���� ������������ TX FIFO

#define NRF24_OBSERVE_TX  0x08
#define NRF24_RPD         0x09
#define NRF24_RX_ADDR_P0  0x0A  // ������ ���������� 0. ������� 5 ���� ��������. ����������� � ���. SETUP_AW.___
// �� ������������ 0xE7E7E7E7E7
#define NRF24_RX_ADDR_P1  0x0B  // ������ ���������� 1. ������� 5 ���� ��������. ����������� � ���. SETUP_AW.___
// �� ������������ 0xC2C2C2C2C2
#define NRF24_RX_ADDR_P2  0x0C  // ������ ���������� 2. ������� 5 ���� ��������. ����������� � ���. SETUP_AW.___
// ҳ���� �������� ����. �� ���� ��� �� � P1. �� ������������ 0xC3
#define NRF24_RX_ADDR_P3  0x0D  // ������ ���������� 2. ������� 5 ���� ��������. ����������� � ���. SETUP_AW.___
// ҳ���� �������� ����. �� ���� ��� �� � P1. �� ������������ 0xC4
#define NRF24_RX_ADDR_P4  0x0E  // ������ ���������� 2. ������� 5 ���� ��������. ����������� � ���. SETUP_AW.___
// ҳ���� �������� ����. �� ���� ��� �� � P1. �� ������������ 0xC5
#define NRF24_RX_ADDR_P5  0x0F  // ������ ���������� 2. ������� 5 ���� ��������. ����������� � ���. SETUP_AW.___
// ҳ���� �������� ����. �� ���� ��� �� � P1. �� ������������ 0xC6

#define NRF24_TX_ADDR     0x10  // ������ �����������. �� ������������ 0xE7E7E7E7E7

#define NRF24_RX_PW_P0  0x11    // ʳ������ ���� ���� �����. ³� 1 �� 32.
#define NRF24_RX_PW_P1  0x12    // ʳ������ ���� ���� �����. ³� 1 �� 32.
#define NRF24_RX_PW_P2  0x13    // ʳ������ ���� ���� �����. ³� 1 �� 32.
#define NRF24_RX_PW_P3  0x14    // ʳ������ ���� ���� �����. ³� 1 �� 32.
#define NRF24_RX_PW_P4  0x15    // ʳ������ ���� ���� �����. ³� 1 �� 32.
#define NRF24_RX_PW_P5  0x16    // ʳ������ ���� ���� �����. ³� 1 �� 32.

#define NRF24_FIFO_STATUS  0x17 // ������ ����� FIFO
#define TX_REUSE            6
#define TX_FULL_FIFO        5
#define TX_EMPTY            4
#define RX_FULL             1
#define RX_EMPTY            0

#define NRF24_DYNPD  0x1C    // �������� ������� �����. ������� ���������� EN_DPL �� ENAA_Px
/* dynamic length */
#define DPL_P0      0
#define DPL_P1      1
#define DPL_P2      2
#define DPL_P3      3
#define DPL_P4      4
#define DPL_P5      5
#define NRF24_FEATURE  0x1D     // 59 ������� ��������


//#define NRF24_TX_ADDRESS 0x1979270904
//#define NRF24_RX_ADDRESS NRF24_TX_ADDRESS



void nrf24_init(uint8_t channel, uint8_t pay_length);
void nrf24_write_reg(uint8_t reg, uint8_t value);
uint8_t nrf24_read_reg(uint8_t reg);
void nrf24_powerUpRx();
//void nrf24_powerUpTx();
//void nrf24_powerDown();
void nrf24_write_buf(uint8_t reg, const uint8_t *pBuf, uint8_t length);
void nrf24_read_buf(uint8_t reg, uint8_t *pBuf, uint8_t length);
void nrf24_tx_address(const uint8_t *adr);
void nrf24_rx_address(const uint8_t *adr);
//void nrf24_send(uint8_t *value);
//uint8_t nrf24_isSending();
uint8_t nrf24_getStatus();
//uint8_t nrf24_messageStatus();
//uint8_t nrf24_retransmissionCount();
void nrf24_getData(uint8_t* data);
//uint8_t nrf24_payloadLength();
uint8_t nrf24_rxFifoEmpty();
uint8_t nrf24_dataReady();


#endif	/* XC_HEADER_TEMPLATE_H */

