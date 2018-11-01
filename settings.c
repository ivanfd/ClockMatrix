#include "settings.h"
#include "nrf24l01p.h"

extern uint8_t(*pFont)[][5];
extern uint8_t Dis_Buff[BUF_SIZE]; // буфер дисплея
extern uint8_t type_clk; // вигляд годинника
extern uint8_t events;
extern bit en_put; // чи можна писати у буфер символи
extern bit show_digit; // чи показувати цифри, в нал. мигання
extern uint8_t type_font;
extern uint8_t en_h_snd; // чи можна генерувати сигнал
extern uint8_t brig;// значення яскравості
extern uint8_t brg_type;// яскравість по датчику, чи постійна
extern uint8_t blk_dot; // дозвіл на мигання кнопок
extern uint8_t en_ds_1;    //  чи пок. температуру з датчика 1
extern uint8_t en_ds_2;    //  чи пок. температуру з датчика 2
extern uint8_t en_bmp280; //  чи показуємо тиск
extern uint8_t dst_flag; // чи зараз літній час????
extern uint8_t en_dst; // перехід на літній час
extern uint8_t type_temp;
extern uint8_t en_am2302; //датчик вологості
//****************************************
// налаштування годинника -  хвилини
//****************************************
void time_set_min(void)
{
    show_digit = ~show_digit;
   switch (events)
   {
        case MAIN_EVENT:

            break;
        case KEY_OK_EVENT: // якщо натиснули ок

                    RTOS_DeleteTask(time_set_min); // видаляємо задачу
            RTOS_SetTask(time_set_hr, 0, 50); // ставимо задачу, налаштування годин
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            if (type_clk == TYPE_CLK_1) {
                putchar_b_buf(13, (TSTime.Tmin / 10) % 10, pFont, &Dis_Buff);
                putchar_b_buf(19, TSTime.Tmin % 10, pFont, &Dis_Buff);
            } else {
                putchar_b_buf(19, (TTime.Tmin / 10) % 10, pFont, &Dis_Buff);
                putchar_b_buf(25, TTime.Tmin % 10, pFont, &Dis_Buff);
            }
            events = MAIN_EVENT;
            en_put = 0;
            break;

        case KEY_EXIT_EVENT: // кнопка EXIT
            RTOS_DeleteTask(time_set_min); // переходимо в основний режим
            RTOS_DeleteTask(default_state);
            RTOS_SetTask(time_led, 0, cycle_main);
            events = MAIN_EVENT;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            TSTime.Tmin++;
            if (TSTime.Tmin > 59) TSTime.Tmin = 0;
            RTOS_SetTask(default_state, 2000, 0); // 5 секунд для виходу
            setTime(TSTime.Thr, TSTime.Tmin, 0);
            events = MAIN_EVENT;
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tmin--;
            if (TSTime.Tmin == 255) TSTime.Tmin = 59;
            RTOS_SetTask(default_state, 2000, 0); // 5 секунд для виходу
            setTime(TSTime.Thr, TSTime.Tmin, 0);
            events = MAIN_EVENT;
            break;
    }
    if (en_put) {
        if (show_digit) // чи показувати цифри
        {
            if (type_clk == TYPE_CLK_1) {
                putchar_b_buf(13, (TSTime.Tmin / 10) % 10, pFont, &Dis_Buff);
                putchar_b_buf(19, TSTime.Tmin % 10, pFont, &Dis_Buff);
            } else {
                putchar_b_buf(19, (TSTime.Tmin / 10) % 10, pFont, &Dis_Buff);
                putchar_b_buf(25, TSTime.Tmin % 10, pFont, &Dis_Buff);
            }
        } else {
            if (type_clk == TYPE_CLK_1) {
                putchar_b_buf(13, 0, &Font, &Dis_Buff);
                putchar_b_buf(19, 0, &Font, &Dis_Buff);
            } else {
                putchar_b_buf(19, 0, &Font, &Dis_Buff);
                putchar_b_buf(25, 0, &Font, &Dis_Buff);
            }
        }
    }
    Update_Matrix(Dis_Buff); // обновити дані на дисплеї
    en_put = 1;
}

