#include "common.h"

extern uint8_t TxtBuf[6];           // ����� �� ����
extern uint8_t Dis_Buff[BUF_SIZE];  // ����� �������
extern uint8_t key_event;           // ���� ������
extern uint8_t text_buf[100];       // ����� ��� �� ������
uint8_t events = MAIN_EVENT;        // 
uint8_t delay_digit = 0;            // ��� ����� ������� ����
bit show_digit;                     // �� ���������� �����, � ���. �������
bit en_put;                         // �� ����� ������ � ����� �������
uint16_t temperature;               // ����������� � ��������� �������
uint8_t timer_val = 0, time_flag = 0;  // ��� ������������� �����������
extern uint8_t (*pFont)[][5];
uint8_t type_font = 0;                  // ����� �����
uint16_t press;                      // ����������� ����
extern uint8_t play_sound; //  �� ����� ����������
__EEPROM_DATA(3, 0, 0, 0, 0, 0, 0, 0); // ����������� ������, 
                                       // 0 - ��� ������ (�� 1 �� 5)

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
            setTime(TSTime.Thr,TSTime.Tmin,0);
            events = MAIN_EVENT;
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tmin--;
            if(TSTime.Tmin == 255) TSTime.Tmin = 59;            
            RTOS_SetTask(default_state, 2000, 0);  // 5 ������ ��� ������
            setTime(TSTime.Thr,TSTime.Tmin,0);
            events = MAIN_EVENT;
            break;            
   }   
   
       if(show_digit)      // �� ���������� �����
    {
        putchar_b_buf(13,(TSTime.Tmin/10) % 10 , pFont);
        putchar_b_buf(19,TSTime.Tmin % 10 , pFont);
    }
    else 
    {
        putchar_b_buf(13,0, &Font);
        putchar_b_buf(19,0, &Font);
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

            putchar_b_buf(13,(TSTime.Tmin/10) % 10, pFont);
            putchar_b_buf(19,TSTime.Tmin % 10, pFont);

            if((TSTime.Thr/10) % 10)
            putchar_b_buf(0,(TSTime.Thr/10) % 10, pFont);
            else
            putchar_b_buf(0,0, pFont);
            putchar_b_buf(6,TSTime.Thr % 10, pFont);
        }
        else 
        {

            putchar_b_buf(0,0, &Font);
            putchar_b_buf(6,0, &Font);
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
    putchar_b_buf(0,'�', &Font);
    putchar_b_buf(6,'�', &Font);
    putchar_b_buf(12,':', &Font);
    putchar_b_buf(18,(TSTime.Tyr/10) % 10, pFont);
    putchar_b_buf(24,TSTime.Tyr % 10, pFont);
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
    putchar_b_buf(0,'�', &Font);
    putchar_b_buf(6,'�', &Font);
    putchar_b_buf(12,':', &Font);
    switch(TSTime.Tmt)
    {
        case 1:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 2:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 3:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 4:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 5:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 6:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;        
        case 7:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 8:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;        
        case 9:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 10:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;        
        case 11:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 12:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
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
    putchar_b_buf(0,'�', &Font);
    putchar_b_buf(6,'�', &Font);
    putchar_b_buf(12,':', &Font);
    putchar_b_buf(18,(TSTime.Tdt/10) % 10, pFont);
    putchar_b_buf(24,TSTime.Tdt % 10 , pFont);
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
            RTOS_SetTask(set_font_set, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
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
    putchar_b_buf(0,'�', &Font);
    putchar_b_buf(6,'�', &Font);
    putchar_b_buf(12,':', &Font);
    switch(TSTime.Tdy)
    {
        case 2:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 3:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 4:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 5:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 6:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
        case 7:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;        
        case 1:
        putchar_b_buf(18,'�', &Font);
        putchar_b_buf(24,'�', &Font);
        break;
    }
}    
       Update_Matrix(Dis_Buff);          // �������� ��� �� ������
       en_put=1;
}

//***************************************************************
//  ������������ ����
//***************************************************************
void set_font_set(void)
{
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_font_set);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state);     
            events = MAIN_EVENT;
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_font_set);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            break;
        case KEY_UP_EVENT:
            type_font++;
            if(type_font > 5) type_font = 1;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_FONT,type_font);
            set_font();
            break;
        case KEY_DOWN_EVENT:
            type_font--;
            if(type_font == 0) type_font = 5;           
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_FONT,type_font);
            set_font();
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,'�', &Font);
    putchar_b_buf(6,'�', &Font);
    putchar_b_buf(12,':', &Font);
    putchar_b_buf(18,type_font % 10, pFont);
    putchar_b_buf(24,0, &Font);
}    
       Update_Matrix(Dis_Buff);          // �������� ��� �� ������
       en_put=1;
}


