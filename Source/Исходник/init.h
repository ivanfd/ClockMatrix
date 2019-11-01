
#ifndef INIT_H
#define	INIT_H

// якщо контроллер 18F4525
#ifdef __18F4525__

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

#endif

// якщо контроллер 18F4525
#ifdef __18F452__
    // PIC18F452 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
    #pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator with PLL enabled/Clock frequency = (4 x FOSC))
    #pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

    // CONFIG2L
    #pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
    #pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)
    #pragma config BORV = 27        // Brown-out Reset Voltage bits (VBOR set to 2.7V)

    // CONFIG2H
    #pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
    #pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

    // CONFIG3H
    #pragma config CCP2MUX = ON     // CCP2 Mux bit (CCP2 input/output is multiplexed with RC1)

    // CONFIG4L
    #pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause RESET)
    #pragma config LVP = OFF        // Low Voltage ICSP Enable bit (Low Voltage ICSP disabled)

    // CONFIG5L
    #pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
    #pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
    #pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
    #pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

    // CONFIG5H
    #pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
    #pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

    // CONFIG6L
    #pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
    #pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
    #pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
    #pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

    // CONFIG6H
    #pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
    #pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
    #pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

    // CONFIG7L
    #pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
    #pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
    #pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
    #pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

    // CONFIG7H
    #pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

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
#endif


#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "max7221.h"
#include "i2c.h"
#include "interrupt.h" 
#include "common.h"
#include "timer.h"
#include "dispatcher.h"
#include "eusart.h"
//#include "bmp180.h"
#include "BMP_280.h"
#include "eeprom.h"
#include "spi.h"
#include "nrf24l01p.h"
#include <stdlib.h>

void SYSTEM_Initialize(void);  // ≥н≥ц≥ал≥зац≥€ контролера
void Port_Init(void);  // ≥н≥ц≥ал≥зац≥€ порт≥в
void Interrupt_Init(void); // ≥н≥т перериванн€



#endif	/* XC_HEADER_TEMPLATE_H */

