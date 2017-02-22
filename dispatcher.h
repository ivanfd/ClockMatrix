#ifndef DISPATCHER_H
#define	DISPATCHER_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h>
#include "interrupt.h"
#include "common.h"

#define MAX_TASKS      15               // ʳ������ �����
//#define TIMER_START    100             // ��������� ��� ������� �0 1ms
#define TIMER_START    61             // ��������� ��� ������� �0 5ms

typedef struct task
{ 
   void (*pFunc) (void); // �������� �� �������
   uint16_t delay; // �������� ����� ������ �������� �������
   uint16_t period; // ����� �������
   uint16_t run; // �� ��������� �� ������� �������
}task;

/******************************************************************************************
 * ��������� �������
 */
void TMR0_ISR(void);
void RTOS_Init (void);
void RTOS_SetTask (void (*taskFunc)(void), uint16_t taskDelay, uint16_t taskPeriod);
void RTOS_DeleteTask (void (*taskFunc)(void));
void RTOS_DispatchTask (void);

 
#endif	/* XC_HEADER_TEMPLATE_H */