//****************************************
// налаштування годинника -  годин
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            //RTOS_DeleteTask(default_state);
            events = MAIN_EVENT;
            clear_matrix();
            en_put = 0; // заборонити малювати
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
            if (TSTime.Thr > 23) TSTime.Thr = 0;
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            if (dst_flag) {// якщо час літній
                if (TSTime.Thr == 0)
                    setTime(23, TSTime.Tmin, 0);
                else
                    setTime((TSTime.Thr - 1), TSTime.Tmin, 0);
            } else
                setTime(TSTime.Thr, TSTime.Tmin, 0);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Thr--;
            
            if (TSTime.Thr == 255) TSTime.Thr = 23;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            if (dst_flag) {// якщо час літній
                if (TSTime.Thr == 0)
                    setTime(23, TSTime.Tmin, 0);
                else
                    setTime((TSTime.Thr - 1), TSTime.Tmin, 0);
            } else
                setTime(TSTime.Thr, TSTime.Tmin, 0);
            break;            
   }   
   if(en_put)
   {
        if(show_digit)      // чи показувати цифри
        {

           // putchar_b_buf(13,(TSTime.Tmin/10) % 10, pFont);
            // putchar_b_buf(19,TSTime.Tmin % 10, pFont);
            if (type_clk == TYPE_CLK_1) {
                if ((TSTime.Thr / 10) % 10)
                    putchar_b_buf(0, (TSTime.Thr / 10) % 10, pFont, &Dis_Buff);
                else
                    putchar_b_buf(0, 0, &Font, &Dis_Buff);
                putchar_b_buf(6, TSTime.Thr % 10, pFont, &Dis_Buff);
            } else {
                if ((TSTime.Thr / 10) % 10)
                    putchar_b_buf(1, (TSTime.Thr / 10) % 10, pFont, &Dis_Buff);
                else
                    putchar_b_buf(1, 0, &Font, &Dis_Buff);
                putchar_b_buf(7, TSTime.Thr % 10, pFont, &Dis_Buff);
            }
        } else {
            if (type_clk == TYPE_CLK_1) {
                putchar_b_buf(0, 0, &Font, &Dis_Buff);
                putchar_b_buf(6, 0, &Font, &Dis_Buff);
            } else {

                putchar_b_buf(1, 0, &Font, &Dis_Buff);
                putchar_b_buf(7, 0, &Font, &Dis_Buff);
            }
        }
    }
    Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
    en_put = 1;                         // дозволити малювати символи
}
//***************************************************************
//  встановлення року
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tyr--;
            if(TSTime.Tyr == 255) TSTime.Tyr = 99;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_YR[0], &Font, &Dis_Buff);
    putchar_b_buf(6,STR_YR[1], &Font, &Dis_Buff);
    putchar_b_buf(12,STR_YR[2], &Font, &Dis_Buff);
    putchar_b_buf(18,(TSTime.Tyr/10) % 10, pFont, &Dis_Buff);
    putchar_b_buf(24,TSTime.Tyr % 10, pFont, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;
}


