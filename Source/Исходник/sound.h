
#ifndef SOUND_H
#define	SOUND_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "display.h"

#define SOUND_PORT LATD
#define SOUND_TRIS TRISD
#define SOUND_PIN 7

#define SOUND_DELAY 30
#define SOUND_DELAY_H 20


extern uint8_t en_h_snd;
extern uint8_t h_snd_t; //������ ������� � ��������
extern uint8_t play_sound; //  �� ����� ����������
extern uint8_t snd_flag; // ���� ��� �����������


void sound_init();

void TMR3_ISR();

#endif	/* XC_HEADER_TEMPLATE_H */

