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
    } if (PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF ==1)
    {
        PIR1bits.TMR1IF = 0; // ������� ������� ����������� �� ������� 1
         TMR1H = HIGH_BYTE(TMR1Val);
         TMR1L = LOW_BYTE(TMR1Val);
        TMR1_ISR();
    }
}
