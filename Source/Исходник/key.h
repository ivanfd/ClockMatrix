
#ifndef KEY_H
#define	KEY_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "dispatcher.h"
#include "events.h" 

#define DELAYKEY 40     // debounce

#define KEY_PORT    PORTB
//  входи до яких підключені кнопки
#define KEY_OK 1
#define KEY_UP 2
#define KEY_DOWN 3
#define KEY_EXIT 4




void key_press(void);

#endif	/* XC_HEADER_TEMPLATE_H */

