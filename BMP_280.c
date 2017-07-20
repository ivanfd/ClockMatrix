#include "BMP_280.h"
#include <stdlib.h>

uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;
//int16_t dig_H1;

//  ініціалізація датчика

void bmp280_Init() {


    dig_T1 = bmp280ReadInt16(0x88);
    dig_T2 = bmp280ReadInt16(0x8A);
    dig_T3 = bmp280ReadInt16(0x8C);
    dig_P1 = bmp280ReadInt16(0x8E); //dig_P1
    dig_P2 = bmp280ReadInt16(0x90); //dig_P2
    dig_P3 = bmp280ReadInt16(0x92); //dig_P3
    dig_P4 = bmp280ReadInt16(0x94); //dig_P4
    dig_P5 = bmp280ReadInt16(0x96); //dig_P5
    dig_P6 = bmp280ReadInt16(0x98); //dig_P6
    dig_P7 = bmp280ReadInt16(0x9A); //dig_P7
    dig_P8 = bmp280ReadInt16(0x9C); //dig_P8
    dig_P9 = bmp280ReadInt16(0x9E); //dig_P9  
    // dig_H1 = bmp280ReadInt16(0xA1);

    bmp280WriteByte(BMP280_REG_CONFIG, (BMP280_TSB_2000 | BMP280_FILTER_COEFFICIENT8 | BMP280_SPI_OFF));
    bmp280WriteByte(BMP280_REG_CONTROL, (BMP280_OVERSAMPLING_T16 | BMP280_OVERSAMPLING_P16 | BMP280_MODE_NORMAL));

#ifdef DEBUG
    dig_T1 = 27504;
    dig_T2 = 26435;
    dig_T3 = -1000;
    dig_P1 = 36477;
    dig_P2 = -10685;
    dig_P3 = 3024;
    dig_P4 = 2855;
    dig_P5 = 140;
    dig_P6 = -7;
    dig_P7 = 15500;
    dig_P8 = -14600;
    dig_P9 = 6000;
#endif

#ifdef DEBUG
    __delay_ms(200);
    printf("dig_T1: %u\n\r", dig_T1);
    printf("dig_T2: %d\n\r", dig_T2);
    printf("dig_T3: %d\n\r", dig_T3);
    printf("dig_P1: %u\n\r", dig_P1);
    printf("dig_P2: %d\n\r", dig_P2);
    printf("dig_P3: %d\n\r", dig_P3);
    printf("dig_P4: %d\n\r", dig_P4);
    printf("dig_P5: %d\n\r", dig_P5);
    printf("dig_P6: %d\n\r", dig_P6);
    printf("dig_P7: %d\n\r", dig_P7);
    printf("dig_P8: %d\n\r", dig_P8);
    printf("dig_P9: %d\n\r", dig_P9);
#endif

}

//=====================================================
//      Читаємо з датчика 16 біт - по двох адресах
//              
//=====================================================
uint16_t bmp280ReadInt16(uint8_t address) {
    uint16_t msb = 0;
    uint16_t lsb = 0;

    I2C_Master_Start();
    I2C_Master_Write(BMP280_ADDRESS);
    I2C_Master_Write(address);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(BMP280_ADDRESS | 0x01);
    lsb = I2C_Master_Read(1); // читаємо з підтвердженням
    msb = I2C_Master_Read(0); // читаємо без підтвердження
    I2C_Master_Stop();
    //     temp = msb << 8;
    //     temp |= lsb;
    return (msb << 8) | lsb;


}

//=====================================================
//      Читаємо з датчика 32 біти - по трьох адресах
//              
//=====================================================

uint32_t bmp280ReadInt32(uint8_t address) {
    uint32_t msb = 0;
    uint32_t lsb = 0;
    uint32_t xlsb = 0;

    I2C_Master_Start();
    I2C_Master_Write(BMP280_ADDRESS);
    I2C_Master_Write(address);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(BMP280_ADDRESS | 0x01);
    msb = (uint32_t) I2C_Master_Read(1); // читаємо з підтвердженням
    lsb = (uint32_t) I2C_Master_Read(1); // читаємо без підтвердження
    xlsb = (uint32_t) I2C_Master_Read(0);
    I2C_Master_Stop();
    //     temp = msb << 8;
    //     temp |= lsb;
    return ((msb << 16) + (lsb << 8) + xlsb) >> 4;


}

void bmp280WriteByte(uint8_t address, int8_t data)
//---------------------------------------------- 
{
    I2C_Master_Start();
    I2C_Master_Write(BMP280_ADDRESS);
    I2C_Master_Write(address);
    I2C_Master_Write(data);
    I2C_Master_Stop();
}

//********************************************* 
//  Процедура читання 1-го байту з датчика
//  address: I2C ареса датчика  
//  повертає 1 байт прчитаних даних  
//---------------------------------------------- 

uint8_t bmp280ReadByte(uint8_t address)
//---------------------------------------------- 
{
    uint8_t data;

    I2C_Master_Start();
    I2C_Master_Write(BMP280_ADDRESS);
    I2C_Master_Write(address);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(BMP280_ADDRESS | 0x01);
    data = I2C_Master_Read(0); // читаємо без підтвердження
    I2C_Master_Stop();
    return (data);
}

//===========================================
//   Читаємо температуру
//===========================================

int32_t bmp280ReadTemp() {
    int32_t temperature = 0;


    temperature = bmp280ReadInt32(BMP280_REG_RESULT_TEMPRERATURE);

#ifdef DEBUG
    __delay_ms(200);

    temperature = 519888;
    printf("UncTemp: %ld\n\r", temperature);


#endif


    //		temperature = bmp280ReadLong(BMP280_REG_RESULT_TEMPRERATURE);
    return temperature;
}


