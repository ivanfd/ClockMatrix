
#ifndef SETTINGS_H
#define	SETTINGS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "common.h"
#include "display.h"
#include "eeprom.h"
#include "strings.h"
#include "dispatcher.h"


void time_set_min(void);
void time_set_hr(void);
void time_set_yr(void);
void time_set_mt(void);
void time_set_dt(void);
void time_set_dy(void);
void set_font_set(void);
void set_type_clk(void);
void brg_set();
void set_brg_manual();
void set_sound_h(void);
void set_en_ds1(void);
void default_state(void);
void set_en_ds2(void);
void set_en_bmp(void);
void set_en_dst(void);


#endif	/* XC_HEADER_TEMPLATE_H */

