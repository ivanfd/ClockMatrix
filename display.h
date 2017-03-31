
#ifndef DISPLAY_H
#define	DISPLAY_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "max7221.h"
#include <stdint.h>
#include "common.h"
#include "dispatcher.h"
#include "ds3231.h"
#include "Font.h"
#include "FontS.h"
#include "pict.h"
#include "digits_f.h"


#define BUF_SIZE COUNT_MATRIX * 8
#define SPEED_STRING 60     // �������� ����� ������, �������
#define DELAY_SHIFT_DOWN 20 // �������� ������ ����� ����

struct Time_Get             // ��������� ��� �����
{
   uint8_t Ts;          // �������         
   uint8_t Tmin;        // �������    
   uint8_t Thr;         // ������
   uint8_t Tdy;         // ����
   uint8_t Tdt;         // �����   
   uint8_t Tmt;         // �����         
   uint8_t Tyr;         // ��
} TTime,TSTime;

//  ����������� � ������� � �������        
//struct Time_Conv
//{
//   unsigned char s_10;
//   unsigned char s_1;
//   unsigned char min_10;
//   unsigned char min_1;
//   unsigned char hr_10;
//   unsigned char hr_1;
//} TTimeConv ;

        
 void FillBufS(uint8_t *buf, uint8_t edit_Flag, uint8_t scr_flag);       
 void FillBuf(void);
 
 void putchar_b_buf(uint8_t x, uint8_t symbol, uint8_t (*pF)[10][5]);
 void putchar_s_buf(uint8_t x, uint8_t symbol);
 void pixel_on(uint8_t x, uint8_t y);
 void pixel_off(uint8_t x, uint8_t y);
 void clear_matrix(void);
 uint8_t scroll_text(void);
 void putchar_down(uint8_t x, uint8_t symbol, uint8_t (*pF)[10][5]);
 void putchar_down_s(uint8_t x, uint8_t symbol);
 void pic_to_led(uint8_t x, uint8_t pic);
 void scroll_left(void);
 void interval_scroll_text();
 
#endif	/* XC_HEADER_TEMPLATE_H */

