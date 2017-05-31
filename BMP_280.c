#include "BMP_280.h"

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
int16_t dig_H1;

//  ������������ �������
void bmp280_Init()
{

 dig_T1 = bmp280ReadInt16(0x88);
 dig_T2 = bmp280ReadInt16(0x8A);
 dig_T3 = bmp280ReadInt16(0x8C);
 dig_P1 = bmp280ReadInt16(0x8E);//dig_P1
 dig_P2 = bmp280ReadInt16(0x90);//dig_P2
 dig_P3 = bmp280ReadInt16(0x92);//dig_P3
 dig_P4 = bmp280ReadInt16(0x94);//dig_P4
 dig_P5 = bmp280ReadInt16(0x96);//dig_P5
 dig_P6 = bmp280ReadInt16(0x98);//dig_P6
 dig_P7 = bmp280ReadInt16(0x9A);//dig_P7
 dig_P8 = bmp280ReadInt16(0x9C);//dig_P8
 dig_P9 = bmp280ReadInt16(0x9E);//dig_P9  
// dig_H1 = bmp280ReadInt16(0xA1);
         
bmp280WriteByte(BMP280_REG_CONFIG,(BMP280_TSB_2000 | BMP280_FILTER_COEFFICIENT8 | BMP280_SPI_OFF)); 
bmp280WriteByte(BMP280_REG_CONTROL,(BMP280_OVERSAMPLING_T16 | BMP280_OVERSAMPLING_P16 | BMP280_MODE_NORMAL)); 

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
    printf("dig_P5: %d\n\r", dig_P7);
    printf("dig_P8: %d\n\r", dig_P8);
    printf("dig_P9: %d\n\r", dig_P9);
#endif
  
}


uint16_t bmp280ReadInt16(uint8_t address)
{
    uint16_t msb=0;
	uint16_t lsb=0;
   
    I2C_Master_Start();
    I2C_Master_Write(BMP280_ADDRESS);
    I2C_Master_Write(address);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(BMP280_ADDRESS | 0x01);
    lsb = I2C_Master_Read(1);                   // ������ � �������������
    msb = I2C_Master_Read(0);                   // ������ ��� ������������
    I2C_Master_Stop(); 
//     temp = msb << 8;
//     temp |= lsb;
     return (msb << 8) | lsb;
    
    
}

uint32_t bmp280ReadInt32(uint8_t address)
{
	uint32_t msb=0;
	uint32_t lsb=0;
	uint32_t xlsb=0;
   
    I2C_Master_Start();
    I2C_Master_Write(BMP280_ADDRESS);
    I2C_Master_Write(address);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(BMP280_ADDRESS | 0x01);
    msb = (uint32_t)I2C_Master_Read(1);                   // ������ � �������������
    lsb = (uint32_t)I2C_Master_Read(1);                   // ������ ��� ������������
    xlsb = (uint32_t)I2C_Master_Read(0);
    I2C_Master_Stop(); 
//     temp = msb << 8;
//     temp |= lsb;
     return ((msb<<16) + (lsb<<8) + xlsb) >> 4;
    
    
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
//  ��������� ������� 1-�� ����� � �������
//  address: I2C ����� �������  
//  ������� 1 ���� ��������� �����  
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
     data = I2C_Master_Read(0);                   // ������ ��� ������������
     I2C_Master_Stop();
     return(data); 
} 

//===========================================
//   ������ �����������
//===========================================
int32_t bmp280ReadTemp()
{
	int32_t temperature = 0;
 
   
   temperature = bmp280ReadInt32(BMP280_REG_RESULT_TEMPRERATURE);
   
   #ifdef DEBUG
    __delay_ms(200);
    
    printf("UncTemp: %ld\n\r", temperature);


#endif
            
            
	//		temperature = bmp280ReadLong(BMP280_REG_RESULT_TEMPRERATURE);
	return temperature;
}


//===========================================
//   ������ ����
//===========================================
int32_t bmp280ReadPressure()
{
	int32_t pressure = 0;
 
   
   pressure = bmp280ReadInt32(BMP280_REG_RESULT_PRESSURE);
   
   #ifdef DEBUG
    __delay_ms(200);
    
    printf("UncPress: %ld\n\r", pressure);


#endif
            
            
	return pressure;
}


void bmp280Convert(int32_t *temperature, int32_t *pressure)
{
    int32_t adc_T;
	int32_t adc_P;
  //  int32_t temperature1;
  //  int32_t pressure1;
    
    double var1, var2, T, P, tfine;
    
    
    adc_T = bmp280ReadTemp();
    adc_P = bmp280ReadPressure();
    
    var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
	var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
	tfine = (var1 + var2);
	*temperature = (int32_t) (tfine*10 / 5120.0);
    
    var1 = ((double)tfine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);

	P = 1048576.0 - (double)adc_P;
	P = (P - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dig_P9) * P * P / 2147483648.0;
	var2 = P * ((double)dig_P8) / 32768.0;
	P = (P + (var1 + var2 + ((double)dig_P7)) / 16.0);

	*pressure = P;
    *pressure = *pressure  * 100/13332;    
       #ifdef DEBUG
    __delay_ms(200);
    
    printf("Temp280: %d\n\r", *temperature);
    printf("Press280: %u\n\r", *pressure);
#endif
    
}