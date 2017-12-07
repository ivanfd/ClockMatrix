
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
#include "bmp_280.h"
#include "eusart.h"


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

#define cycle_main 20       // ����� ��������� ������, �������
//#define NULL  0

#define EE_FONT 0 // ������ � ������ ���� ������
#define EE_TYPE_CLK 1 // ������ � ������ ���� ���������
#define EE_TYPE_BRG 2 // ������ � ������ ���� ���������
#define EE_DAT_BRG 3 // ������ � ������ �������� ���������

#define TYPE_CLK_1 1  // ������ ���������
#define TYPE_CLK_2 2  // ������ ���������
#define MAX_BRIG 10 // ����������� ���������

 void INT0_ISR(void);
 void GetTime(void);
 void time_set_min(void);
 void time_set_hr(void);
 void time_set_yr(void);
 void time_set_mt(void); 
 void time_set_dt(void); 
 void time_set_dy(void);
 void TMR1_ISR();
 void time_led(); 
 void version(void);
 void default_state(void);
 void home_temp(void);
 void set_font();
 void set_font_set(void);
 void pressure(void);
 void pre_ref_dis(void);
 void set_type_clk(void);
 void radio_temp(void);
 void read_adc();
 void adj_brig();
 void brg_set();
 void set_brg_manual();
 void usart_r();

#endif	/* XC_HEADER_TEMPLATE_H */

