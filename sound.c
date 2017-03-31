#include "sound.h"

uint8_t count_sound = 0;    // ��������� ������� �����

uint8_t play_sound = 0; //  �� ����� ����������

void sound_init()
{
  SOUND_TRIS &= ~(1<<SOUND_PIN);  // ������ ���� �������
  SOUND_PORT &= ~(1<<SOUND_PIN); //x &= ~(1 << (y))  |= (1<<SOUND_PIN)
}



void TMR3_ISR()
{
   
   if (play_sound)
   {
   count_sound++; 

       if (count_sound < SOUND_DELAY)
        SOUND_PORT ^= (1<<SOUND_PIN); 
       else
       {
           count_sound = 0;
           play_sound = 0;
           SOUND_PORT &= ~(1<<SOUND_PIN);
       }
   }
   
   //       if (count_sound <= 127)    (x)^=  (1 << (y))
//        SOUND_PORT |= (1<<SOUND_PIN);
//    else
//        SOUND_PORT &= ~(1<<SOUND_PIN);
   
}
