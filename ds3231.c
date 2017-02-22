#include "ds3231.h"                                                       
                                                                 
                                                         
uint8_t bcd_to_decimal(uint8_t d)               
{                                                                                         
         return ((d & 0x0F) + (((d & 0xF0) >> 4) * 10));
}                               
                                                             

uint8_t decimal_to_bcd(uint8_t d)
{
         return (((d / 10) << 4) & 0xF0) | ((d % 10) & 0x0F);
}                                                   
                                       
                 
uint8_t DS3231_Read(uint8_t address)
{                                     
         uint8_t value = 0;
         I2C_start(DS3231_Address,0);
         I2C_write(address);           
         I2C_restart(DS3231_Address,1);
         value = I2C_read_noack();
         I2C_stop();                
         return value;
}                     


void DS3231_Write(uint8_t address, uint8_t value)   
{ 
         I2C_start(DS3231_Address,0);
         I2C_write(address);
         I2C_write(value);   
         I2C_stop();
} 


void DS3231_init()
{                                   
         DS3231_Write(controlREG, 0x00); 
         DS3231_Write(statusREG, 0x08);
}                       
//****************************************************
// прочитати дан≥ з годинника
// p3 - години, p2 - хвилини, p1 - секунди
//****************************************************
void getTime(uint8_t *p3, uint8_t *p2, uint8_t *p1)
{                       
         *p1 = DS3231_Read(secondREG);
         *p1 = bcd_to_decimal(*p1);
         *p2 = DS3231_Read(minuteREG);
         *p2 = bcd_to_decimal(*p2);
         *p3 = (0x3F & DS3231_Read(hourREG));           
         *p3 = bcd_to_decimal(*p3);   
}                                 
                                                     
                               
// прочитати дан≥ з годинника
// p4 - день, p3 - число, p2 - м≥с€ць, p1 - р≥к
void getDate(uint8_t *p4, uint8_t *p3, uint8_t *p2, uint8_t *p1)
{
         *p1 = DS3231_Read(yearREG);
         *p1 = bcd_to_decimal(*p1);                 
         *p2 = (0x1F & DS3231_Read(monthREG));
         *p2 = bcd_to_decimal(*p2);                               
         *p3 = (0x3F & DS3231_Read(dateREG));
         *p3 = bcd_to_decimal(*p3);   
         *p4 = (0x07 & DS3231_Read(dayREG));   
         *p4 = bcd_to_decimal(*p4);                   
}
                                                   
void setTime(uint8_t hSet, uint8_t mSet, uint8_t sSet) 
{                                                                                                             
         DS3231_Write(secondREG, (decimal_to_bcd(sSet)));
         DS3231_Write(minuteREG, (decimal_to_bcd(mSet)));       
         DS3231_Write(hourREG, (0x3F & (decimal_to_bcd(hSet))));
}                                                 

                                   
void setDate(uint8_t daySet, uint8_t dateSet, uint8_t monthSet, uint8_t yearSet)
{         
         DS3231_Write(dayREG, (decimal_to_bcd(daySet)));           
         DS3231_Write(dateREG, (decimal_to_bcd(dateSet))); 
         DS3231_Write(monthREG, (decimal_to_bcd(monthSet)));
         DS3231_Write(yearREG, (decimal_to_bcd(yearSet)));   
}

                               
