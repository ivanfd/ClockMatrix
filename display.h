
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

#define BUF_SIZE_TEMP 11
#define BUF_SIZE COUNT_MATRIX * 8
#define SPEED_STRING 60     // �������� ����� ������, �������
#define DELAY_SHIFT_DOWN 40 // �������� ������ ����� ����

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

typedef void (*p_MyFunc)();

        
 void FillBufS(uint8_t *buf, uint8_t edit_Flag, uint8_t scr_flag);       
 void FillBuf(uint8_t type);
 
 void putchar_b_buf(uint8_t x, uint8_t symbol, uint8_t (*pF)[][5], uint8_t *buf);
 void putchar_s_buf(uint8_t x, uint8_t symbol);
 void pixel_on(uint8_t x, uint8_t y, uint8_t *buf);
 void pixel_off(uint8_t x, uint8_t y, uint8_t *buf);
 void clear_matrix(void);
 uint8_t scroll_text(uint8_t *buf);
 void putchar_down(uint8_t x, uint8_t symbol, uint8_t (*pF)[10][5]);
 void putchar_down_s(uint8_t x, uint8_t symbol);
 void pic_to_led(uint8_t x, uint8_t pic, uint8_t *buf);
 void scroll_left(void);
 void dissolve(void);
 void interval_scroll_text(uint8_t *buf);
 void scroll_right(void);
 void hide_two_side(void);
 void Rand_ef(void);
 void fill_buff_t(uint16_t data);
 void center_two_side(void);
 void scroll_down_one(void);
 void scroll_text_temp(uint8_t pos);

 
#endif	/* XC_HEADER_TEMPLATE_H */