//***************************************************************
//  встановлення місяць
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tmt--;
            if(TSTime.Tmt == 0) TSTime.Tmt = 12;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_MNT[0], &Font, &Dis_Buff);
    putchar_b_buf(6,STR_MNT[1], &Font, &Dis_Buff);
    putchar_b_buf(12,STR_MNT[2], &Font, &Dis_Buff);
    switch(TSTime.Tmt)
    {
        case 1:
        putchar_b_buf(18,STR_MNT_1[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_1[1], &Font, &Dis_Buff);
        break;
        case 2:
        putchar_b_buf(18,STR_MNT_2[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_2[1], &Font, &Dis_Buff);
        break;
        case 3:
        putchar_b_buf(18,STR_MNT_3[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_3[1], &Font, &Dis_Buff);
        break;
        case 4:
        putchar_b_buf(18,STR_MNT_4[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_4[1], &Font, &Dis_Buff);
        break;
        case 5:
        putchar_b_buf(18,STR_MNT_5[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_5[1], &Font, &Dis_Buff);
        break;
        case 6:
        putchar_b_buf(18,STR_MNT_6[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_6[1], &Font, &Dis_Buff);
        break;        
        case 7:
        putchar_b_buf(18,STR_MNT_7[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_7[1], &Font, &Dis_Buff);
        break;
        case 8:
        putchar_b_buf(18,STR_MNT_8[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_8[1], &Font, &Dis_Buff);
        break;        
        case 9:
        putchar_b_buf(18,STR_MNT_9[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_9[1], &Font, &Dis_Buff);
        break;
        case 10:
        putchar_b_buf(18,STR_MNT_10[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_10[1], &Font, &Dis_Buff);
        break;        
        case 11:
        putchar_b_buf(18,STR_MNT_11[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_11[1], &Font, &Dis_Buff);
        break;
        case 12:
        putchar_b_buf(18,STR_MNT_12[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_MNT_12[1], &Font, &Dis_Buff);
        break;        

    }
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;
}

//***************************************************************
//  встановлення число
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tdt--;
            if(TSTime.Tdt == 0) TSTime.Tdt = 31;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_DATE[0], &Font, &Dis_Buff);
    putchar_b_buf(6,STR_DATE[1], &Font, &Dis_Buff);
    putchar_b_buf(12,STR_DATE[2], &Font, &Dis_Buff);
    putchar_b_buf(18,(TSTime.Tdt/10) % 10, pFont, &Dis_Buff);
    putchar_b_buf(24,TSTime.Tdt % 10 , pFont, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;
}


//***************************************************************
//  встановлення день
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;
        case KEY_DOWN_EVENT:
            TSTime.Tdy--;
            if(TSTime.Tdy == 0) TSTime.Tdy = 7;            
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            setDate(TSTime.Tdy,TSTime.Tdt,TSTime.Tmt,TSTime.Tyr);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_DAY[0], &Font, &Dis_Buff);
    putchar_b_buf(6,STR_DAY[1], &Font, &Dis_Buff);
    putchar_b_buf(12,STR_DAY[2], &Font, &Dis_Buff);
    switch(TSTime.Tdy)
    {
        case 2:
        putchar_b_buf(18,STR_DAY_1[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_DAY_1[1], &Font, &Dis_Buff);
        break;
        case 3:
        putchar_b_buf(18,STR_DAY_2[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_DAY_2[1], &Font, &Dis_Buff);
        break;
        case 4:
        putchar_b_buf(18,STR_DAY_3[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_DAY_3[1], &Font, &Dis_Buff);
        break;
        case 5:
        putchar_b_buf(18,STR_DAY_4[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_DAY_4[1], &Font, &Dis_Buff);
        break;
        case 6:
        putchar_b_buf(18,STR_DAY_5[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_DAY_5[1], &Font, &Dis_Buff);
        break;
        case 7:
        putchar_b_buf(18,STR_DAY_6[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_DAY_6[1], &Font, &Dis_Buff);
        break;        
        case 1:
        putchar_b_buf(18,STR_DAY_7[0], &Font, &Dis_Buff);
        putchar_b_buf(24,STR_DAY_7[1], &Font, &Dis_Buff);
        break;
    }
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;
}

//***************************************************************
//  встановлення шрифту
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_FONT,type_font);
            set_font();
            break;
        case KEY_DOWN_EVENT:
            type_font--;
            if(type_font == 0) type_font = 5;           
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_FONT,type_font);
            set_font();
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_FONT[0], &Font, &Dis_Buff);
    putchar_b_buf(6,STR_FONT[1], &Font, &Dis_Buff);
    putchar_b_buf(12,STR_FONT[2], &Font, &Dis_Buff);
    putchar_b_buf(18,type_font % 10, pFont, &Dis_Buff);
    putchar_b_buf(24,0, &Font, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;
}

//***************************************************************
//  встановлення вигляду годинника
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_TYPE_CLK,type_clk);
            break;
        case KEY_DOWN_EVENT:
            type_clk--;
            if(type_clk == 0) type_clk = 2;           
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_TYPE_CLK,type_clk);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_TP_CLK[0], &Font, &Dis_Buff);
    putchar_b_buf(6,STR_TP_CLK[1], &Font, &Dis_Buff);
        putchar_b_buf(12, STR_TP_CLK[2], &Font, &Dis_Buff);
        putchar_b_buf(18, type_clk % 10, pFont, &Dis_Buff);
        putchar_b_buf(24, 0, &Font, &Dis_Buff);
    }
    Update_Matrix(Dis_Buff); // обновити дані на дисплеї
    en_put = 1;
    if (type_clk == 1)
        blk_dot = 0;
    else
        blk_dot = 1;
}

 //=====================================================
 //   Налаштування - яскравість авто чи постійна
 //=====================================================

void brg_set() {
    switch (events) {
        case MAIN_EVENT:

            break;
        case KEY_OK_EVENT:
            if (brg_type) {
                RTOS_DeleteTask(brg_set);
                RTOS_SetTask(set_sound_h, 0, cycle_main);
                RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
                events = MAIN_EVENT;
                en_put = 0;
                clear_matrix();
            } else {
                RTOS_DeleteTask(brg_set);
                RTOS_SetTask(set_brg_manual, 0, cycle_main);
                brig = read_eep(EE_DAT_BRG);
                Cmd7221(INTENSITY_R, brig); //Intensity Register
                RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_TYPE_BRG,brg_type);
            break;
        case KEY_DOWN_EVENT:
            brg_type = !brg_type;
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_TYPE_BRG,brg_type);
            break;
    }
    if (en_put) {
        putchar_b_buf(0, STR_BRG[0], &Font, &Dis_Buff);
        putchar_b_buf(6, STR_BRG[1], &Font, &Dis_Buff);
        putchar_b_buf(12, STR_BRG[2], &Font, &Dis_Buff);
        if (brg_type) {
            putchar_b_buf(18, STR_BRG_A[0], &Font, &Dis_Buff);
            putchar_b_buf(24, STR_BRG_A[1], &Font, &Dis_Buff);
        } else {
            putchar_b_buf(18, STR_BRG_M[0], &Font, &Dis_Buff);
            putchar_b_buf(24, STR_BRG_M[1], &Font, &Dis_Buff);
        }

    }
    Update_Matrix(Dis_Buff); // обновити дані на дисплеї
    en_put = 1;

}


//**************************************************
//      ручна яскравість - налаштування
//*************************************************
void set_brg_manual(){
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_brg_manual);
            RTOS_SetTask(set_sound_h, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
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
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_DAT_BRG,brig);
            Cmd7221(INTENSITY_R, brig); //Intensity Register
            break;
        case KEY_DOWN_EVENT:
            brig--;
            if(brig == 255) brig = 8;           
            RTOS_SetTask(default_state, 2000, 0);  // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_DAT_BRG,brig);
            Cmd7221(INTENSITY_R, brig); //Intensity Register
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_BRG[0], &Font, &Dis_Buff);
    putchar_b_buf(6,STR_BRG[1], &Font, &Dis_Buff);
    putchar_b_buf(12,STR_BRG[2], &Font, &Dis_Buff);
    putchar_b_buf(18,brig % 10, pFont, &Dis_Buff);
    putchar_b_buf(24,0, &Font, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;    
    
}


//============================================================
// Налаштування - чи можна відтворювати щогодинний сигнал. 
//============================================================
void set_sound_h(void){
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_sound_h);
            RTOS_SetTask(set_en_ds1, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу 
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
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_SND_H, en_h_snd);
            break;
        case KEY_DOWN_EVENT:
            en_h_snd--;
            if (en_h_snd == 255) en_h_snd = 1;
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_SND_H, en_h_snd);
            break;    
    }
if(en_put)
{
    putchar_b_buf(0,STR_SND_H[0], &Font, &Dis_Buff);
    putchar_b_buf(6,STR_SND_H[1], &Font, &Dis_Buff);
    putchar_b_buf(12,STR_SND_H[2], &Font, &Dis_Buff);
    putchar_b_buf(18,en_h_snd % 10, pFont, &Dis_Buff);
    putchar_b_buf(24,0, &Font, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;    
    
}

//============================================================
// Налаштування - чи можна показувати температуру з 1 датчика. 
//============================================================
void set_en_ds1(void){
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_en_ds1);
            RTOS_SetTask(set_en_ds2, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_en_ds1);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            en_ds_1 = !(en_ds_1);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_DS1, en_ds_1);
            break;
        case KEY_DOWN_EVENT:
            en_ds_1 = !(en_ds_1);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_DS1, en_ds_1);
            break;
   
    }
