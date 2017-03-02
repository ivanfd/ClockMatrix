#include "eusart.h"



void putch(char data)
{
 while( ! TXIF)
 continue;
 TXREG = data;
}


void init_uart(void)
{
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;
    TXSTAbits.BRGH = 1; // 1 = High speed
    TXSTAbits.SYNC = 0; // 0 = Asynchronous mode
    TXSTAbits.TXEN = 1; // 1 = Transmit enabled
    RCSTAbits.CREN = 1; // 1 = Enables receiver
    RCSTAbits.SPEN = 1; // 1 = Serial port enabled (configures RX/DT and TX/CK pins as serial port pins)
    BAUDCONbits.BRG16 = 1; // 1 = 16-bit Baud Rate Generator – SPBRGH and SPBRG
    
    // BAUD = FOSC/[4 (n + 1)]
    // n = value of SPBRGH:SPBRG register pair
    //  SPBRGH:SPBRG = ((40000000/9600)/4) - 1 = 1040
    // BAUDRATE = 40000000/4*(1040 + 1) = 9606
    // ERROR = (9606 - 9600)/9600 = 0.06%
    
    SPBRGH = 0x04;
    SPBRG = 0x10;
}
