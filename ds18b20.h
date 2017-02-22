#ifndef DS18B20_H
#define	DS18B20_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "common.h"
#include "onewire.h"


const char fract[] = {0,1,1,2,2,3,4,4,5,6,6,7,7,8,9,9}; // дискретність, знак після коми
                                                        //  0/16..., 1/16..., 2/16......

#endif	/* XC_HEADER_TEMPLATE_H */

