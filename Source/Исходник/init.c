


#include "init.h"

//extern const unsigned char userID[ 8 ] @ 0x200000;
//extern uint8_t type_font;
//extern uint8_t type_clk;
//extern int8_t brg_type;
//extern uint8_t brig;// �������� ���������
//extern uint8_t blk_dot; // ����� �� ������� ������
//extern uint8_t en_h_snd;
//extern int8_t en_ds_1;    //  �� ���. ����������� � ������� 1
//extern int8_t en_ds_2;    //  �� ���. ����������� � ������� 2
//extern int8_t en_bmp280; //  �� �������� ����
//extern int8_t en_dst; // ������� �� ���� ���
//extern uint8_t type_temp;
//extern int8_t en_am2302; //������ ��������
//extern const unsigned char userID[8] @ 0x200000;
//volatile unsigned char usrID[8];


void SYSTEM_Initialize(void)  // ����������� ����������
{
  //  uint8_t i;
    
   // for (i = 0; i <= 7; i++)
   //     usrID[i] = userID[i];


    Port_Init();
    I2C_Init();
    Init7221();
    TMRInit();
    Interrupt_Init();
    DS3231_init();
    init_ds18b20();
    RTOS_Init();
    init_uart();
    //bmp085Calibration();
    bmp280_Init();
    type_font = read_eep(EE_FONT);
    type_clk = read_eep(EE_TYPE_CLK);
    if(type_clk == 1)
        blk_dot = 0;
    else
        blk_dot = 1;
    brg_type = read_eep(EE_TYPE_BRG);
    brig = read_eep(EE_DAT_BRG);
    Cmd7221(INTENSITY_R, brig); //Intensity Register
    set_font();
    sound_init();
    spi_init();
    nrf24_init(120, 4);
    RTOS_SetTask(usart_r, 40, cycle_main); // ������ ���������� ��� �����
    RTOS_SetTask(GetTime, 100, cycle_main);// ������ ���������� ����� � RTC
    en_h_snd = read_eep(EE_EN_SND_H);
    en_ds_1 = read_eep(EE_EN_DS1);    
    en_ds_2 = read_eep(EE_EN_DS2);
    en_bmp280 = read_eep(EE_EN_BMP);
    en_dst = read_eep(EE_EN_DST); // ������� �� ���� ���
    type_temp = read_eep(EE_TYPE_TEMP); // ��� ������ �����������
    en_am2302 = read_eep(EE_EN_AM2302);
   // dst_yn = read_eep(EE_DST_YN);
    srand(3);
}

void Port_Init(void)  // ����������� �����
{
    LATC = 0x00;
    TRISC = 0b00011000;
    LATB = 0x00;
    TRISB = 0b00011111;
    INTCON2 &= (~(1<<7));      // ��������� ���������
    LATD = 0x00;
    TRISD = 0x00;
    LATA = 0;
    TRISA = 0b00000001;
#ifdef __18F4525__
    ADCON0bits.CHS = 0b0000; // ���������� ���� - 0
    ADCON1bits.PCFG = 0b1110; // ���� RA0 - ����������
    ADCON1bits.VCFG = 0b00; // Voltage Reference - �� VSS, VDD
    ADCON2bits.ADCS = 0b010; //FOSC/32
    ADCON2bits.ADFM = 1; // ����� �����������
    ADCON0bits.ADON = 1; // ���. ������ ���
#endif
   
#ifdef __18F452__
    ADCON0bits.CHS = 0b000; // ���������� ���� - 0
    ADCON0bits.ADCS = 0b10; //FOSC/32
    ADCON1bits.ADCS2 = 0; //FOSC/32
    ADCON1bits.ADFM = 1; // ����� �����������
    ADCON1bits.PCFG = 0b1110; // ���� RA0 - ����������
    ADCON0bits.ADON = 1; // ���. ������ ���
#endif
    INTCON2bits.INTEDG0 = 1;    // ����������� �� ���������� ������
    INTCONbits.INT0IF = 0;      // ������� ������� ����������� �� ���������� �����
    INTCONbits.INT0IE = 1;      // ���. ����������� �� ����� RB0
    DQ = 1;
}

void Interrupt_Init(void) // ��� �����������
{
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
}