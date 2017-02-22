#ifndef ONEWIRE_H
#define	ONEWIRE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "common.h"


#define TRIS_DQ TRISCbits.RC0
#define DQ PORTCbits.RC0

uint8_t ow_reset(void);
uint8_t read_bit(void);
void write_bit(uint8_t bitval);

#endif	/* XC_HEADER_TEMPLATE_H */

