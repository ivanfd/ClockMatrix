
#ifndef COMMON_H
#define	COMMON_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>
#include <stdio.h>
//#include "max7221.h"
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
#include "time.h"
#include "AM2302.h"



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
#define EE_EN_SND_H 4 //������ � ������, �� ����� ����������� ���������� ������
#define EE_EN_DS1 5 // ������, �� ���������� ����������� � ������� 1
#define EE_EN_DS2 6 // ������, �� ���������� ����������� � ������� 2
#define EE_EN_BMP 7 // ������, ������ �����
#define EE_EN_DST 8 // ������, ���� ��� (��������� �������� �� ���� ���)
#define EE_TYPE_TEMP 9 // ������, ��� ������ �����������
#define EE_EN_AM2302 10 //  ������, �� �������� ��������
#define EE_DST_YN 11 // �� ������� ������� �� ���� ���

#define TYPE_CLK_1 1  // ������ ���������
#define TYPE_CLK_2 2  // ������ ���������
#define MAX_BRIG 10 // ����������� ���������
#define TYPE_TEMP_1 1 //��� ������ �����������
#define TYPE_TEMP_2 2 //��� ������ �����������
#define EN_AM2302 1
#define DIS_AM2302 0


extern uint8_t blk_dot; // ����� �� ������� ������
extern int8_t brg_type;// ��������� �� �������, �� �������
extern uint8_t brig;// �������� ���������
extern uint8_t dst_flag; // �� ����� ���� ���????
extern int8_t en_am2302; //������ ��������
extern int8_t en_bmp280; //  �� �������� ����
extern int8_t en_ds_1;    //  �� ���. ����������� � ������� 1
extern int8_t en_ds_2;    //  �� ���. ����������� � ������� 2
extern int8_t en_dst; // ������� �� ���� ���
extern bit en_put; // �� ����� ������ � ����� �������
extern uint8_t events;
extern uint8_t idx_pnt;
extern bit show_digit; // �� ���������� �����, � ���. �������
extern uint8_t type_clk; // ������ ���������
extern uint8_t type_font;
extern uint8_t type_temp;
extern volatile uint8_t x1, x2, x3, x4, y1, y2, y3, y4; //��� ����� ������� ����
//extern uint8_t dst_yn; // �� ��������� ������� �� ���� ���


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
 void convert_utf(uint8_t *buf);
 void hum(void);

#endif	/* XC_HEADER_TEMPLATE_H */

