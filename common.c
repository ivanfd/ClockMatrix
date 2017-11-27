#include "common.h"

extern uint8_t TxtBuf[6]; // ����� �� ����
extern uint8_t Dis_Buff[BUF_SIZE]; // ����� �������
extern uint8_t key_event; // ���� ������
extern uint8_t text_buf[100]; // ����� ��� �� ������
uint8_t events = MAIN_EVENT; // 
//uint8_t delay_digit = 0;            // ��� ����� ������� ����
bit show_digit; // �� ���������� �����, � ���. �������
bit en_put; // �� ����� ������ � ����� �������
uint8_t temperature; // ����������� � ��������� �������
uint8_t temperature_radio; // ����������� � ���� �������
bit err_ds18 = 0;// ������� ������� ����
uint16_t err_ds_count = 0;
uint8_t minus; // ���� �����������
uint8_t timer_val = 0, time_flag = 0; // ��� ������������� �����������
extern uint8_t(*pFont)[][5];
uint8_t type_font = 0; // ����� �����
uint32_t press, temperbmp280; // ����������� ����
extern uint8_t play_sound; //  �� ����� ����������
uint8_t type_clk = TYPE_CLK_1; // ������ ���������
uint8_t idx_pnt = 0; // ������, ��� ������� �������� ������
extern uint8_t x1, x2, x3, x4, y1, y2, y3, y4; //��� ����� ������� ����
__EEPROM_DATA(3, 1, 0, 0, 0, 0, 0, 0); // ����������� ������, 
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
        case KEY_OK_EVENT: // ���� ��������� ��

                    RTOS_DeleteTask(time_set_min); // ��������� ������
            RTOS_SetTask(time_set_hr, 0, 50); // ������� ������, ������������ �����
            RTOS_SetTask(default_state, 2000, 0); // 10 ������ ��� ������
            if (type_clk == TYPE_CLK_1) {
                putchar_b_buf(13, (TSTime.Tmin / 10) % 10, pFont);
                putchar_b_buf(19, TSTime.Tmin % 10, pFont);
            } else {
                putchar_b_buf(19, (TTime.Tmin / 10) % 10, pFont);
                putchar_b_buf(25, TTime.Tmin % 10, pFont);
            }
            events = MAIN_EVENT;
            en_put = 0;
            break;

        case KEY_EXIT_EVENT: // ������ EXIT
            RTOS_DeleteTask(time_set_min); // ���������� � �������� �����
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            TSTime.Tmin++;
            if (TSTime.Tmin > 59) TSTime.Tmin = 0;
            RTOS_SetTask(default_state, 2000, 0); // 5 ������ ��� ������
            setTime(TSTime.Thr, TSTime.Tmin, 0);
            events = MAIN_EVENT;
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tmin--;
            if (TSTime.Tmin == 255) TSTime.Tmin = 59;
            RTOS_SetTask(default_state, 2000, 0); // 5 ������ ��� ������
            setTime(TSTime.Thr, TSTime.Tmin, 0);
            events = MAIN_EVENT;
            break;
    }
    if (en_put) {
        if (show_digit) // �� ���������� �����
        {
            if (type_clk == TYPE_CLK_1) {
                putchar_b_buf(13, (TSTime.Tmin / 10) % 10, pFont);
                putchar_b_buf(19, TSTime.Tmin % 10, pFont);
            } else {
                putchar_b_buf(19, (TSTime.Tmin / 10) % 10, pFont);
                putchar_b_buf(25, TSTime.Tmin % 10, pFont);
            }
        } else {
            if (type_clk == TYPE_CLK_1) {
                putchar_b_buf(13, 0, &Font);
                putchar_b_buf(19, 0, &Font);
            } else {
                putchar_b_buf(19, 0, &Font);
                putchar_b_buf(25, 0, &Font);
            }
        }
    }
    Update_Matrix(Dis_Buff); // �������� ��� �� ������
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
            en_put = 0; // ���������� ��������
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(time_set_hr);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            clear_matrix();
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

           // putchar_b_buf(13,(TSTime.Tmin/10) % 10, pFont);
            // putchar_b_buf(19,TSTime.Tmin % 10, pFont);
            if (type_clk == TYPE_CLK_1) {
                if ((TSTime.Thr / 10) % 10)
                    putchar_b_buf(0, (TSTime.Thr / 10) % 10, pFont);
                else
                    putchar_b_buf(0, 0, &Font);
                putchar_b_buf(6, TSTime.Thr % 10, pFont);
            } else {
                if ((TSTime.Thr / 10) % 10)
                    putchar_b_buf(1, (TSTime.Thr / 10) % 10, pFont);
                else
                    putchar_b_buf(1, 0, &Font);
                putchar_b_buf(7, TSTime.Thr % 10, pFont);
            }
        } else {
            if (type_clk == TYPE_CLK_1) {
                putchar_b_buf(0, 0, &Font);
                putchar_b_buf(6, 0, &Font);
            } else {

                putchar_b_buf(1, 0, &Font);
                putchar_b_buf(7, 0, &Font);
            }
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
            en_put = 0;
            clear_matrix();
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
            en_put = 0;
            clear_matrix();
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
            en_put = 0;
            clear_matrix();
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
            en_put = 0;
            clear_matrix();
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
//  ������������ ������
//***************************************************************
void set_font_set(void)
{
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_font_set);
            RTOS_SetTask(set_type_clk, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_font_set);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
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


//***************************************************************
//  ������������ ������� ���������
//***************************************************************
void set_type_clk(void)
{
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_type_clk);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state);
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_type_clk);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            type_clk++;
            if(type_clk > 2) type_clk = 1;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_TYPE_CLK,type_clk);
            break;
        case KEY_DOWN_EVENT:
            type_clk--;
            if(type_clk == 0) type_clk = 2;           
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_TYPE_CLK,type_clk);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,'�', &Font);
    putchar_b_buf(6,'�', &Font);
    putchar_b_buf(12,':', &Font);
    putchar_b_buf(18,type_clk % 10, pFont);
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
                } else {
                pic_to_led(3, 1);
                putchar_down(13, (temperature / 10) % 10, pFont);
                putchar_down(19, temperature % 10, pFont);
                putchar_down(25, 176, &Font);

            }
            events = TEMP_EVENT;
            RTOS_SetTask(default_state, 650, 0); // 3,5 ������ ��� ������

            // }

            break;
        case TEMP_EVENT:
            break;
        case KEY_EXIT_EVENT: // ����������� � ����� ����
            events = MAIN_EVENT;
            scroll_right();
            //pre_ref_dis();
            RTOS_DeleteTask(default_state);
            RTOS_DeleteTask(home_temp);
            RTOS_SetTask(radio_temp, 0, cycle_main);
            // clear_matrix();
            break;
    }
}

