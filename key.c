#include "key.h"
uint8_t key_event = NULL;
extern uint8_t events;

void key_press(void)
{
    static uint8_t count = 0;
    uint8_t key;
    
    if ((KEY_PORT & (1<<KEY_OK))==0)
        key = KEY_OK_EVENT;
    else if ((KEY_PORT & (1<<KEY_UP))==0)
        key = KEY_UP_EVENT;
    else if ((KEY_PORT & (1<<KEY_DOWN))==0)
        key = KEY_DOWN_EVENT;
    else if ((KEY_PORT & (1<<KEY_EXIT))==0)
        key = KEY_EXIT_EVENT;
    else
        key = NULL;

    
    if(key){
        if(count <= DELAYKEY)
            {
                count++;
                if(count == DELAYKEY)
                {
                    //key_event = key;
                      events = key;
                      count = 0;
                }
            };
    }
    else
    {
        count = 0;
    //    key_event = 0;
    }
}
