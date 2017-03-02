/* Проект годинника на матричних індикаторах.
 * На драйверах MAX7221
 * Процесор: PIC18F4525
 * Тактова частота 40Мгц
 * Контролер годинника: DS3231 
 * Author: Ivan_fd
 * Version: 1.0
 */


#include "main.h"

extern uint8_t TxtBuf[6];          // буфер дл¤ цифр
extern uint8_t Dis_Buff[BUF_SIZE]; // буфер дисплея




void main(void) {
    __delay_ms(10);
    SYSTEM_Initialize(); // ініціалізація контролера

    clear_matrix();
   // version();
    
    INTERRUPT_GlobalInterruptDisable();
    RTOS_SetTask(time_led, 100, cycle_main); // додаємо задачу, запуск кожних 100мс, затримка перед запуском 500ms
    RTOS_SetTask(key_press,0,1);    // опитування кнопок кожні 5 мс.
    INTERRUPT_GlobalInterruptEnable();
    
     while (1)
    {
        RTOS_DispatchTask();

    }
    return;
}



