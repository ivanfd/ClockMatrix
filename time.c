#include "time.h"
#include "display.h"

//*******************************************************
//            Визначаємо чи зараз літній час
//*******************************************************

void dst_time(struct Time_Get *pTime, uint8_t *dst) {
    
uint8_t Mrt, Oct; // останні дні місяців, число

    Mrt = 31 - DayOfWeek(31, 3, (pTime->Tyr)); //остання неділя березня, число
    Oct = 31 - DayOfWeek(31, 10, (pTime->Tyr)); //остання неділя жовтня, число

        // перевіряємо, чи час літній
    if (((pTime->Tmt > 3) && (pTime->Tmt < 10)) ||
            ((pTime->Tmt == 3) && (pTime->Tdt > Mrt)) ||
            ((pTime->Tmt == 3) && (pTime->Tdt == Mrt) && (pTime->Thr >= 3)) ||
            ((pTime->Tmt == 10) && (pTime->Tdt < Oct)) ||
            ((pTime->Tmt == 10) && (pTime->Tdt == Oct) && (pTime->Thr <= 1)))
        *dst = 1; // якщо так, то ставимо признак
    else
        *dst = 0;

    if (*dst)
        if (pTime->Thr == 23)
            pTime->Thr = 0;
        else
            pTime->Thr = pTime->Thr + 1; // додаємо до години 1


}

//**********************************************************
//                        день тижня
//**********************************************************

uint8_t DayOfWeek (uint8_t day, uint8_t month, uint8_t year)
 {
  uint8_t a = (14 - month) / 12;
  uint8_t y = year - a;
  uint8_t m = month + 12 * a - 2;
  return (7000 + (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12)) % 7;
}