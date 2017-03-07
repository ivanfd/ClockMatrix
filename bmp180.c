//************************************************ 
//  BMP180 Barometric Pressure Sensor 
//************************************************ 
#include "bmp180.h"



// ������� �����������
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


//signed int32 UT;  // ��������������� �����������
//signed int32 UP;  // ���������������� ����

int32_t X1;  //   ������� ���
int32_t X2;  //       ����������
int32_t X3;  //       ����������
int32_t B3;  // 
uint32_t B4;  //
int32_t B5;  // 
int32_t B6;  // 
uint32_t B7;  // 

int32_t T;  // 
//signed int32 Pr;  // 






//********************************************* 
//  ��������� ������� 1-�� ����� � �������
//  address: I2C ����� �������  
//  ������� 1 ���� ��������� �����  
//---------------------------------------------- 
uint8_t BMP085ReadByte(uint8_t address) 
//---------------------------------------------- 
{ 
uint8_t data; 

     I2C_Master_Start();
     I2C_Master_Write(BMP180_ADDRESS);
     I2C_Master_Write(address);
     I2C_Master_RepeatedStart();
     I2C_Master_Write(BMP180_ADDRESS | 0x01);
     data = I2C_Master_Read(0);                   // ������ ��� ������������
     I2C_Master_Stop();
     return(data); 
} 

//********************************************* 
//  ��������� ������� 2-� ��������� ����� � �������
//  address: I2C ����� �������  
//  ������� 16 - �� �������� ����
//---------------------------------------------- 
int16_t bmp085ReadInt(uint8_t address) 
//---------------------------------------------- 
{ 
int16_t msb, lsb; 
int16_t temp; 

     I2C_Master_Start();
     I2C_Master_Write(BMP180_ADDRESS);
     I2C_Master_Write(address);
     I2C_Master_RepeatedStart();
     I2C_Master_Write(BMP180_ADDRESS | 0x01);
     msb = (int16_t)I2C_Master_Read(1);                   // ������ � �������������
     lsb = I2C_Master_Read(0);                   // ������ ��� ������������
     I2C_Master_Stop(); 
//     temp = msb << 8;
//     temp |= lsb;
     return (msb << 8) | lsb;

} 

//********************************************* 
//  ��������� ������ 1-�� ����� � ������
//  address: I2C ����� �������  
//  �������: �����  
//---------------------------------------------- 
void BMP085WriteByte(uint8_t address, int8_t data) 
//---------------------------------------------- 
{ 
     I2C_Master_Start();
     I2C_Master_Write(BMP180_ADDRESS);    
     I2C_Master_Write(address);
     I2C_Master_Write(data);
     I2C_Master_Stop();
} 

//********************************************* 
//  ��������� ������� ������������ �����
//    
// 
//---------------------------------------------- 
void bmp085Calibration() 
//---------------------------------------------- 
{ 
   // ������ ��� ����������� 
   ac1 = bmp085ReadInt(0xAA); 
   ac2 = bmp085ReadInt(0xAC); 
   ac3 = bmp085ReadInt(0xAE); 
   ac4 = bmp085ReadInt(0xB0); 
   ac5 = bmp085ReadInt(0xB2); 
   ac6 = bmp085ReadInt(0xB4); 
   b1  = bmp085ReadInt(0xB6); 
   b2  = bmp085ReadInt(0xB8); 
   mb  = bmp085ReadInt(0xBA); 
   mc  = bmp085ReadInt(0xBC); 
   md  = bmp085ReadInt(0xBE); 



//   ac1 =   408;
//   ac2 =   -72;
//   ac3 =   -14383;
//   ac4 =   32741;
//   ac5 =   32757;
//   ac6 =   23153;
//   b1 =   6190;
//   b2 =   4;
//   mb = -32767;
//   mc = -8711;
//   md = 2868;
   
#ifdef DEBUG
    __delay_ms(200);
    printf("AC1: %d\n\r", ac1);
    printf("AC2: %d\n\r", ac2);
    printf("AC3: %d\n\r", ac3);
    printf("AC4: %u\n\r", ac4);
    printf("AC5: %u\n\r", ac5);
    printf("AC6: %u\n\r", ac6);
    printf("B1: %d\n\r", b1);
    printf("B2: %d\n\r", b2);
    printf("MB: %d\n\r", mb);
    printf("MC: %d\n\r", mc);
    printf("MD: %d\n\r", md);
#endif

} 


// ��������� ������� ���������������� ����� ����������� 
//---------------------------------------------- 
int32_t BMP085ReadUT() 
//---------------------------------------------- 
{ 
  
  // ������ 0x2E � ������ 0xF4 
  BMP085WriteByte(0xF4, 0x2E); 
  __delay_ms(5); // ����� 4,5 ��.
  // ������ 2 ����� � ������� 0xF6 � 0xF7 
  return(bmp085ReadInt(0xF6));
  //return 27898;
} 


