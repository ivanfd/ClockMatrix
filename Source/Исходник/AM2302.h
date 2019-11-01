
#ifndef AM2302_H
#define	AM2302_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "main.h"

#define DHT_IO   PORTEbits.RE0   // вибираємо вивід для підключення датчика
#define DHT_IO_DIR TRISEbits.RE0 // направлення порта


uint8_t DHT_GetByte();
uint8_t DHT_GetData(uint8_t *pVal);

#endif	/* XC_HEADER_TEMPLATE_H */

