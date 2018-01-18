//*********************************************************************************************************
//                                            ��������� �����.
// ���������� � �����: 
// http://chipenable.ru/index.php/embedded-programming/item/219-planirovschik-dlya-mikrokontrollera.html
// ��������� �� AVR. ���� ����������� ��� PIC, ���� ���� ����. 
// 
//*********************************************************************************************************

#include "dispatcher.h"

volatile static task TaskArray[MAX_TASKS];      // ������
volatile static uint8_t arrayTail;                  // "����" ����� �����
uint8_t Timer0Interrupt;        // �� ���� ����������� �� ������� 0

/******************************************************************************************
 * ���
 */
 void RTOS_Init()
{
   T0CON = 0b11000111;                         // ������ 1:256, ������ ���. 8-��
   INTCONbits.TMR0IE = 1;                      // ����������� �� ������������ �������
   TMR0 = TIMER_START;                         // ��������� �������� � ������ �������

   arrayTail = 0;                               // "����" � 0
}
 
/******************************************************************************************
 * ������ ������ � ������
 */
void RTOS_SetTask (void (*taskFunc)(void), uint16_t taskDelay, uint16_t taskPeriod)
{
   uint8_t i;
   
   if(!taskFunc) return;
   for(i = 0; i < arrayTail; i++)                     // ����� ������ � ������
   {
      if(TaskArray[i].pFunc == taskFunc)              // ���� ���� ��� �, �� ���������� ��
      {
         INTERRUPT_GlobalInterruptDisable();

         TaskArray[i].delay  = taskDelay;
         TaskArray[i].period = taskPeriod;
         TaskArray[i].run    = 0;   

         INTERRUPT_GlobalInterruptEnable();
           return;                                      // ���������, ��������
      }
   }

   if (arrayTail < MAX_TASKS)                         // ���� ���� ������ ���� 
   {                                                  // � � ����, �� �������� ��
         INTERRUPT_GlobalInterruptDisable();
      
      TaskArray[arrayTail].pFunc  = taskFunc;
      TaskArray[arrayTail].delay  = taskDelay;
      TaskArray[arrayTail].period = taskPeriod;
      TaskArray[arrayTail].run    = 0;   

      arrayTail++;                                    // �������� ����
         INTERRUPT_GlobalInterruptEnable();
   } 
} 

/******************************************************************************************
 * �������� ������ � ������
 */
void RTOS_DeleteTask (void (*taskFunc)(void))
{
   uint8_t i;
   
   for (i=0; i<arrayTail; i++)                        // ��������� �� ������ �����
   {
      if(TaskArray[i].pFunc == taskFunc)              // ���� ������� ������
      {
         
                  INTERRUPT_GlobalInterruptDisable();
         if(i != (arrayTail - 1))                     // ���������� ������� ������
         {                                            // �� ���� ��������
            TaskArray[i] = TaskArray[arrayTail - 1];
         }
         arrayTail--;                                 // �������� ����
                  INTERRUPT_GlobalInterruptEnable();
         return;
      }
   }
}

/******************************************************************************************
 * ��������� ����
 */

void RTOS_DispatchTask()
{
   uint8_t i, j;
   void (*function) (void);
   
   if (Timer0Interrupt)
   {
       Timer0Interrupt = 0;
       for (j=0; j<arrayTail; j++)                        // ��������� �� ������ �����
        {
            if  (TaskArray[j].delay == 0)                   // ���� ��� �� ��������� ������
                TaskArray[j].run = 1;                      // ������� �� �������
            else TaskArray[j].delay--;                      // ���� �, �� �������� ����
        }
   }

   for (i=0; i<arrayTail; i++)                        // ��������� �� ������ �����
   {
      if (TaskArray[i].run == 1)                      // ���� �� �� ��������� ������ = 1
      {                                               // �����'������� ������, �.�� ��
         function = TaskArray[i].pFunc;               // ��������� ���� 
                                                      // �������� ������
         if(TaskArray[i].period == 0)                 
         {                                            // ���� ����� ����� ����
            RTOS_DeleteTask(TaskArray[i].pFunc);      // ��������� ������ � ������
            
         }
         else
         {
            TaskArray[i].run = 0;                     // ���� �, �� ������ �� �������
            if(!TaskArray[i].delay)                   // ���� ������ �� ������ ��������
            {                                         // ������ ��
               TaskArray[i].delay = TaskArray[i].period-1; 
            }                                         //   
         }
         (*function)();                               // �������� ������
      
      }
      
   }
}

// ����������� �� ������� 0
 void TMR0_ISR(void)
{

    // Clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;
    TMR0 = TIMER_START;
    Timer0Interrupt = 1; // ������� ������� ������������
   
   

    }