
#ifndef I2C_H
#define	I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "common.h"

void I2C_Init(void);                    // ініціалізація I2C
uint8_t I2C_start (uint8_t adress, uint8_t _R_W); //старт
void I2C_idle (void);                   // чекати закінчення активності
uint8_t I2C_write (uint8_t data);       // запис байту
uint8_t I2C_read_noack (void);          //читати байт
uint8_t I2C_restart (uint8_t adres, uint8_t _R_W);
void I2C_stop (void);                   // формувати стоп біт
uint8_t i2c_read_ack (void);

#endif	/* XC_HEADER_TEMPLATE_H */