if(en_put) {
        putchar_b_buf(0, STR_DS1[0], &Font, &Dis_Buff);
        putchar_b_buf(6, STR_DS1[1], &Font, &Dis_Buff);
        putchar_b_buf(12, STR_DS1[2], &Font, &Dis_Buff);
        if (en_ds_1)
            putchar_b_buf(18, '+', &Font, &Dis_Buff);
        else
            putchar_b_buf(18, '-', &Font, &Dis_Buff);
        putchar_b_buf(24, 0, &Font, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;    
    
}

//============================================================
// Налаштування - чи можна показувати температуру з 2 датчика. 
//============================================================
void set_en_ds2(void){
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_en_ds2);
            RTOS_SetTask(set_en_bmp, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_en_ds2);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            en_ds_2 = !(en_ds_2);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_DS2, en_ds_2);
            break;
        case KEY_DOWN_EVENT:
            en_ds_2 = !(en_ds_2);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_DS2, en_ds_2);
            break;
   
    }
if(en_put) {
        putchar_b_buf(0, STR_DS2[0], &Font, &Dis_Buff);
        putchar_b_buf(6, STR_DS2[1], &Font, &Dis_Buff);
        putchar_b_buf(12, STR_DS2[2], &Font, &Dis_Buff);
        if (en_ds_2)
            putchar_b_buf(18, '+', &Font, &Dis_Buff);
        else
            putchar_b_buf(18, '-', &Font, &Dis_Buff);
        putchar_b_buf(24, 0, &Font, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;    
    
}

//============================================================
// Налаштування - чи можна показувати тиск 
//============================================================
void set_en_bmp(void){
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_en_bmp);
            RTOS_SetTask(set_en_dst, 0, cycle_main);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_en_bmp);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            en_bmp280 = !(en_bmp280);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_BMP, en_bmp280);
            break;
        case KEY_DOWN_EVENT:
            en_bmp280 = !(en_bmp280);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_BMP, en_bmp280);
            break;
   
    }
