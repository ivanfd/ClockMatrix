/* ������ ��������� �� ��������� �����������.
 * �� ��������� MAX7221
 * ��������: PIC18F4525
 * ������� ������� 40���
 * ��������� ���������: DS3231 
 * Author: Ivan_fd
 * Version: 1.0
 */


#include "main.h"

extern uint8_t TxtBuf[6];          // ����� �� ����
extern uint8_t Dis_Buff[BUF_SIZE]; // ����� �������

//int8_t tst1 = -125, tst2 = 93;
//int16_t tstr;

void main(void) {
    __delay_ms(10);

    SYSTEM_Initialize(); // ������������ ����������
//    tstr = (tst1 << 8) | tst2;
//    printf("AC1: %d\n\r", tstr);
//    printf("AC1: %d\n\r", tst1);
//    printf("AC1: %d\n\r", tst2);
    clear_matrix();
   // version();
    
    INTERRUPT_GlobalInterruptDisable();
    RTOS_SetTask(time_led, 100, cycle_main); // ������ ������, ������ ������ 100��, �������� ����� �������� 500ms
    RTOS_SetTask(key_press,0,1);    // ���������� ������ ����� 5 ��.
    INTERRUPT_GlobalInterruptEnable();
    
     while (1)
    {
        RTOS_DispatchTask();

    }
    return;
}



