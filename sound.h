
#ifndef SOUND_H
#define	SOUND_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

#define SOUND_PORT LATD
#define SOUND_TRIS TRISD
#define SOUND_PIN 7

#define SOUND_DELAY 20

void sound_init();

void TMR3_ISR();

#endif	/* XC_HEADER_TEMPLATE_H */

