#ifndef _DS3231H

#define _DS3231H

#include <stdint.h>
#include "common.h"
#include "i2c.h"

#define DS3231_AddressW              0xD0
#define DS3231_AddressR              0xD1
                                                 
                                           
#define secondREG                   0x00
#define minuteREG                   0x01
#define hourREG                     0x02
#define dayREG                      0x03
#define dateREG                     0x04                             
#define monthREG                    0x05                           
#define yearREG                     0x06                   
#define alarm1secREG                0x07       
#define alarm1minREG                0x08
#define alarm1hrREG                 0x09           
#define alarm1dateREG               0x0A 
#define alarm2minREG                0x0B   
#define alarm2hrREG                 0x0C
#define alarm2dateREG               0x0D
#define controlREG                  0x0E
#define statusREG                   0x0F
#define ageoffsetREG                0x10
#define tempMSBREG                  0x11
#define tempLSBREG                  0x12
                                 
#define _24_hour_format             0
#define _12_hour_format             1
#define am                          0
#define pm                          1

                                                               
uint8_t bcd_to_decimal(uint8_t d);
uint8_t decimal_to_bcd(uint8_t d);

// ������� ��� ������� ����� � ���������
// address -  ������ I2C ��������
uint8_t DS3231_Read(uint8_t address);

// ������� ��� ������ ����� � ��������
// address -  ������ I2C ��������
// value - ��� ��� ������ 
void DS3231_Write(uint8_t address, uint8_t value);

//  �����������
void DS3231_init(); 

// ��������� ��� � ���������
// p3 - ������, p2 - �������, p1 - �������
void getTime(uint8_t *p3, uint8_t *p2, uint8_t *p1);

void getDate(uint8_t *p4, uint8_t *p3, uint8_t *p2, uint8_t *p1);   

void setTime(uint8_t hSet, uint8_t mSet, uint8_t sSet);
void setDate(uint8_t daySet, uint8_t dateSet, uint8_t monthSet, uint8_t yearSet);   
//float getTemp();

#endif