if(en_put) {
        putchar_b_buf(0, STR_BMP[0], &Font, &Dis_Buff);
        putchar_b_buf(6, STR_BMP[1], &Font, &Dis_Buff);
        putchar_b_buf(12, STR_BMP[2], &Font, &Dis_Buff);
        if (en_bmp280)
            putchar_b_buf(18, '+', &Font, &Dis_Buff);
        else
            putchar_b_buf(18, '-', &Font, &Dis_Buff);
        putchar_b_buf(24, 0, &Font, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;    
    
}


//============================================================
// Налаштування - літный час 
//============================================================
void set_en_dst(void){
switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_en_dst);
            RTOS_SetTask(set_type_temp, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_en_dst);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            en_dst = !(en_dst);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_DST, en_dst);
            break;
        case KEY_DOWN_EVENT:
            en_dst = !(en_dst);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_DST, en_dst);
            break;
   
    }
if(en_put) {
        putchar_b_buf(0, STR_DST[0], &Font, &Dis_Buff);
        putchar_b_buf(6, STR_DST[1], &Font, &Dis_Buff);
        putchar_b_buf(12, STR_DST[2], &Font, &Dis_Buff);
        putchar_b_buf(18, STR_DST[3], &Font, &Dis_Buff);
        if (en_dst)
            putchar_b_buf(24, '+', &Font, &Dis_Buff);
        else
            putchar_b_buf(24, '-', &Font, &Dis_Buff);
//        putchar_b_buf(24, 0, &Font, &Dis_Buff);
}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;     
    
    

}

//============================================================
// Налаштування - тип відображення температури з радіодатчика
// Є два типи. 1 - Округлена, 2 - температура з десятими
//============================================================
void set_type_temp(void){
 switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_type_temp);
            RTOS_SetTask(set_en_am2302, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_type_temp);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            type_temp ++;
            if (type_temp == 3) type_temp = 1;
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_TYPE_TEMP, type_temp);
            break;
        case KEY_DOWN_EVENT:
            type_temp --;
            if (type_temp == 0) type_temp = 2;
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_TYPE_TEMP, type_temp);
            break;
   
    }
if(en_put) {
        putchar_b_buf(0, STR_TYPE_TEMP[0], &Font, &Dis_Buff);
        putchar_b_buf(6, STR_TYPE_TEMP[1], &Font, &Dis_Buff);
        putchar_b_buf(12, STR_TYPE_TEMP[2], &Font, &Dis_Buff);
        putchar_b_buf(18, STR_TYPE_TEMP[3], &Font, &Dis_Buff);
        putchar_b_buf(24, type_temp, pFont, &Dis_Buff);

}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;    
      
    
}

//============================================================
// Налаштування - показ датчика AM2302. Вологість.
//============================================================
void set_en_am2302(void){
 switch (events)
   {
        case MAIN_EVENT:

        break;
        case KEY_OK_EVENT:
            RTOS_DeleteTask(set_en_am2302);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_EXIT_EVENT:
            RTOS_DeleteTask(set_en_am2302);
            RTOS_SetTask(time_led, 0, cycle_main);
            RTOS_DeleteTask(default_state); 
            events = MAIN_EVENT;
            en_put = 0;
            clear_matrix();
            break;
        case KEY_UP_EVENT:
            en_am2302 = !(en_am2302);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_AM2302, en_am2302);
            break;
        case KEY_DOWN_EVENT:
            en_am2302 = !(en_am2302);
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            events = MAIN_EVENT;
            write_eep(EE_EN_AM2302, en_am2302);
            break;
   
    }
if(en_put) {
        putchar_b_buf(0, STR_AM2302[0], &Font, &Dis_Buff);
        putchar_b_buf(6, STR_AM2302[1], &Font, &Dis_Buff);
        putchar_b_buf(12, STR_AM2302[2], &Font, &Dis_Buff);
        putchar_b_buf(18, STR_AM2302[3], &Font, &Dis_Buff);
        if (en_am2302)
            putchar_b_buf(24, '+', &Font, &Dis_Buff);
        else
            putchar_b_buf(24, '-', &Font, &Dis_Buff);

}    
       Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
       en_put=1;    
      
    
}

void default_state(void) {
    events = KEY_EXIT_EVENT;

}

