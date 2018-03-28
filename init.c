
// PIC18F4525 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#pragma config IDLOC0 = 1;
#pragma config IDLOC1 = 9;
#pragma config IDLOC2 = 7;
#pragma config IDLOC3 = 9;
#pragma config IDLOC4 = 3;
#pragma config IDLOC5 = 0;
#pragma config IDLOC6 = 0;
#pragma config IDLOC7 = 0;

#include "init.h"

//extern const unsigned char userID[ 8 ] @ 0x200000;
extern uint8_t type_font;
extern uint8_t type_clk;
extern uint8_t brg_type;
extern uint8_t brig;// значення яскравості
extern uint8_t blk_dot; // дозвіл на мигання кнопок
extern uint8_t en_h_snd;
extern uint8_t en_ds_1;    //  чи пок. температуру з датчика 1
extern uint8_t en_ds_2;    //  чи пок. температуру з датчика 2
extern uint8_t en_bmp280; //  чи показуємо тиск
extern uint8_t en_dst; // перехід на літній час
//extern const unsigned char userID[8] @ 0x200000;
//volatile unsigned char usrID[8];


void SYSTEM_Initialize(void)  // ініціалізація контролера
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
    RTOS_SetTask(usart_r, 40, cycle_main); // ЗАДАЧА ОПИТУВАННЯ КОМ ПОРТА
    RTOS_SetTask(GetTime, 100, cycle_main);// Задача зчитування даних з RTC
    en_h_snd = read_eep(EE_EN_SND_H);
    en_ds_1 = read_eep(EE_EN_DS1);    
    en_ds_2 = read_eep(EE_EN_DS2);
    en_bmp280 = read_eep(EE_EN_BMP);
    en_dst = read_eep(EE_EN_DST); // перехід на літній час
    srand(3);
}

void Port_Init(void)  // ініціалізація портів
{
    LATC = 0x00;
    TRISC = 0b00011000;
    LATB = 0x00;
    TRISB = 0b00011111;
    INTCON2 &= (~(1<<7));      // підтягуючі резистори
    LATD = 0x00;
    TRISD = 0x00;
    LATA = 0;
    TRISA = 0b00000001;
    ADCON0bits.CHS = 0b0000; // аналоговий вхід - 0
    ADCON1bits.PCFG = 0b1110; // порт RA0 - аналоговий
    ADCON1bits.VCFG = 0b00; // Voltage Reference - до VSS, VDD
    ADCON2bits.ADCS = 0b010; //FOSC/32
    ADCON2bits.ADFM = 1; // праве вирівнювання
    ADCON0bits.ADON = 1; // вмк. модуль АЦП
    INTCON2bits.INTEDG0 = 1;    // переривання по передньому фронту
    INTCONbits.INT0IF = 0;      // скинути признак переривання по зовнішньому входу
    INTCONbits.INT0IE = 1;      // вмк. переривання по входу RB0
    DQ = 1;
}

void Interrupt_Init(void) // ініт переривання
{
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
}