//===========================================
//   Читаємо тиск
//===========================================

int32_t bmp280ReadPressure() {
    int32_t pressure = 0;


    pressure = bmp280ReadInt32(BMP280_REG_RESULT_PRESSURE);

#ifdef DEBUG
    __delay_ms(200);
    pressure = 415148;
    printf("UncPress: %ld\n\r", pressure);


#endif


    return pressure;
}

uint8_t bmp280Convert(int32_t *press, int32_t *temper) {
    int32_t adc_T;
    int32_t adc_P;
    //  int32_t temperature1;
    //  int32_t pressure1;

    // double var1, var2, T, P, tfine;
    int32_t var1, var2, t_fine;
    uint32_t T, P;



    adc_T = bmp280ReadTemp();
    adc_P = bmp280ReadPressure();

    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2)) >> 11;
#ifdef DEBUG
    printf("var1 =  %lu\r", var1);
#endif
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) * ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3)) >> 14;
#ifdef DEBUG
    printf("var2 =  %ld\r", var2);
#endif
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
#ifdef DEBUG
    printf("T1 =  %ld\r", T);
#endif    
    *temper = T;

    var1 = (((int32_t) t_fine) >> 1) - (int32_t) 64000;
#ifdef DEBUG
    printf("var1 =  %ld\r", var1);
#endif  
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) dig_P6);
#ifdef DEBUG
    printf("var2 =  %ld\r", var2);
#endif  
    var2 = var2 + ((var1 * ((int32_t) dig_P5)) << 1);
#ifdef DEBUG
    printf("var2 =  %ld\r", var2);
#endif  
    var2 = (var2 >> 2)+(((int32_t) dig_P4) << 16);
#ifdef DEBUG
    printf("var2 =  %ld\r", var2);
#endif  
    var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t) dig_P2) * var1) >> 1)) >> 18;
#ifdef DEBUG
    printf("var1 =  %ld\r", var1);
#endif  
    var1 = ((((32768 + var1))*((int32_t) dig_P1)) >> 15);
#ifdef DEBUG
    printf("var1 =  %ld\r", var1);
#endif  
    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    P = (((uint32_t) (((int32_t) 1048576) - adc_P) - (var2 >> 12)))*3125;
    if (P < 0x80000000) {
        P = (P << 1) / ((uint32_t) var1);
    } else {
        P = (P / (uint32_t) var1) * 2;
    }
#ifdef DEBUG
    printf("P =  %ld\r", P);
#endif  

    var1 = (((int32_t) dig_P9) * ((int32_t) (((P >> 3) * (P >> 3)) >> 13))) >> 12;
#ifdef DEBUG
    printf("var1 =  %ld\r", var1);
#endif  
    var2 = (((int32_t) (P >> 2)) * ((int32_t) dig_P8)) >> 13;
#ifdef DEBUG
    printf("var2 =  %ld\r", var2);
#endif  
    P = (uint32_t) ((int32_t) P + ((var1 + var2 + dig_P7) >> 4));
#ifdef DEBUG
    printf("P =  %lu\r", P);
#endif  

    //*press = P;
    *press = P * 100 / 13332;

    //    var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
    //    //var9=(double)45678.125;
    //    #ifdef DEBUG
    //        printf("var1 =  %f\r", var1);
    //    #endif
    //	var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
    //    #ifdef DEBUG
    //        printf("var2 =  %f\r", var2);
    //    #endif
    //
    //	tfine = (var1 + var2);
    //    #ifdef DEBUG
    //        printf("tfine =  %f\r", tfine);
    //    #endif
    //    
    //	*temper = (int32_t) (tfine*10 / 5120.0);
    //    #ifdef DEBUG
    //        printf("temper =  %u\r", *temper);
    //    #endif
    //    
    //    var1 = ((double)tfine/2.0) - 64000.0;
    //    #ifdef DEBUG
    //        printf("var1 =  %f\r", var1);
    //    #endif
    //	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
    //    #ifdef DEBUG
    //        printf("var2 =  %f\r", var2);
    //    #endif
    //	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
    //    #ifdef DEBUG
    //        printf("var2 =  %f\r", var2);
    //    #endif
    //	var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
    //    #ifdef DEBUG
    //        printf("var2 =  %f\r", var2);
    //    #endif
    //	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
    //    #ifdef DEBUG
    //        printf("var1 =  %f\r", var1);
    //    #endif
    //	var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
    //    #ifdef DEBUG
    //        printf("var1 =  %f\r", var1);
    //    #endif
    //
    //	P = 1048576.0 - (double)adc_P;
    //    #ifdef DEBUG
    //        printf("P =  %f\r", P);
    //    #endif
    //	P = (P - (var2 / 4096.0)) * 6250.0 / var1;
    //    #ifdef DEBUG
    //        printf("P =  %f\r", P);
    //    #endif
    //	var1 = ((double)dig_P9) * P * P / 2147483648.0;
    //    #ifdef DEBUG
    //        printf("var1 =  %f\r", var1);
    //    #endif
    //	var2 = P * ((double)dig_P8) / 32768.0;
    //    #ifdef DEBUG
    //        printf("var2 =  %f\r", var2);
    //    #endif
    //	P = (P + (var1 + var2 + ((double)dig_P7)) / 16.0);
    //    #ifdef DEBUG
    //        printf("P =  %f\r", P);
    //    #endif
    //
    //	*press = ((uint32_t)P)*100/13332;
    //    //*press = *press  * 100/13332;    
#ifdef DEBUG
    __delay_ms(200);

    printf("Temp280: %d\n\r", *temper);
    printf("Press280: %u\n\r", *press);
#endif

}
