#include "common.h"

extern uint8_t TxtBuf[6];           // ����� �� ����
extern uint8_t Dis_Buff[BUF_SIZE];  // ����� �������
extern uint8_t key_event;           // ���� ������
extern uint8_t text_buf[100];       // ����� ��� �� ������
uint8_t events = MAIN_EVENT;        // 
uint8_t delay_digit = 0;            // ��� ����� ������� ����
bit show_digit;                     // �� ���������� �����, � ���. �������
bit en_put;                         // �� ����� ������ � ����� �������

// ������ � DS3231 ������, �������, ������� �� ����
// 
void GetTime(void)
{
    getTime(&TTime.Thr, &TTime.Tmin, &TTime.Ts);
    getDate(&TTime.Tdy,&TTime.Tdt,&TTime.Tmt,&TTime.Tyr);
}

//****************************************
// ������������ ��������� -  �������
//****************************************
void time_set_min(void)
{
    show_digit = ~show_digit;
   switch (events)
   {
        case MAIN_EVENT:

            break;
        case KEY_OK_EVENT:                      // ���� ��������� ��
            //setTime(TSTime.Thr,TSTime.Tmin,0);  // �������� ��� � ��������
            RTOS_DeleteTask(time_set_min);      // ��������� ������
            RTOS_SetTask(time_set_hr, 0, 50);   // ������� ������, ������������ �����
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            break;
        case KEY_EXIT_EVENT:                    // ������ EXIT
            RTOS_DeleteTask(time_set_min);      // ���������� � �������� �����
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            break;
        case KEY_UP_EVENT:
            TSTime.Tmin++;
            if(TSTime.Tmin > 59) TSTime.Tmin = 0;
            RTOS_SetTask(default_state, 2000, 0);  // 5 ������ ��� ������
            events = MAIN_EVENT;
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tmin--;
            if(TSTime.Tmin == 255) TSTime.Tmin = 59;            
            RTOS_SetTask(default_state, 2000, 0);  // 5 ������ ��� ������
            events = MAIN_EVENT;
            break;            
   }   
   
       if(show_digit)      // �� ���������� �����
    {
        putchar_b_buf(13,(TSTime.Tmin/10) % 10 + 48);
        putchar_b_buf(19,TSTime.Tmin % 10 + 48);
    }
    else 
    {
        putchar_b_buf(13,0);
        putchar_b_buf(19,0);
    }
   Update_Matrix(Dis_Buff);          // �������� ��� �� ������
   en_put = 1;
}

//****************************************
// ������������ ��������� -  �����
//****************************************
void time_set_hr(void)
{
   show_digit = ~show_digit;
   switch (events)
   {
        case MAIN_EVENT:

            break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(time_set_hr);
            RTOS_SetTask(time_set_yr, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            //RTOS_DeleteTask(default_state);
            events = MAIN_EVENT;
            clear_matrix();
            en_put = 0;     // ���������� ��������
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(time_set_hr);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            break;
        case KEY_UP_EVENT:
            TSTime.Thr++;
            if(TSTime.Thr > 23) TSTime.Thr = 0;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setTime(TSTime.Thr,TSTime.Tmin,0);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Thr--;
            if(TSTime.Thr == 255) TSTime.Thr = 23;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setTime(TSTime.Thr,TSTime.Tmin,0);
            break;            
   }   
   if(en_put)
   {
        if(show_digit)      // �� ���������� �����
        {

            putchar_b_buf(13,(TSTime.Tmin/10) % 10 + 48);
            putchar_b_buf(19,TSTime.Tmin % 10 + 48);

            putchar_b_buf(0,(TSTime.Thr/10) % 10 + 48);
            putchar_b_buf(6,TSTime.Thr % 10 + 48);
        }
        else 
        {

            putchar_b_buf(0,0);
            putchar_b_buf(6,0);
        }
   }
    Update_Matrix(Dis_Buff);          // �������� ��� �� ������
    en_put = 1;                         // ��������� �������� �������
}
//***************************************************************
//  ������������ ����
//***************************************************************
void time_set_yr(void)
{
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(time_set_yr);
            RTOS_SetTask(time_set_mt, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            en_put=0;
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(time_set_yr);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            break;
        case KEY_UP_EVENT:
            TSTime.Tyr++;
            if(TSTime.Tyr > 99) TSTime.Tyr = 0;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tyr--;
            if(TSTime.Tyr == 255) TSTime.Tyr = 99;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,'�');
    putchar_b_buf(6,'�');
    putchar_b_buf(12,':');
    putchar_b_buf(18,(TSTime.Tyr/10) % 10 + 48);
    putchar_b_buf(24,TSTime.Tyr % 10 + 48);
}    
       Update_Matrix(Dis_Buff);          // �������� ��� �� ������
       en_put=1;
}

