//************************************************ 
//  BMP180 Barometric Pressure Sensor 
//************************************************ 
#include "bmp180.h"



// перемінні калібрування
int16_t ac1; 
int16_t ac2; 
int16_t ac3; 
uint16_t ac4; 
uint16_t ac5; 
uint16_t ac6; 
int16_t b1; 
int16_t b2; 
int16_t mb; 
int16_t mc; 
int16_t md; 


//signed int32 UT;  // незкомпенсована температура
//signed int32 UP;  // незкомпенсований тиск

int32_t X1;  //   перемінні для
int32_t X2;  //       тимчасових
int32_t X3;  //       розрахунків
int32_t B3;  // 
uint32_t B4;  //
int32_t B5;  // 
int32_t B6;  // 
uint32_t B7;  // 

int32_t T;  // 
//signed int32 Pr;  // 






//********************************************* 
//  Процедура читання 1-го байту з датчика
//  address: I2C ареса датчика  
//  повертає 1 байт прчитаних даних  
//---------------------------------------------- 
int8_t BMP085ReadByte(uint8_t address) 
//---------------------------------------------- 
{ 
int8_t data; 

   I2C_start(BMP085_ADDRESS,0);
   I2C_write(address);
   I2C_restart(BMP085_ADDRESS,1);
   data = I2C_read_noack();
   I2C_stop();                   
   return(data); 
} 

//********************************************* 
//  Процедура читання 2-х байтового числа з датчика
//  address: I2C ареса датчика  
//  повертає 16 - ти розрядне ціле
//---------------------------------------------- 
int16_t bmp085ReadInt(uint8_t address) 
//---------------------------------------------- 
{ 
int8_t msb, lsb; 
//int16_t temp; 

   I2C_start(BMP085_ADDRESS,0);
   I2C_write(address);
   I2C_restart(BMP085_ADDRESS,1);
   msb = i2c_read_ack();
   lsb = I2C_read_noack();
   I2C_stop();  
   return ( msb << 8 + lsb ); 
} 

//********************************************* 
//  Процедура запису 1-го байту в датчик
//  address: I2C ареса датчика  
//  повертає: нічого  
//---------------------------------------------- 
void BMP085WriteByte(uint8_t address, int8_t data) 
//---------------------------------------------- 
{ 
   I2C_start(BMP085_ADDRESS,0);
   I2C_write(address);
   I2C_write(data); 
   I2C_stop();   
} 

//********************************************* 
//  Процедура читання калібровочних даних
//    
// 
//---------------------------------------------- 
void bmp085Calibration() 
//---------------------------------------------- 
{ 
   // читаємо дані калібрування 
//   ac1 = bmp085ReadInt(0xAA); 
//   ac2 = bmp085ReadInt(0xAC); 
//   ac3 = bmp085ReadInt(0xAE); 
//   ac4 = bmp085ReadInt(0xB0); 
//   ac5 = bmp085ReadInt(0xB2); 
//   ac6 = bmp085ReadInt(0xB4); 
//   b1  = bmp085ReadInt(0xB6); 
//   b2  = bmp085ReadInt(0xB8); 
//   mb  = bmp085ReadInt(0xBA); 
//   mc  = bmp085ReadInt(0xBC); 
//   md  = bmp085ReadInt(0xBE); 

   ac1 =   408;
   ac2 =   -72;
   ac3 =   -14383;
   ac4 =   32741;
   ac5 =   32757;
   ac6 =   23153;
   b1 =   6190;
   b2 =   4;
   mb = -32767;
   mc = -8711;
   md = 2868;
   
#ifdef DEBUG
    __delay_ms(200);
    printf("AC1: %d\n\r", ac1);
    printf("AC2: %d\n\r", ac2);
    printf("AC3: %d\n\r", ac3);
    printf("AC4: %d\n\r", ac4);
    printf("AC5: %d\n\r", ac5);
    printf("AC6: %d\n\r", ac6);
    printf("B1: %d\n\r", b1);
    printf("B2: %d\n\r", b2);
    printf("MB: %d\n\r", mb);
    printf("MC: %d\n\r", mc);
    printf("MD: %d\n\r", md);
#endif

} 


// Процедура читання незкомпенсованих даних температури 
//---------------------------------------------- 
int32_t BMP085ReadUT() 
//---------------------------------------------- 
{ 
  
  // Пишемо 0x2E в регістр 0xF4 
 // BMP085WriteByte(0xF4, 0x2E); 
  __delay_ms(5); // пауза 4,5 мс.
  // читаємо 2 байти з регістрів 0xF6 і 0xF7 
  ///return(bmp085ReadInt(0xF6));
  return 27898;
} 


