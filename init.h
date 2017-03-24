
#ifndef INIT_H
#define	INIT_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "i2c.h"
#include "interrupt.h" 
#include "common.h"
#include "timer.h"
#include "dispatcher.h"
#include "eusart.h"
#include "bmp180.h"
#include "eeprom.h"

void SYSTEM_Initialize(void);  // ����������� ����������
void Port_Init(void);  // ����������� �����
void Interrupt_Init(void); // ��� �����������



#endif	/* XC_HEADER_TEMPLATE_H */