//***************************************************************
//  ������������ �����
//***************************************************************
void time_set_mt(void)
{
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(time_set_mt);
            RTOS_SetTask(time_set_dt, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            en_put=1;
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(time_set_mt);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            break;
        case KEY_UP_EVENT:
            TSTime.Tmt++;
            if(TSTime.Tmt > 12) TSTime.Tmt = 1;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tmt--;
            if(TSTime.Tmt == 0) TSTime.Tmt = 12;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,'�');
    putchar_b_buf(6,'�');
    putchar_b_buf(12,':');
    switch(TSTime.Tmt)
    {
        case 1:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 2:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 3:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 4:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 5:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 6:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;        
        case 7:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 8:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;        
        case 9:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 10:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;        
        case 11:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 12:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;        

    }
}    
       Update_Matrix(Dis_Buff);          // �������� ��� �� ������
       en_put=1;
}

//***************************************************************
//  ������������ �����
//***************************************************************
void time_set_dt(void)
{
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(time_set_dt);
            RTOS_SetTask(time_set_dy, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            en_put=1;
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(time_set_dt);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            break;
        case KEY_UP_EVENT:
            TSTime.Tdt++;
            if(TSTime.Tdt > 31) TSTime.Tdt = 1;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tdt--;
            if(TSTime.Tdt == 0) TSTime.Tdt = 31;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,'�');
    putchar_b_buf(6,'�');
    putchar_b_buf(12,':');
    putchar_b_buf(18,(TSTime.Tdt/10) % 10 + 48);
    putchar_b_buf(24,TSTime.Tdt % 10 + 48);
}    
       Update_Matrix(Dis_Buff);          // �������� ��� �� ������
       en_put=1;
}


//***************************************************************
//  ������������ �����
//***************************************************************
void time_set_dy(void)
{
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(time_set_dy);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state);
            events = MAIN_EVENT;
            en_put=1;
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(time_set_dy);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            break;
        case KEY_UP_EVENT:
            TSTime.Tdy++;
            if(TSTime.Tdy > 7) TSTime.Tdy = 1;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tdy--;
            if(TSTime.Tdy == 0) TSTime.Tdy = 7;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,'�');
    putchar_b_buf(6,'�');
    putchar_b_buf(12,':');
    switch(TSTime.Tdy)
    {
        case 2:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 3:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 4:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 5:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 6:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
        case 7:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;        
        case 1:
        putchar_b_buf(18,'�');
        putchar_b_buf(24,'�');
        break;
    }
}    
       Update_Matrix(Dis_Buff);          // �������� ��� �� ������
       en_put=1;
}

//==================================================
//  �������� ������ �� ������� -:)
//==================================================
void time_led()
{
    

   switch (events)
   {
        case MAIN_EVENT:
            FillBuf();
//            Update_Matrix(Dis_Buff);          // �������� ��� �� ������
            break;
        case  KEY_OK_EVENT:
//                putchar_down(0,0);
//                putchar_down(6,0);
//                putchar_down(13,0);
//                putchar_down(19,0);
//                putchar_down_s(25,0);
//                putchar_down_s(29,0);
//                putchar_down(0, TTimeConv.hr_10);  
//                putchar_down(6, TTimeConv.hr_1);     
//                putchar_down(13, TTimeConv.min_10);
//                putchar_down(19, TTimeConv.min_1);
//                putchar_down_s(25, TTimeConv.s_10);
//                putchar_down_s(29, TTimeConv.s_1);
            RTOS_DeleteTask(time_led);
            RTOS_SetTask(time_set_min, 0, 50);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            TSTime = TTime;
            events = MAIN_EVENT;
            break;
        case  KEY_UP_EVENT:
            clear_matrix();
            pic_to_led(0,1);
            pic_to_led(8,2);
            pic_to_led(16,3);
            break;
        case  KEY_DOWN_EVENT:
            ow_reset();
            break;
   }           
            Update_Matrix(Dis_Buff);          // �������� ��� �� ������
               

}


//  ���� ����
void version(void)
{
    uint8_t i;
    
    sprintf(text_buf, VERSION);  
    while(scroll_text())
    {
        Update_Matrix(Dis_Buff);          // �������� ��� �� ������
        for(i=0; i<SPEED_STRING; i++)
            __delay_ms(1);

    };
}

void default_state(void)
{
    events = KEY_EXIT_EVENT;
}

// ������� ����������� �� ����� RB0
 void INT0_ISR(void)
{
        RTOS_SetTask(GetTime, 0, 0); // ������ ����������� ������ ������ � ���������
                                        // ���� 500��.

}
 
 
 // ������� ����������� ������1

 void TMR1_ISR(void)
{
//     delay_digit++;
//     if(delay_digit > 30)
//     {
//         delay_digit = 0;
//         show_digit = ~show_digit;
//     }
     
     //Update_Matrix(Dis_Buff);          // �������� ��� �� ������
//        RTOS_SetTask(GetTime, 0, 0); // ������ ����������� ������ ������ � ���������
                                        // ���� 500��.
}