// Процедура читання незкомпенсованих даних тиску
//---------------------------------------------- 
int32_t bmp085ReadUP() 
//---------------------------------------------- 
{ 
uint8_t msb, lsb, xlsb; 
int32_t p; 
  
  // Пишемо 0x34+(OSS<<6) в регістр 0xF4 
  // OSS - значення роздільної здатності 
 // BMP085WriteByte(0xF4, (0x34 + (OSS<<6)) ); 
  
  // Чекаємо на конвертацію даних, відповідно до значення роздільної здатності
  switch (OSS) 
  { 
     case 0: __delay_ms(5);  break; 
     case 1: __delay_ms(8);  break; 
     case 2: __delay_ms(14); break; 
     case 3: __delay_ms(26); break; 
  }    
  
  // Читаємо регістр 0xF6 (MSB), 0xF7 (LSB), і 0xF8 (XLSB) 
//   msb  = BMP085ReadByte(0xF6); 
//   lsb  = BMP085ReadByte(0xF7); 
//   xlsb = BMP085ReadByte(0xF8); 
//   p = (((uint32_t)msb<<16) + ((uint32_t)lsb<<8) + xlsb) >> (8-OSS); 
   //return(p);
   return 23843;
}

// Процедура перетворення незкомпенсованої температури в вірну
//---------------------------------------------- 
int32_t BMP085GetTemp(int32_t _tu) 
//---------------------------------------------- 
{ 
int32_t Tt; 
//X1 = ((int32_t)_tu - (int32_t)ac6) * (int32_t)ac5 >> 15;
X1 = (((int32_t)_tu - (int32_t)ac6) * (int32_t)ac5) / 32768;
X2 = ((int32_t)mc << 11) / (X1 + md);
B5 = X1 + X2;
//Tt = (B5+8) >> 4;
Tt = (B5+8) / 16;
return(Tt); 
}  

 
// Процедура перетворення незкомпенсованого тиску в вірний
//---------------------------------------------- 
int32_t BMP085GetPressure(int32_t _pu) 
//---------------------------------------------- 
{ 
int32_t Pp;

   B6 = B5 - 4000;
  // X1 = (b2*((B6*B6)>>12))>>11;
   X1 = (b2*(B6*B6/4096))/2048;
   //X2 = ac2 * B6 >> 11;
   X2 = ac2 * B6 / 2048;
   X3 = X1 + X2;
   B3 = ((((int32_t)ac1 * 4 + X3) << OSS) + 2) / 4;
//   X1 = (ac3 * B6) >> 13;
   X1 = ac3 * B6 / 8192;
//   X2 = (b1*((B6*B6) >> 12)) >> 16;
   X2 = (b1*(B6*B6 / 4096)) / 65536;
//   X3 = ((X1 + X2)+2) >> 2;
   X3 = ((X1 + X2)+2) / 4;
//   B4 = (ac4 * (uint32_t)(X3 + 32768)) >> 15;
   B4 = (ac4 * (uint32_t)(X3 + 32768)) / 32768;
   B7 = ((uint32_t)_pu - B3) * (50000>>OSS);
   
   if (B7 < 0x80000000)
      Pp = (B7*2)/B4;
   else
      Pp = (B7/B4)*2;
    X1 = (Pp/256)*(Pp/256);//?????
    X1 = (X1*3038)/65536;
    X2 = -7357 * Pp / 65536;
    Pp = Pp+((X1+X2+3791)/16);
    Pp = Pp * 0.7501;
     
   return(Pp); 
} 

//  Процедура визначення тиску
//  getTemp: чи будемо читати спочатку температуру(потрібно якщо ми ще не читали температуру)
//---------------------------------------------- 
uint32_t BMP085Pressure(uint8_t getTemp) 
//---------------------------------------------- 
{ 
   if (getTemp) 
      T = BMP085GetTemp(BMP085ReadUT());  //  
   return(BMP085GetPressure(bmp085ReadUP())); 
} 


//---------------------------------------------- 
uint32_t BMP085Temperature(void) 
//---------------------------------------------- 
{ 
   T = BMP085GetTemp(BMP085ReadUT()); 
   //_Temp = BMP085GetTemp(26572); 
   
   return(T);
} 



