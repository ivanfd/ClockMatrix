/* Проект годинника на матричних індикаторах.
 * На драйверах MAX7221
 * Процесор: PIC18F4525
 * Тактова частота 40Мгц
 * Контролер годинника: DS3231 
 * Author: Ivan_fd
 * Version: 3.0.1
 */

#include "main.h"

//extern uint8_t TxtBuf[6];          // буфер дл¤ цифр
extern uint8_t Dis_Buff[BUF_SIZE]; // буфер дисплея
extern uint8_t blk_dot; // дозвіл на мигання кнопок
//int8_t tst1 = -125, tst2 = 93;
//int16_t tstr;

void main(void) {

    __delay_ms(10);
    
    SYSTEM_Initialize(); // ініціалізація контролера
//    tstr = (tst1 << 8) | tst2;
//    printf("AC1: %d\n\r", tstr);
//    printf("AC1: %d\n\r", tst1);
//    printf("AC1: %d\n\r", tst2);
    clear_matrix();
    blk_dot = 0;
    version();
    blk_dot = 1;
    
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



