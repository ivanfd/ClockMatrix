 
#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdio.h>
#include "max7221.h"
#include "init.h"
#include "interrupt.h"
#include "dispatcher.h"
#include "display.h"
#include "common.h"
#include "ds3231.h"
#include "key.h"
#include "events.h"
#include "onewire.h"

#define _XTAL_FREQ  40000000 // ������ ���������� 40���


#define VERSION "Clock v3.0.1 by Ivan_fd"
//#define SHOW_VERSION
//#define DEBUG   // ��� �������, ��������������.


#endif	/* XC_HEADER_TEMPLATE_H */

