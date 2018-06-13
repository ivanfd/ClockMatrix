#include "sound.h"


uint8_t count_sound = 0;    // ��������� ������� �����

uint8_t play_sound = 0; //  �� ����� ����������

//��� ����������� �������
uint8_t cnt_snd_h = 0;
uint8_t cnt_b = 0;
uint16_t cnt_p = 0;
uint8_t en_h_snd = 0; // ���� ����� ���������� ������
uint8_t h_snd_t = 0; //������ ������� � ��������
uint8_t snd_flag = 1; // ���� ��� �����������

void sound_init()
{
  SOUND_TRIS &= ~(1<<SOUND_PIN);  // ������ ���� �������
  SOUND_PORT &= ~(1<<SOUND_PIN); //x &= ~(1 << (y))  |= (1<<SOUND_PIN)
}

void TMR3_ISR() {

    if (play_sound) {
        count_sound++;


        if (count_sound < SOUND_DELAY)
            SOUND_PORT ^= (1 << SOUND_PIN);
        else {
            count_sound = 0;
            play_sound = 0;
            SOUND_PORT &= ~(1 << SOUND_PIN);
        }
    }
    //--------------------------------------------------------

    if (en_h_snd) { // ���� ����� ���������� ���������� ������
        if (h_snd_t) { // ���� ������ �������
            snd_flag = 0; // ���������� �� ��� ����������
            if (cnt_b == 0) {//�������� ������ ���
                cnt_snd_h++;
                if (cnt_snd_h < SOUND_DELAY_H)
                    SOUND_PORT ^= (1 << SOUND_PIN);
                else {
                    cnt_snd_h = 0;
                    cnt_b = 1;
                    SOUND_PORT &= ~(1 << SOUND_PIN);
                }
            }
            if (cnt_b == 1)
                cnt_p++; // �������� ����� �����
            if (cnt_p > 300) {
                cnt_p = 0;
                cnt_b = 2;
            }
            if (cnt_b == 2) {//�������� ������ ���
                cnt_snd_h++;
                if (cnt_snd_h < SOUND_DELAY_H)
                    SOUND_PORT ^= (1 << SOUND_PIN);
                else {
                    cnt_snd_h = 0;
                    cnt_b = 0;
                    h_snd_t = 0;
                    SOUND_PORT &= ~(1 << SOUND_PIN);
                }
            }
        }
    }

    //       if (count_sound <= 127)    (x)^=  (1 << (y))
    //        SOUND_PORT |= (1<<SOUND_PIN);
    //    else
    //        SOUND_PORT &= ~(1<<SOUND_PIN);

}
