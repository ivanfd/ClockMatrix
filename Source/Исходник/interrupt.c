#include "interrupt.h"


void interrupt INTERRUPT_InterruptManager (void)
{
    // interrupt handler
    if(INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1)
    {
        TMR0_ISR();
    }
    else if (INTCONbits.INT0IE == 1 && INTCONbits.INT0IF == 1)
    {
        INTCONbits.INT0IF = 0;      // ������� ������� �����������
        INTCON2bits.INTEDG0 = ~INTCON2bits.INTEDG0; // ���������� � ����������� �� �������� ������
                                                    // ��� ����������� ���� ������ 500��
       INT0_ISR();
    } else if (PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF ==1)
    {
        PIR1bits.TMR1IF = 0; // ������� ������� ����������� �� ������� 1
         TMR1H = HIGH_BYTE(TMR1Val);
         TMR1L = LOW_BYTE(TMR1Val);
         //TMR1L = (uint8_t)TMR1Val;
        TMR1_ISR();
        
    } else if (PIE2bits.TMR3IE == 1 && PIR2bits.TMR3IF ==1) {
        PIR2bits.TMR3IF = 0;
        TMR3H = HIGH_BYTE(TMR3Val);
        TMR3L = LOW_BYTE((unsigned int)TMR3Val);
        //TMR3L = (uint8_t)TMR3Val;

        TMR3_ISR();
    } else if (INTCONbits.PEIE == 1 && PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1) {
        EUSART_Receive_ISR();
    }
}
