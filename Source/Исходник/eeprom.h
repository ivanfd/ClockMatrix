#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h> // include processor files - each processor file is guarded.  


void write_eep( unsigned char address, unsigned char data );
unsigned char read_eep( unsigned short address );


#endif	/* XC_HEADER_TEMPLATE_H */

