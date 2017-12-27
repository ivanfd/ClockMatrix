#include "settings.h"

extern uint8_t(*pFont)[][5];
extern uint8_t Dis_Buff[BUF_SIZE]; // ����� �������
extern uint8_t type_clk; // ������ ���������
extern uint8_t events;
extern bit en_put; // �� ����� ������ � ����� �������
extern bit show_digit; // �� ���������� �����, � ���. �������
extern uint8_t type_font;
extern uint8_t en_h_snd; // �� ����� ���������� ������
extern uint8_t brig;// �������� ���������
extern uint8_t brg_type;// ��������� �� �������, �� �������
extern uint8_t blk_dot; // ����� �� ������� ������
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
    Update_Matrix(Dis_Buff); // �������� ���� �� ������
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
    Update_Matrix(Dis_Buff);          // �������� ���� �� ������
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
    putchar_b_buf(0,STR_YR[0], &Font);
    putchar_b_buf(6,STR_YR[1], &Font);
    putchar_b_buf(12,STR_YR[2], &Font);
    putchar_b_buf(18,(TSTime.Tyr/10) % 10, pFont);
    putchar_b_buf(24,TSTime.Tyr % 10, pFont);
}    
       Update_Matrix(Dis_Buff);          // �������� ���� �� ������
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
    putchar_b_buf(0,STR_MNT[0], &Font);
    putchar_b_buf(6,STR_MNT[1], &Font);
    putchar_b_buf(12,STR_MNT[2], &Font);
    switch(TSTime.Tmt)
    {
        case 1:
        putchar_b_buf(18,STR_MNT_1[0], &Font);
        putchar_b_buf(24,STR_MNT_1[1], &Font);
        break;
        case 2:
        putchar_b_buf(18,STR_MNT_2[0], &Font);
        putchar_b_buf(24,STR_MNT_2[1], &Font);
        break;
        case 3:
        putchar_b_buf(18,STR_MNT_3[0], &Font);
        putchar_b_buf(24,STR_MNT_3[1], &Font);
        break;
        case 4:
        putchar_b_buf(18,STR_MNT_4[0], &Font);
        putchar_b_buf(24,STR_MNT_4[1], &Font);
        break;
        case 5:
        putchar_b_buf(18,STR_MNT_5[0], &Font);
        putchar_b_buf(24,STR_MNT_5[1], &Font);
        break;
        case 6:
        putchar_b_buf(18,STR_MNT_6[0], &Font);
        putchar_b_buf(24,STR_MNT_6[1], &Font);
        break;        
        case 7:
        putchar_b_buf(18,STR_MNT_7[0], &Font);
        putchar_b_buf(24,STR_MNT_7[1], &Font);
        break;
        case 8:
        putchar_b_buf(18,STR_MNT_8[0], &Font);
        putchar_b_buf(24,STR_MNT_8[1], &Font);
        break;        
        case 9:
        putchar_b_buf(18,STR_MNT_9[0], &Font);
        putchar_b_buf(24,STR_MNT_9[1], &Font);
        break;
        case 10:
        putchar_b_buf(18,STR_MNT_10[0], &Font);
        putchar_b_buf(24,STR_MNT_10[1], &Font);
        break;        
        case 11:
        putchar_b_buf(18,STR_MNT_11[0], &Font);
        putchar_b_buf(24,STR_MNT_11[1], &Font);
        break;
        case 12:
        putchar_b_buf(18,STR_MNT_12[0], &Font);
        putchar_b_buf(24,STR_MNT_12[1], &Font);
        break;        

    }
}    
       Update_Matrix(Dis_Buff);          // �������� ���� �� ������
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
    putchar_b_buf(0,STR_DATE[0], &Font);
    putchar_b_buf(6,STR_DATE[1], &Font);
    putchar_b_buf(12,STR_DATE[2], &Font);
    putchar_b_buf(18,(TSTime.Tdt/10) % 10, pFont);
    putchar_b_buf(24,TSTime.Tdt % 10 , pFont);
}    
       Update_Matrix(Dis_Buff);          // �������� ���� �� ������
       en_put=1;
}


