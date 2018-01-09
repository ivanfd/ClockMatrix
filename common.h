
#ifndef COMMON_H
#define	COMMON_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>
#include <stdio.h>
#include "Font.h"
#include "dispatcher.h"
#include "display.h"
#include "interrupt.h"
#include "key.h"
#include "events.h"
#include "main.h"
#include "onewire.h"
#include "ds18b20.h"
#include "BMP_280.h"
#include "eusart.h"
#include "strings.h"
#include "settings.h"
#include <stdint.h>
#include <string.h>


#define SetBit(x,y)    do{ x |=  (1 << (y));} while(0)
#define ClrBit(x,y)    do{ x &= ~(1 << (y));} while(0)
#define InvBit(x,y)    do{(x)^=  (1 << (y));} while(0)
#define IsBit(x,y)        (x &   (1 << (y)))

//#define LOW_BYTES(x) ((unsigned char)((x) & 0xFF))

#define TRUE 1
#define FALSE 0

#define HIGH  1
#define LOW   0

#define ON    1
#define OFF   0

#define cycle_main 20       // період виконання задачі, основна
//#define NULL  0

#define EE_FONT 0 // адреса в еепром типу шрифту
#define EE_TYPE_CLK 1 // адреса в еепром типу годинника
#define EE_TYPE_BRG 2 // адреса в еепром типу яскравості
#define EE_DAT_BRG 3 // адреса в еепром значення яскравості
#define EE_EN_SND_H 4 //адреса в еепром, чи можна відтворювати щогодинний сигнал
#define EE_EN_DS1 5 // еепром, чи показувати температуру з датчика 1
#define EE_EN_DS2 6 // еепром, чи показувати температуру з датчика 2
#define EE_EN_BMP 7 // еепром, датчик тиску

#define TYPE_CLK_1 1  // вигляд годинника
#define TYPE_CLK_2 2  // вигляд годинника
#define MAX_BRIG 10 // максимальна яскравість

 void INT0_ISR(void);
 void GetTime(void);
 


 void TMR1_ISR();
 void time_led(); 
 void version(void);

 void home_temp(void);
 void set_font();
 
 void pressure(void);
 void pre_ref_dis(void);

 void radio_temp(void);
 void read_adc();
 void adj_brig();

 void usart_r();



#endif	/* XC_HEADER_TEMPLATE_H */

