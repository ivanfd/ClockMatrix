#include "time.h"
#include "display.h"

uint8_t day_in_m[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // ������� ��� � ������
//*******************************************************
//            ��������� �� ����� ���� ���
//*******************************************************

void dst_time(struct Time_Get *pTime, uint8_t *dst) {
    
uint8_t Mrt, Oct; // ������ �� ������, �����

    Mrt = (unsigned)31 - DayOfWeek(31, 3, (pTime->Tyr)); //������� ����� �������, �����
    Oct = (unsigned)31 - DayOfWeek(31, 10, (pTime->Tyr)); //������� ����� ������, �����

        // ����������, �� ��� ����
    if (((pTime->Tmt > 3) && (pTime->Tmt < 10)) ||
            ((pTime->Tmt == 3) && (pTime->Tdt > Mrt)) ||
            ((pTime->Tmt == 3) && (pTime->Tdt == Mrt) && (pTime->Thr >= 3)) ||
            ((pTime->Tmt == 10) && (pTime->Tdt < Oct)) ||
            ((pTime->Tmt == 10) && (pTime->Tdt == Oct) && (pTime->Thr <= 1)))
        *dst = 1; // ���� ���, �� ������� �������
    else
        *dst = 0;
    
//    pTime->Tdt = 29;
//    pTime->Thr = 23;
//    pTime->Tmt = 2;
//    pTime->Tyr = 20;

    if (*dst) {
        if (pTime->Thr == 23) { // ���� ������ ���� 23
            pTime->Thr = 0;
            pTime->Tdy++;
            if (pTime->Tdy == 8)
                pTime->Tdy = 1;
            if ((pTime->Tyr) % 4) { // ���� �� �� ����������
                if ((pTime->Tdt == day_in_m[(pTime->Tmt)-(unsigned) 1]) | ((pTime->Tmt == 2)&&(pTime->Tdt == 28))) { // ���� ����� ���� ���������� ����� �����
                    if (pTime->Tmt == 12) {// ���� ����� �������
                        pTime->Tyr++; // �������� ��
                        pTime->Tdt = 1; // ������� ����� 1
                        pTime->Tmt = 1;
                    } else {
                        pTime->Tdt = 1; // ������� ����� 1
                        pTime->Tmt++;
                    }
                } else {
                    pTime->Tdt++; // �������� �� 1
                    //pTime->Tmt++;
                }
            } else { //���� ����������
                if ((pTime->Tdt == day_in_m[(pTime->Tmt) - (unsigned)1])) { // ���� ����� ���� ���������� ����� �����
                    if (pTime->Tmt == 12) {// ���� ����� �������
                        pTime->Tyr++; // �������� ��
                        pTime->Tdt = 1; // ������� ����� 1
                        pTime->Tmt = 1; // ����� �����
                    } else {
                        pTime->Tdt = 1; // ������� ����� 1
                        pTime->Tmt++;
                    }
                } else {
                    pTime->Tdt++; // �������� �� 1
                }
            }

        } else
            pTime->Thr = (unsigned) pTime->Thr + 1; // ������ �� ������ 1

    }
}

//**********************************************************
//                        ���� �����
//**********************************************************

uint8_t DayOfWeek (uint8_t day, uint8_t month, uint8_t year)
 {
  uint8_t a = (uint8_t)((14 - month) / 12);
  uint8_t y = (uint8_t)(year - a);
  uint8_t m = (uint8_t)(month + 12 * a - 2);
  return (uint8_t)((7000 + (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12)) % 7);
}