//**************************************************
//      ����������� � ���� �������
//*************************************************

void radio_temp(void) {
    switch (events) {
        case MAIN_EVENT:
            clear_matrix();
            if (err_ds18) {
                pic_to_led(0, 2);
                putchar_down(9, 'E', &Font);
                putchar_down(15, 'r', &Font);
                putchar_down(21, 'r', &Font);
            } else {


                if (!((temperature_radio / 10) % 10)) // ���� ����� ����� 0
                {
                    pic_to_led(0, 2);
                    //  putchar_down(13,(temperature/10) % 10 + 48);
                    putchar_down(9, minus, &Font);
                    putchar_down(15, temperature_radio % 10, pFont);
                    putchar_down(21, 176, &Font);

                } else {
                    pic_to_led(0, 2);
                    putchar_down(9, minus, &Font);
                    putchar_down(15, (temperature_radio / 10) % 10, pFont);
                    putchar_down(22, temperature_radio % 10, pFont);
                    putchar_down(27, 176, &Font);

                }
            }
                events = TEMP_EVENT;
                RTOS_SetTask(default_state, 650, 0);  // 3,5 ������ ��� ������

           // }
                
            break;
        case TEMP_EVENT:
            break;    
        case KEY_EXIT_EVENT:  // ����������� � ����� ����
            events = MAIN_EVENT;
            scroll_left();
            pre_ref_dis();
            RTOS_DeleteTask(default_state);
            RTOS_DeleteTask(radio_temp);
            RTOS_SetTask(time_led, 0, cycle_main);
           // clear_matrix();
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
     uint16_t pr;
    
    switch (events)
    {
        case MAIN_EVENT:
            clear_matrix();
            if (press) {
//                pic_to_led(3, 4);
//                putchar_down(11, (press / 100) % 10, pFont);
//                putchar_down(17, (press / 10) % 10, pFont);
                //                putchar_down(23, press % 10, pFont);
                pr = press / 100;
                pic_to_led(3, 4);
                putchar_down(11, (pr / 100) % 10, pFont);
                putchar_down(17, (pr / 10) % 10, pFont);
                putchar_down(23, pr % 10, pFont);
                //putchar_down(25, '.', &Font);
                
            } else {
                pic_to_led(3, 4);
                putchar_down(11, 'E', &Font);
                putchar_down(17, 'R', &Font);
                putchar_down(23, 'R', &Font);

            }

            events = TEMP_EVENT;
            RTOS_SetTask(default_state, 400, 0); // 3,5 ������ ��� ������

            // }

            break;
        case TEMP_EVENT:
            break;
        case KEY_EXIT_EVENT: // ����������� � ����� ����
            events = MAIN_EVENT;
            //
            if (press) {
                sprintf(text_buf, "��.��.��.");
                interval_scroll_text();
            } else
                scroll_left();
            pre_ref_dis();
            RTOS_DeleteTask(default_state);
            RTOS_DeleteTask(pressure);
            RTOS_SetTask(time_led, 0, cycle_main);
            //clear_matrix();
            break;
    }
}

void pre_ref_dis(void) {


    clear_matrix();
    switch (type_clk) {
        case TYPE_CLK_1:
            if ((TTime.Thr / 10) % 10)
                putchar_down(0, (TTime.Thr / 10) % 10, pFont);
            else putchar_down(0, 0, &Font);
            putchar_down(6, TTime.Thr % 10, pFont);
            putchar_down(13, (TTime.Tmin / 10) % 10, pFont);
            putchar_down(19, TTime.Tmin % 10, pFont);
            getTime(&TTime.Thr, &TTime.Tmin, &TTime.Ts);
            putchar_down_s(25, (TTime.Ts / 10) % 10 + 1);
            putchar_down_s(29, TTime.Ts % 10 + 1);
            break;
        case TYPE_CLK_2:
            if ((TTime.Thr / 10) % 10)
                putchar_down(1, (TTime.Thr / 10) % 10, pFont);
            else putchar_down(1, 0, &Font);
            putchar_down(7, TTime.Thr % 10, pFont);
            putchar_down(13, ':', &Font);
            putchar_down(19, (TTime.Tmin / 10) % 10, pFont);
            putchar_down(25, TTime.Tmin % 10, pFont);
            break;
    }
    idx_pnt = 0;
    x1 = y1;
    x2 = y2;
    x3 = y3;
    x4 = y4;
}


//==================================================
//  �������� ������ �� ������� -:)
//==================================================
void time_led()
{
    uint8_t data_array[4];
    static uint16_t test = 0;

   switch (events)
   {
        case MAIN_EVENT:
            FillBuf(type_clk);
            if((TTime.Ts>5)&&(TTime.Ts<7))          //��������� �����������
            {
                readTemp_Single(&temperature, &minus, &time_flag, &timer_val);
            }
            if(((TTime.Ts>14)&&(TTime.Ts<16)))// ||((TTime.Ts>45)&&(TTime.Ts<47)))    //  �������� �����������
                events = KEY_DOWN_EVENT;
            if(((TTime.Ts>39)&&(TTime.Ts<41)))// ||((TTime.Ts>45)&&(TTime.Ts<47)))    //  �������� �����������
                events = KEY_UP_EVENT;
            break;
        case KEY_OK_EVENT: // ���� ��������� ������ ��
            RTOS_DeleteTask(time_led); // ���������� ������ � ��� ������
            RTOS_SetTask(time_set_min, 0, 50); //  ������� ������ ������������ ���������
            RTOS_SetTask(default_state, 2000, 0); // 10 ������ ��� ������
            TSTime = TTime;
            events = MAIN_EVENT;
            en_put = 0;
            if (type_clk == TYPE_CLK_2)
                putchar_b_buf(13, 23, &Font);
            //clear_matrix();
            break;
        case  KEY_UP_EVENT:
   //         asm("nop");
            
            bmp280Convert(&press, &temperbmp280);
         ////  press = BMP085Pressure(1);
            //scroll_left();
            if (type_clk == TYPE_CLK_2)
                putchar_b_buf(13, 23, &Font);
            scroll_right();
            RTOS_DeleteTask(time_led); //��������� ������
            RTOS_SetTask(pressure, 0, cycle_main); //������ ������ 
            events = MAIN_EVENT;
            en_put = 0;

            //  scroll_left();
            //            clear_matrix();
            //            pic_to_led(0,1);
//            pic_to_led(8,2);
//            pic_to_led(16,3);
            break;
        case  KEY_DOWN_EVENT:
            //ow_reset();
            //    init_ds18b20();
            if (type_clk == TYPE_CLK_2)
                putchar_b_buf(13, 23, &Font);
            scroll_left();
            RTOS_DeleteTask(time_led); //��������� ������
            RTOS_SetTask(home_temp, 0, cycle_main); //������ ������ 
            events = MAIN_EVENT;
            en_put = 0;
            break;
        case KEY_EXIT_EVENT:
            events = MAIN_EVENT;
            RTOS_DeleteTask(default_state);
#ifdef DEBUG
            __delay_ms(10);
            //printf("Test_NRF =  %u\n\r", nrf24_read_reg(NRF24_RX_ADDR_P0));
            //printf("Test_NRF_Chan =  %u\n\r", nrf24_read_reg(NRF24_RF_CH));

            if (nrf24_dataReady()) {
                test++;
                nrf24_getData(&data_array);
                printf("%u > ", test);
                printf("%u ", data_array[0]);
                printf("%c ", data_array[1]);
                printf("%2X ", data_array[2]);
                printf("%2X\r\n", data_array[3]);
            }
#endif  
            break;
        case TEMP_EVENT:
            
            break;
    }
    if (en_put)
        Update_Matrix(Dis_Buff); // �������� ��� �� ������
    en_put = 1;
    // ������ ����������
    if (nrf24_dataReady()) {
        nrf24_getData(&data_array);
        spi_rw(FLUSH_RX);   // �������� ��������� �����

        temperature_radio = data_array[0];
        minus = data_array[1];
        err_ds_count = 0;
        err_ds18 = 0;
    } else
        err_ds_count++;

    if (err_ds_count > 600) // ������ �������. ���� �� ���� � ������ ����������
        err_ds18 = 1;       // �� ������� ������� ������� �����������
    

#ifdef DEBUG
            if (nrf24_dataReady()) {
                test++;
                nrf24_getData(&data_array);
                printf("%u > ", test);
                printf("%u ", data_array[0]);
                printf("%c ", data_array[1]);
                printf("%2X ", data_array[2]);
                printf("%2X\r\n", data_array[3]);
            }
#endif  
    

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
     idx_pnt++;
     if (idx_pnt == 8) idx_pnt = 0;
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

