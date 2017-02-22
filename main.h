 
#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
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


#define VERSION "Clock v1.0 by Ivan_fd"


#endif	/* XC_HEADER_TEMPLATE_H */

