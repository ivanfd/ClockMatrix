#include "timer.h" 

//volatile uint16_t RelTMR = 16536;

void TMR1Init(void)
{
  T1CONbits.TMR1ON	 = 0;
  TMR1H = HIGH_BYTE(TMR1Val);
  TMR1L = LOW_BYTE(TMR1Val); 
  PIE1bits.TMR1IE = 1; // переривання від таймера 1
  
}


//void TMR1Reload(uint16_t value)
//{
//       TMR1H = (value >> 8);
//       TMR1L = value; 
//}