// ��������� ������� ���������������� ����� �����
//---------------------------------------------- 
int32_t bmp085ReadUP() 
//---------------------------------------------- 
{ 
unsigned long int msb, lsb, xlsb; 
int32_t p; 
  
  // ������ 0x34+(OSS<<6) � ������ 0xF4 
  // OSS - �������� �������� �������� 
  BMP085WriteByte(0xF4, (0x34 + (OSS<<6)) ); 
  
  // ������ �� ����������� �����, �������� �� �������� �������� ��������
  switch (OSS) 
  { 
     case 0: __delay_ms(5);  break; 
     case 1: __delay_ms(8);  break; 
     case 2: __delay_ms(14); break; 
     case 3: __delay_ms(26); break; 
  }    
  
  // ������ ������ 0xF6 (MSB), 0xF7 (LSB), � 0xF8 (XLSB) 
   msb  = BMP085ReadByte(0xF6); 
   lsb  = BMP085ReadByte(0xF7); 
   xlsb = BMP085ReadByte(0xF8); 
   p = ((msb<<16) + (lsb<<8) + xlsb) >> (8-OSS); 
   return(p);
 //  return 23843;
}

// ��������� ������������ ��������������� ����������� � ����
//---------------------------------------------- 
int32_t BMP085GetTemp(int32_t _tu) 
//---------------------------------------------- 
{ 
int32_t Tt; 
//X1 = ((int32_t)_tu - (int32_t)ac6) * (int32_t)ac5 >> 15;
X1 = (((int32_t)_tu - (int32_t)ac6) * (int32_t)ac5) / 32768;
#ifdef DEBUG
    printf("X1: %d\n\r", X1);
#endif
X2 = ((int32_t)mc << 11) / (X1 + md);
#ifdef DEBUG
    printf("X2: %d\n\r", X2);
#endif
B5 = X1 + X2;
#ifdef DEBUG
    printf("B5: %d\n\r", B5);
#endif

//Tt = (B5+8) >> 4;
Tt = (B5+8) / 16;
    #ifdef DEBUG
    printf("TemperBMP: %d\n\r", Tt);
#endif

return(Tt); 
}  

 
// ��������� ������������ ����������������� ����� � �����
//---------------------------------------------- 
uint16_t BMP085GetPressure(int32_t _pu) 
//---------------------------------------------- 
{ 
int32_t Pp;

   B6 = B5 - 4000;
#ifdef DEBUG
    printf("B6: %d\n\r", B6);
#endif
  // X1 = (b2*((B6*B6)>>12))>>11;
   X1 = (b2*(B6*B6/4096))/2048;
#ifdef DEBUG
    printf("X1: %d\n\r", X1);
#endif
   //X2 = ac2 * B6 >> 11;
   X2 = ac2 * B6 / 2048;
#ifdef DEBUG
    printf("X2: %d\n\r", X2);
#endif
  X3 = X1 + X2;
#ifdef DEBUG
    printf("X3: %d\n\r", X3);
#endif
  
   B3 = ((((int32_t)ac1 * 4 + X3) << OSS) + 2) / 4;
#ifdef DEBUG
    printf("B3: %li\n\r", B3);
#endif
//   X1 = (ac3 * B6) >> 13;
   X1 = ac3 * B6 / 8192;
#ifdef DEBUG
    printf("X1: %d\n\r", X1);
#endif

//   X2 = (b1*((B6*B6) >> 12)) >> 16;
   X2 = (b1*(B6*B6 / 4096)) / 65536;
#ifdef DEBUG
    printf("X2: %d\n\r", X2);
#endif
//   X3 = ((X1 + X2)+2) >> 2;
   X3 = ((X1 + X2)+2) / 4;
#ifdef DEBUG
    printf("X3: %d\n\r", X3);
#endif
//   B4 = (ac4 * (uint32_t)(X3 + 32768)) >> 15;
   B4 = (ac4 * (uint32_t)(X3 + 32768)) / 32768;
#ifdef DEBUG
    printf("B4: %li\n\r", B4);
#endif
   B7 = ((uint32_t)_pu - B3) * (50000>>OSS);
#ifdef DEBUG
    printf("B7: %li\n\r", B7);
#endif
   
   if (B7 < 0x80000000)
      Pp = (B7*2)/B4;
   else
      Pp = (B7/B4)*2;
#ifdef DEBUG
    printf("Pp: %li\n\r", Pp);
#endif
    
    X1 = (Pp/256)*(Pp/256);//?????
#ifdef DEBUG
    printf("X1: %li\n\r", X1);
#endif
    X1 = (X1*3038)/65536;
#ifdef DEBUG
    printf("X1: %li\n\r", X1);
#endif
    X2 = -7357 * Pp / 65536;
#ifdef DEBUG
    printf("X2: %li\n\r", X2);
#endif
    Pp = Pp+((X1+X2+3791)/16);
#ifdef DEBUG
    printf("Pp: %li\n\r", Pp);
#endif
    //Pp = Pp * 0.7501;
    Pp = Pp * 100/13332;
#ifdef DEBUG
    printf("Pp: %d\n\r", Pp);
#endif

    
   return(Pp); 
} 

//  ��������� ���������� �����
//  getTemp: �� ������ ������ �������� �����������(������� ���� �� �� �� ������ �����������)
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