//***************************************************************
//  ������������ ����
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
    putchar_b_buf(0,STR_DAY[0], &Font);
    putchar_b_buf(6,STR_DAY[1], &Font);
    putchar_b_buf(12,STR_DAY[2], &Font);
    switch(TSTime.Tdy)
    {
        case 2:
        putchar_b_buf(18,STR_DAY_1[0], &Font);
        putchar_b_buf(24,STR_DAY_1[1], &Font);
        break;
        case 3:
        putchar_b_buf(18,STR_DAY_2[0], &Font);
        putchar_b_buf(24,STR_DAY_2[1], &Font);
        break;
        case 4:
        putchar_b_buf(18,STR_DAY_3[0], &Font);
        putchar_b_buf(24,STR_DAY_3[1], &Font);
        break;
        case 5:
        putchar_b_buf(18,STR_DAY_4[0], &Font);
        putchar_b_buf(24,STR_DAY_4[1], &Font);
        break;
        case 6:
        putchar_b_buf(18,STR_DAY_5[0], &Font);
        putchar_b_buf(24,STR_DAY_5[1], &Font);
        break;
        case 7:
        putchar_b_buf(18,STR_DAY_6[0], &Font);
        putchar_b_buf(24,STR_DAY_6[1], &Font);
        break;        
        case 1:
        putchar_b_buf(18,STR_DAY_7[0], &Font);
        putchar_b_buf(24,STR_DAY_7[1], &Font);
        break;
    }
}    
       Update_Matrix(Dis_Buff);          // �������� ���� �� ������
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
    putchar_b_buf(0,STR_FONT[0], &Font);
    putchar_b_buf(6,STR_FONT[1], &Font);
    putchar_b_buf(12,STR_FONT[2], &Font);
    putchar_b_buf(18,type_font % 10, pFont);
    putchar_b_buf(24,0, &Font);
}    
       Update_Matrix(Dis_Buff);          // �������� ���� �� ������
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
            RTOS_SetTask(brg_set, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
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
    putchar_b_buf(0,STR_TP_CLK[0], &Font);
    putchar_b_buf(6,STR_TP_CLK[1], &Font);
        putchar_b_buf(12, STR_TP_CLK[2], &Font);
        putchar_b_buf(18, type_clk % 10, pFont);
        putchar_b_buf(24, 0, &Font);
    }
    Update_Matrix(Dis_Buff); // �������� ���� �� ������
    en_put = 1;
    if (type_clk == 1)
        blk_dot = 0;
    else
        blk_dot = 1;
}

 //=====================================================
 //   ������������ - ��������� ���� �� �������
 //=====================================================

void brg_set() {
    switch (events) {
        case MAIN_EVENT:

            break;
        case KEY_OK_EVENT:
            if (brg_type) {
                RTOS_DeleteTask(brg_set);
                RTOS_SetTask(set_sound_h, 0, cycle_main);
                RTOS_SetTask(default_state, 2000, 0); // 10 ������ ��� ������
                events = MAIN_EVENT;
                en_put = 0;
                clear_matrix();
            } else {
                RTOS_DeleteTask(brg_set);
                RTOS_SetTask(set_brg_manual, 0, cycle_main);
                brig = read_eep(EE_DAT_BRG);
                Cmd7221(INTENSITY_R, brig); //Intensity Register
                RTOS_SetTask(default_state, 2000, 0); // 10 ������ ��� ������
                events = MAIN_EVENT;
                en_put = 0;
                clear_matrix();
            }
            
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(brg_set);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            brg_type = !brg_type;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_TYPE_BRG,brg_type);
            break;
        case KEY_DOWN_EVENT:
            brg_type = !brg_type;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_TYPE_BRG,brg_type);
            break;
    }
    if (en_put) {
        putchar_b_buf(0, STR_BRG[0], &Font);
        putchar_b_buf(6, STR_BRG[1], &Font);
        putchar_b_buf(12, STR_BRG[2], &Font);
        if (brg_type) {
            putchar_b_buf(18, STR_BRG_A[0], &Font);
            putchar_b_buf(24, STR_BRG_A[1], &Font);
        } else {
            putchar_b_buf(18, STR_BRG_M[0], &Font);
            putchar_b_buf(24, STR_BRG_M[1], &Font);
        }

    }
    Update_Matrix(Dis_Buff); // �������� ���� �� ������
    en_put = 1;

}


//**************************************************
//      ����� ��������� - ������������
//*************************************************
void set_brg_manual(){
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_brg_manual);
            RTOS_SetTask(set_sound_h, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_brg_manual);
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            brig++;
            if(brig > 8) brig = 0;
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_DAT_BRG,brig);
            Cmd7221(INTENSITY_R, brig); //Intensity Register
            break;
        case KEY_DOWN_EVENT:
            brig--;
            if(brig == 255) brig = 8;           
            RTOS_SetTask(default_state, 2000, 0);  // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_DAT_BRG,brig);
            Cmd7221(INTENSITY_R, brig); //Intensity Register
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_BRG[0], &Font);
    putchar_b_buf(6,STR_BRG[1], &Font);
    putchar_b_buf(12,STR_BRG[2], &Font);
    putchar_b_buf(18,brig % 10, pFont);
    putchar_b_buf(24,0, &Font);
}    
       Update_Matrix(Dis_Buff);          // �������� ���� �� ������
       en_put=1;    
    
}


//============================================================
// ������������ - �� ����� ����������� ���������� ������. 
//============================================================
void set_sound_h(void){
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_sound_h);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_sound_h);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            en_h_snd++;
            if (en_h_snd > 1) en_h_snd = 0;
            RTOS_SetTask(default_state, 2000, 0); // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_EN_SND_H, en_h_snd);
            break;
        case KEY_DOWN_EVENT:
            en_h_snd--;
            if (en_h_snd == 255) en_h_snd = 1;
            RTOS_SetTask(default_state, 2000, 0); // 10 ������ ��� ������
            events = MAIN_EVENT;
            write_eep(EE_EN_SND_H, en_h_snd);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_SND_H[0], &Font);
    putchar_b_buf(6,STR_SND_H[1], &Font);
    putchar_b_buf(12,STR_SND_H[2], &Font);
    putchar_b_buf(18,en_h_snd % 10, pFont);
    putchar_b_buf(24,0, &Font);
}    
       Update_Matrix(Dis_Buff);          // �������� ���� �� ������
       en_put=1;    
    
}