//**************************************************
//      ����������� � ��������� �������
//*************************************************
 void home_temp(void)
 {
    switch (events)
    {
        case MAIN_EVENT:
         //   if (readTemp_Single(&temperature, &time_flag, &timer_val))
         //   {
                clear_matrix();
                if (!((temperature/10) % 10)) // ���� ����� ����� 0
                {
                    pic_to_led(3,1);
                  //  putchar_down(13,(temperature/10) % 10 + 48);
                    putchar_down(13,temperature % 10, pFont);
                    putchar_down(19,176, &Font);
                }else
                {
                    pic_to_led(3,1);
                    putchar_down(13,(temperature/10) % 10, pFont);
                    putchar_down(19,temperature % 10, pFont);
                    putchar_down(25,176, &Font);
                    
                }
                events = TEMP_EVENT;
                RTOS_SetTask(default_state, 750, 0);  // 3,5 ������ ��� ������

           // }
                
            break;
        case TEMP_EVENT:
            break;    
        case KEY_EXIT_EVENT:  // ����������� � ����� ����
            events = MAIN_EVENT;
            scroll_left();
            if((TTime.Thr/10) % 10)
            putchar_down(0, (TTime.Thr/10) % 10, pFont);  
            else putchar_down(0, 0, &Font);
            putchar_down(6, TTime.Thr % 10, pFont);     
            putchar_down(13, (TTime.Tmin/10) % 10, pFont);
            putchar_down(19, TTime.Tmin % 10, pFont);
            getTime(&TTime.Thr, &TTime.Tmin, &TTime.Ts);
            putchar_down_s(25, (TTime.Ts/10) % 10 + 1);
            putchar_down_s(29, TTime.Ts % 10 + 1);            
            RTOS_DeleteTask(default_state);
            RTOS_DeleteTask(home_temp);
            RTOS_SetTask(time_led,0,cycle_main);
            break;
    }
 }
 
 //=====================================================
 //   ����� ������
 //=====================================================
 void set_font()
 {
     switch (type_font)
     {
         case 1:
             pFont = &dFont1;
            break;
         case 2:
             pFont = &dFont2;
             break;
         case 3:
             pFont = &dFont3;
             break;
         case 4:
             pFont = &dFont4;
             break;
         case 5:
             pFont = &dFont5;
             break;
     }
 }

