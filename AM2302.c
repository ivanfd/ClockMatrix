/*
Модуль роботи з датчиком
тиску та температури AM2302(DHT22)

Автор: Ivan_fd
Дата: 30.10.2018р
*/

#include "AM2302.h"

   volatile bit ERROR_DT;

//***********************************************
//          Зчитуємо 1 байт з датчика
//***********************************************

uint8_t DHT_GetByte() {
    uint8_t i, Val = 0;

    for (i = 0; i < 8; i++) {
        Val <<= 1;
        DHT_IO_DIR = 1; // робимо порт входом
        while (!DHT_IO);
        __delay_us(30);

        if (DHT_IO) {
            Val |= 1;
        }
        while (DHT_IO);
    }
    return Val;

}

//*************************************************
//        функція читання даних з датчика
// Встановлюємо низький рівень на DHT_IO на 1-10мс.
// Потім встановлюємо високий рівець на 20-40мкс.
//
//*************************************************

uint8_t DHT_GetData(uint8_t *pVal) {
    uint8_t i;
    uint8_t crc = 0;


    DHT_IO = 0; // 0 в порт
    DHT_IO_DIR = 0; // робимо порт виходом
    __delay_us(1500); //пауза 1.5мс
    DHT_IO_DIR = 1; // порт на вихід
    __delay_us(30);



    if (DHT_IO) return (1);
    __delay_us(80);
    if (!DHT_IO) return (2);
    __delay_us(80);


    for (i = 0; i < 5; i++) {
        *pVal = DHT_GetByte();
        pVal++;
    }

    for (i = 0; i < 4; i++) {
        crc += *(pVal - 5 + i);
    }

    if (crc != *(pVal - 1)) {
        ERROR_DT = 1;
        return (3);
    } else {
        ERROR_DT = 0;
        return (0);
    }
}