//==================================================
//  ���� ������������ �����
// =================================================
 void pressure(void)
 {
    
    switch (events)
    {
        case MAIN_EVENT:
            clear_matrix();
            if (press)
            {
                    pic_to_led(3,4);
                    putchar_down(11,(press / 100) % 10, pFont);
                    putchar_down(17,(press / 10) % 10, pFont);
                    putchar_down(23,press % 10, pFont);
            } else
            {
                    pic_to_led(3,4);
                    putchar_down(11,'E', &Font);
                    putchar_down(17,'R', &Font);
                    putchar_down(23,'R', &Font);
                
            }

                events = TEMP_EVENT;
                RTOS_SetTask(default_state, 400, 0);  // 3,5 ������ ��� ������

           // }
                
            break;
        case TEMP_EVENT:
            break;    
        case KEY_EXIT_EVENT:  // ����������� � ����� ����
            events = MAIN_EVENT;
            //
            if (press)
            {
                sprintf(text_buf, "��.��.��.");  
                interval_scroll_text();
            }else scroll_left(); 
            if((TTime.Thr/10) % 10)
            putchar_down(0, (TTime.Thr/10) % 10, pFont);  
            else putchar_down(0, 0, &Font);
            putchar_down(6, TTime.Thr % 10, pFont);     
            putchar_down(13, (TTime.Tmin/10) % 10, pFont);
            putchar_down(19, TTime.Tmin % 10, pFont);
            getTime(&TTime.Thr, &TTime.Tmin, &TTime.Ts);
            putchar_down_s(25, (TTime.Ts/10) % 10 + 1);
            putchar_down_s(29, TTime.Ts % 10 + 1);            
            RTOS_DeleteTask(default_state);
            RTOS_DeleteTask(pressure);
            RTOS_SetTask(time_led,0,cycle_main);
            break;
    }
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
            if((TTime.Ts>5)&&(TTime.Ts<7))
            {
                readTemp_Single(&temperature, &time_flag, &timer_val);
            }
            if(((TTime.Ts>14)&&(TTime.Ts<16))||((TTime.Ts>45)&&(TTime.Ts<47)))    //  �������� �����������
                events = KEY_DOWN_EVENT;
            break;
        case  KEY_OK_EVENT:     // ���� ��������� ������ ��
            RTOS_DeleteTask(time_led);  // ���������� ������ � ��� ������
            RTOS_SetTask(time_set_min, 0, 50); //  ������� ������ ������������ ���������
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            TSTime = TTime;
            events = MAIN_EVENT;
            break;
        case  KEY_UP_EVENT:
   //         asm("nop");

            press = BMP085Pressure(1);
            scroll_left();
            RTOS_DeleteTask(time_led);      //��������� ������
            RTOS_SetTask(pressure, 0, cycle_main); //������ ������ 
            events = MAIN_EVENT;
            //  scroll_left();
//            clear_matrix();
//            pic_to_led(0,1);
//            pic_to_led(8,2);
//            pic_to_led(16,3);
            break;
        case  KEY_DOWN_EVENT:
            //ow_reset();
        //    init_ds18b20();
            scroll_left();
            RTOS_DeleteTask(time_led);      //��������� ������
            RTOS_SetTask(home_temp, 0, cycle_main); //������ ������ 
            events = MAIN_EVENT;
            break;
        case KEY_EXIT_EVENT:
            events = MAIN_EVENT;
            RTOS_DeleteTask(default_state);
            break; 
        case TEMP_EVENT:
            
            break;
   }           
            Update_Matrix(Dis_Buff);          // �������� ��� �� ������
               

}


//  ���� ����
void version(void)
{
    uint8_t i;
    
    sprintf(text_buf, VERSION); 
    interval_scroll_text();
//    while(scroll_text())
//    {
//        Update_Matrix(Dis_Buff);          // �������� ��� �� ������
//        for(i=0; i<SPEED_STRING; i++)
//            __delay_ms(1);
//
//    };
}

void default_state(void)
{
    events = KEY_EXIT_EVENT;
}

// ������� ����������� �� ����� RB0
 void INT0_ISR(void)
{
     
//     if (++count = 2)
//     {
//         count = 0;
//         timer_flag = 1;
//     }
     
     RTOS_SetTask(GetTime, 0, 0); // ������ ����������� ������ ������ � ���������
                                        // ���� 500��.

}
 
 
 // ������� ����������� ������1

 void TMR1_ISR(void)
{

     
     if(++timer_val >= 150)      // �������� > 375��
     {
         timer_val = 0;
         time_flag = 1;
         T1CONbits.TMR1ON = 0;

     }
     
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

