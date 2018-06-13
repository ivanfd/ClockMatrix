#include "common.h"

extern uint8_t TxtBuf[6]; // буфер дл¤ цифр
extern uint8_t Dis_Buff[BUF_SIZE + BUF_SIZE_TEMP]; // буфер дисплея
extern uint8_t key_event; // стан кнопок
extern uint8_t text_buf[50]; // буфер для біг строки
extern uint8_t rs_text_buf[100];
uint8_t events = MAIN_EVENT; // подія, яка відбувається
//uint8_t delay_digit = 0;            // для паузи мигання цифр
bit show_digit; // чи показувати цифри, в нал. мигання
bit en_put; // чи можна писати у буфер символи
uint8_t temperature; // температура з кімнатного датчика
uint16_t temperature_radio; // температура з радіо датчика
bit err_ds18 = 0;// помилка датчика радіо
uint16_t err_ds_count = 0;
uint8_t minus; // знак температури
uint8_t minus_radio = '+'; // знак температури
uint8_t timer_val = 0, time_flag = 0; // для конвертування температури
extern uint8_t(*pFont)[][5];
uint8_t type_font = 0; // шрифт годин
uint8_t type_temp = 2;
uint32_t press, temperbmp280; // атмосферний тиск
extern uint8_t play_sound; //  чи можна програвати
uint8_t type_clk = TYPE_CLK_1; // вигляд годинника
uint8_t idx_pnt = 0; // індекс, для вигляду мигаючих крапок
volatile uint8_t x1, x2, x3, x4, y1, y2, y3, y4; //Для зсуву стовбця вниз
uint16_t adc_res = 500; // результат вимірювання АЦП
uint8_t oldsec, oldmin;// секунди попередні
bit oldsec_flag, oldmin_flag, bmp_show, mess_show;
uint8_t brg_type;// яскравість по датчику, чи постійна
uint8_t brig;// значення яскравості
uint8_t usart_data[EUSART_RX_BUFFER_SIZE];
uint8_t blk_dot = 0; // дозвіл на мигання кнопок
extern uint8_t en_h_snd; // чи можна генерувати сигнал
extern uint8_t h_snd_t; //година співпадає з дозволом
extern uint8_t snd_flag; // один раз відтворювати
uint8_t en_ds_1;    //  чи пок. температуру з датчика 1
uint8_t en_ds_2;    //  чи пок. температуру з датчика 2
uint8_t en_bmp280; //  чи показуємо тиск
uint8_t en_dst; // перехід на літній час
uint8_t count_min = 0; // лічильник пройдених хвилин
uint8_t day_mess = 0; // день в який буде виводитись повідомлення
uint8_t dst_flag = 0; // чи зараз літній час????
uint8_t const compile_date[12]   = __DATE__;     // Mmm dd yyyy
uint8_t const compile_time[9]    = __TIME__;     // hh:mm:ss
 



__EEPROM_DATA(4, 2, 1, 2, 1, 1, 1, 1); // ініціалізація еепром, 
// 0 - тип шрифту (від 1 до 5)
// 1 - тип годинника (1,2)
// 2 - тип яскравості(0, 1). 1 - автоматично, 0 - ручний
// 3 - значеняя яскравості для ручного
// 4 - щогодинний сигнал
// 5 - чи показувати температуру з датчика 1 (кімнатний)
// 6 - чи показувати температуру з датчика 2 (радіодатчик)
// 7 - чи показувати атмосферний тиск
__EEPROM_DATA(1, 2, 0, 0, 0, 0, 0, 0); // ініціалізація еепром, (слідуючі комірки пам'яті) 
// 0 - автоматичний перехід на літній час
// 1 - тип показу температури
// читаємо з DS3231 години, хвилини, секунди та дату
// 
void GetTime(void)
{
    oldsec = TTime.Ts;
    oldmin = TTime.Tmin;
    getTime(&TTime.Thr, &TTime.Tmin, &TTime.Ts);
    getDate(&TTime.Tdy,&TTime.Tdt,&TTime.Tmt,&TTime.Tyr);
    if (oldsec != TTime.Ts)
        oldsec_flag = 1;
    if (oldmin != TTime.Tmin)
        oldmin_flag = 1;
    if (TTime.Ts == 3)
        snd_flag = 1; //дозволяємо знову генерувати щогодинний сигнал

    if (en_dst)
        dst_time(&TTime, &dst_flag);
    else
        dst_flag = 0; // вимкнути признак літнього часу

    if ((TTime.Thr >= 7)&&(TTime.Thr <= 23)&&(TTime.Tmin == 0)&&(TTime.Ts == 0)&&(snd_flag))
        h_snd_t = 1; //щогодинний сигнал
}


//**************************************************
//      температура з кімнатного датчика
//*************************************************
 void home_temp(void)
 {
    switch (events)
    {
        case MAIN_EVENT:
         //   if (readTemp_Single(&temperature, &time_flag, &timer_val))
         //   {
                clear_matrix();
                if (!((temperature/10) % 10)) // якщо перша цифра 0
                {
                    pic_to_led(3,1, &Dis_Buff);
                  //  putchar_down(13,(temperature/10) % 10 + 48);
                    putchar_down(13,temperature % 10, pFont);
                    putchar_down(19,176, &Font);
                } else {
                pic_to_led(3, 1, &Dis_Buff);
                putchar_down(13, (temperature / 10) % 10, pFont);
                putchar_down(19, temperature % 10, pFont);
                putchar_down(25, 176, &Font);

            }
            events = TEMP_EVENT;
            RTOS_SetTask(default_state, 650, 0); // 3,5 секунд для виходу

            // }

            break;
        case TEMP_EVENT:
            break;
        case KEY_EXIT_EVENT: // повертаємось в показ часу
            events = MAIN_EVENT;
            Rand_ef(); // випадковий ефект
            //scroll_right();
            //pre_ref_dis();
            RTOS_DeleteTask(default_state);
            RTOS_DeleteTask(home_temp);
            if (en_ds_2)
                RTOS_SetTask(radio_temp, 0, cycle_main);
            else {
                RTOS_SetTask(time_led, 0, cycle_main);
                pre_ref_dis();
            }
            break;

    }
}

//**************************************************
//      температура з радіо датчика
//*************************************************

void radio_temp(void) {
    uint8_t fptmp, i;

    switch (events) {
        case MAIN_EVENT:
            clear_matrix();
            if (err_ds18) {
                pic_to_led(0, 2, &Dis_Buff);
                putchar_down(9, 'E', &Font);
                putchar_down(15, 'r', &Font);
                putchar_down(21, 'r', &Font);
            } else {
                       // temperature_radio = 98; //&&&&&&&&&&&&&&&&&??????
                       // minus_radio = '+';
                if (temperature_radio != 0xFFFF) {

                    if ((type_temp == TYPE_TEMP_1) || (temperature_radio == 0)) {
                        fptmp = temperature_radio % 10; // остача
                        temperature_radio = temperature_radio / 10; // ціла частина
                        if (fptmp >= 6) temperature_radio += 1;

                        if (!((temperature_radio / 10) % 10)) // якщо перша цифра 0
                        {
                            pic_to_led(3, 2, &Dis_Buff);
                            //  putchar_down(13,(temperature/10) % 10 + 48);
                            if (temperature_radio != 0) {
                                putchar_down(13, minus_radio, &Font);
                                putchar_down(19, temperature_radio % 10, pFont);
                                putchar_down(25, 176, &Font);
                            } else {
                                //putchar_down(9, 0, &Font);
                                putchar_down(13, temperature_radio % 10, pFont);
                                putchar_down(19, 176, &Font);
                            }
                        } else {

                            pic_to_led(0, 2, &Dis_Buff);

                            putchar_down(9, minus_radio, &Font);
                            putchar_down(15, (temperature_radio / 10) % 10, pFont);
                            putchar_down(21, temperature_radio % 10, pFont);
                            putchar_down(27, 176, &Font);

                        }
                    } else { // якщо тип 2
                        for (i = 32; i < BUF_SIZE + BUF_SIZE_TEMP; i++)
                            Dis_Buff[i] = 0;
                        sprintf(text_buf, "%u%u%c%u%c", (temperature_radio / 100) % 10, (temperature_radio / 10) % 10, '.', temperature_radio % 10, '°'); // формуємо строку

                        if (text_buf[0] != 48) { //якщо перша цифра не 0
                            pic_to_led(3, 2, &Dis_Buff);
                            putchar_down(11, minus_radio, &Font);
                            putchar_down(17, text_buf[0] - 48, pFont); // цифра 1 - сотні
                            putchar_down(23, text_buf[1] - 48, pFont); // цифра 2 - десятки
                            putchar_down(29, text_buf[2], &Font);
                            //putchar_down(32, text_buf[3] - 48, pFont);
                            putchar_b_buf(32, text_buf[3] - 48, pFont, &Dis_Buff); // цифра 3 
                            putchar_b_buf(38, text_buf[4], &Font, &Dis_Buff);
                            __delay_ms(1000);
                            scroll_text_temp(10);
                        } else {
                            pic_to_led(1, 2, &Dis_Buff);
                            putchar_down(11, minus_radio, &Font);
                            putchar_down(17, text_buf[1] - 48, pFont); // цифра 1 - десятки
                            putchar_down(23, text_buf[2]  , &Font); // крапка
                            putchar_down(26, text_buf[3] - 48  , pFont); // цифра 2 
                            putchar_b_buf(32, text_buf[4], &Font, &Dis_Buff);//знак градуса
                            __delay_ms(1000);
                            scroll_text_temp(8);
                        }

                    }
                } else {
                    pic_to_led(0, 2, &Dis_Buff);

                    putchar_down(9, '-', &Font);
                    putchar_down(15, '-', &Font);
                    putchar_down(21, '-', &Font);
                }
            }
            events = TEMP_EVENT;
            RTOS_SetTask(default_state, 650, 0); // 3,5 секунд для виходу

            // }

            break;
        case TEMP_EVENT:
            break;
        case KEY_EXIT_EVENT: // повертаємось в показ часу
            events = MAIN_EVENT;
                       
            Rand_ef(); // випадковий ефект
            //hide_two_side();
            //scroll_left();
            pre_ref_dis();
            RTOS_DeleteTask(default_state);
            RTOS_DeleteTask(radio_temp);
            RTOS_SetTask(time_led, 0, cycle_main);
            // clear_matrix();
            break;
    }
}


//=====================================================
//   Заміна шрифту
//=====================================================

void set_font() {
    switch (type_font) {
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
//  вивід атмосферного тиску
// =================================================

void pressure(void) {
    uint16_t pr;

    switch (events) {
        case MAIN_EVENT:
           // press = 75126;
            if (press) {
                pr = press / 100;
                fill_buff_t(pr);
                center_two_side();
//                pic_to_led(3, 4, &Dis_Buff);
//                putchar_down(11, (pr / 100) % 10, pFont);
//                putchar_down(17, (pr / 10) % 10, pFont);
//                putchar_down(23, pr % 10, pFont);
                

            } else {
                clear_matrix();
                pic_to_led(3, 4, &Dis_Buff);
                putchar_down(11, 'E', &Font);
                putchar_down(17, 'R', &Font);
                putchar_down(23, 'R', &Font);

            }

            events = TEMP_EVENT;
            RTOS_SetTask(default_state, 400, 0); // 3,5 секунд для виходу

            // }

            break;
        case TEMP_EVENT:
            break;
        case KEY_EXIT_EVENT: // повертаємось в показ часу
            events = MAIN_EVENT;
            //
            if (press) {
                sprintf(text_buf, "мм.рт.ст.");
                interval_scroll_text(&text_buf);
            } else
                        //scroll_right();
            Rand_ef(); // випадковий ефект
                //    scroll_left();
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
            if (en_dst)
                dst_time(&TTime, &dst_flag);
            blk_dot = 0;
            break;
        case TYPE_CLK_2:
            if ((TTime.Thr / 10) % 10)
                putchar_down(1, (TTime.Thr / 10) % 10, pFont);
            else putchar_down(1, 0, &Font);
            putchar_down(7, TTime.Thr % 10, pFont);
            putchar_down(13, ':', &Font);
            getTime(&TTime.Thr, &TTime.Tmin, &TTime.Ts);
            putchar_down(19, (TTime.Tmin / 10) % 10, pFont);
            putchar_down(25, TTime.Tmin % 10, pFont);
            if (en_dst)
                dst_time(&TTime, &dst_flag);
            blk_dot = 1;
            idx_pnt = 0;
            x1 = (TTime.Thr / 10) % 10;
            x2 = TTime.Thr % 10;
            x3 = (TTime.Tmin / 10) % 10;
            x4 = TTime.Tmin % 10;

            break;

    }

}


//==================================================
//  виводимо годину на дисплей -:)
//==================================================

void time_led() {
    uint8_t data_array[4];


    switch (events) {
        case MAIN_EVENT:
            FillBuf(type_clk);
            if (type_clk == TYPE_CLK_2)
                blk_dot = 1;
            else
                blk_dot = 0;
            if ((TTime.Ts > 5)&&(TTime.Ts < 7)) //прочитаємо температуру
            {
                readTemp_Single(&temperature, &minus, &time_flag, &timer_val);
            }
            if (((TTime.Ts > 14)&&(TTime.Ts < 16)))// ||((TTime.Ts>45)&&(TTime.Ts<47)))    //  виведемо температуру
                events = KEY_DOWN_EVENT;
            if (en_bmp280) // якщо можна виводити атм. тиск
                if (((TTime.Ts > 39)&&(TTime.Ts < 41)&&((TTime.Tmin % 2) == 0))){    //  виведемо атмосферний тиск
                    events = KEY_UP_EVENT;
                  //  bmp_show = 0;
                }
//            if (oldmin_flag) { // пройшла хвилина
//                count_min++; // збільшуємо лічильник хвилин
//                oldmin_flag = 0;
//            }
//            if(count_min == 2){
//                count_min = 0;
//                bmp_show = 1;
//            }
            break;
        case KEY_OK_EVENT: // якщо натиснули кнопку ОК
            RTOS_DeleteTask(time_led); // видаляємо задачу в якій сидимо
            RTOS_SetTask(time_set_min, 0, 50); //  ставимо задачу налаштування годинника
            RTOS_SetTask(default_state, 2000, 0); // 10 секунд для виходу
            TSTime = TTime;
            events = MAIN_EVENT;
            en_put = 0;
            if (type_clk == TYPE_CLK_2){
                blk_dot = 0;
                putchar_b_buf(13, 23, &Font, &Dis_Buff);
            }
            //clear_matrix();
            break;
        case KEY_UP_EVENT:
            //         asm("nop");
            blk_dot = 0;
            bmp280Convert(&press, &temperbmp280);
            if (type_clk == TYPE_CLK_2)
                putchar_b_buf(13, 23, &Font, &Dis_Buff);
            //scroll_right();
            Rand_ef(); // випадковий ефект
            RTOS_DeleteTask(time_led); //видаляємо задачу
            RTOS_SetTask(pressure, 0, cycle_main); //додаємо задачу 
            events = MAIN_EVENT;
            en_put = 0;

            break;
        case KEY_DOWN_EVENT:
            if (en_ds_1) {
                blk_dot = 0;
                temperature_radio = data_array[1] | (uint16_t) (data_array[2] << 8);
                if (type_clk == TYPE_CLK_2)
                    putchar_b_buf(13, 23, &Font, &Dis_Buff);
                //scroll_left();
                //dissolve();
                //scroll_down_one();
                Rand_ef();
                RTOS_DeleteTask(time_led); //видаляємо задачу
                RTOS_SetTask(home_temp, 0, cycle_main); //додаємо задачу 
                events = MAIN_EVENT;
                en_put = 0;
            } else if (en_ds_2) {
                blk_dot = 0;
                if (type_clk == TYPE_CLK_2)
                    putchar_b_buf(13, 23, &Font, &Dis_Buff);
                //scroll_left();
                //dissolve();
                Rand_ef();
                RTOS_DeleteTask(time_led); //видаляємо задачу
                RTOS_SetTask(home_temp, 0, cycle_main); //додаємо задачу 
                events = MAIN_EVENT;
                en_put = 0;
            }
            events = MAIN_EVENT;
            break;
        case KEY_EXIT_EVENT:
            events = MAIN_EVENT;
            RTOS_DeleteTask(default_state);
            //           test = 0 + rand() % 10;
            //           printf("Tst = %u", test);
            //nrf24_init(120, 4);
            //#ifdef DEBUG
            //            __delay_ms(10);
            //            //printf("Test_NRF =  %u\n\r", nrf24_read_reg(NRF24_RX_ADDR_P0));
            //            //printf("Test_NRF_Chan =  %u\n\r", nrf24_read_reg(NRF24_RF_CH));
            //
            //            if (nrf24_dataReady()) {
            //                test++;
            //                nrf24_getData(&data_array);
            //                printf("%u > ", test);
            //                printf("%u ", data_array[0]);
            //                printf("%c ", data_array[1]);
            //                printf("%2X ", data_array[2]);
            //                printf("%2X\r\n", data_array[3]);
            //            }
            //#endif  
            //            break;
        case TEMP_EVENT:

            break;
    }
    if (en_put)
        Update_Matrix(Dis_Buff); // обновити дані на дисплеї

    // яскравість
    if (brg_type) {
        if ((TTime.Ts % 2 == 0)&&(oldsec_flag)) {
            oldsec_flag = 0;
            read_adc(); // прочитаємо дані з ацп
            adj_brig(); //  регулюємо яскравість
        }

    }
    en_put = 1;

    // читаємо радіодатчик
    if (nrf24_dataReady()) {
        nrf24_getData(&data_array);
        //spi_rw(FLUSH_RX); // очистити прийомний буфер
        nrf24_powerUpRx();
        temperature_radio = data_array[1] | (uint16_t) (data_array[2] << 8);
        minus_radio = data_array[0];
        err_ds_count = 0;
        err_ds18 = 0;
    } else
        err_ds_count++;


    if (err_ds_count > 1000) // чекаємо ~1.6 хвилини. Якщо не було ні одного зчитування
    {
        err_ds18 = 1; // то ставимо признак помилки радіодатчика
        nrf24_powerUpRx(); // Переводимо датчик у режим прийому, та скидаємо всі переривання
        nrf24_init(120, 4); // Ще раз ініціалізуємо
    }
    if (TTime.Tdt == day_mess) { // будемо виводити строку. Наприклад - привітання.
        if (((TTime.Tmin % 5) == 0) && (TTime.Ts == 35) && mess_show) { // один раз в 5 хвилин
            blk_dot = 0;
            putchar_b_buf(13, 23, &Font, &Dis_Buff);
            interval_scroll_text(&rs_text_buf);
            blk_dot = 1;
        }
    } else
        mess_show = 0;

    //#ifdef DEBUG
    //            if (nrf24_dataReady()) {
    //                test++;
    //                nrf24_getData(&data_array);
    //                printf("%u > ", test);
    //                printf("%u ", data_array[0]);
    //                printf("%c ", data_array[1]);
    //                printf("%2X ", data_array[2]);
    //                printf("%2X\r\n", data_array[3]);
    //            }
    //#endif  


}

//=======================================================
//          Робота з ком портом
//=======================================================

void usart_r() {

    static uint8_t i = 0;
    uint8_t j;
    
    if (EUSART_DataReady) {

        usart_data[i++] = EUSART_Read();
        //        if (sizeof (usart_data) <= EUSART_DataReady)
        //            eusartRxCount = 0;
        return;
    }
    if ((usart_data[0] == '$')) {
        switch (usart_data[1]) {
            case 't': // налаштування через синій зуб годин
                // формат "$tHHMM"
                TSTime.Thr = ((usart_data[2] - 48)*10)+(usart_data[3] - 48);
                TSTime.Tmin = ((usart_data[4] - 48)*10)+(usart_data[5] - 48);
                if (dst_flag) {// якщо час літній
                    if (TSTime.Thr == 0)
                        setTime(23, TSTime.Tmin, 0);
                    else
                        setTime((TSTime.Thr - 1), TSTime.Tmin, 0);
                }
                else
                    setTime(TSTime.Thr, TSTime.Tmin, 0);
                // setTime(TSTime.Thr, TSTime.Tmin, 0);
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'c': // тип годинника
                        // формат - ""$cN; N - тип, 1, або 2
                if ((usart_data[2] - 48 == 1) || (usart_data[2] - 48 == 2)) {
                    type_clk = usart_data[2] - 48;
                    write_eep(EE_TYPE_CLK, type_clk);
                    clear_matrix();
                    if (type_clk == 1)
                        blk_dot = 0;
                    else
                        blk_dot = 1;
                    idx_pnt = 0;
                    EUSART_Write('O');
                    EUSART_Write('K');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                } else {
                    EUSART_Write('E');
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');

                }

                break;
            case 'f': // шрифт
                    // формат - $fN  N - номер шрифту від 1 до 5
                switch (usart_data[2] - 48) {
                    case 1:
                        type_font = 1;
                        write_eep(EE_FONT, type_font);
                        set_font();
                        break;
                    case 2:
                        type_font = 2;
                        write_eep(EE_FONT, type_font);
                        set_font();
                        break;
                    case 3:
                        type_font = 3;
                        write_eep(EE_FONT, type_font);
                        set_font();
                        break;
                    case 4:
                        type_font = 4;
                        write_eep(EE_FONT, type_font);
                        set_font();
                        break;
                    case 5:
                        type_font = 5;
                        write_eep(EE_FONT, type_font);
                        set_font();
                        break;
                }
                if (((usart_data[2] - 48) > 5) || (usart_data[2] - 48) == 0) {
                    EUSART_Write('E'); // якщо не вірні дані
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                } else {
                    EUSART_Write('O');
                    EUSART_Write('K');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                }

                break;
            case 'd': // налаштування через синій зуб дата
                // формат "$dDYDTMNYR  DY - день тижня(1 - неділя), DT - число, MN - місяць, YR - рік "
                // наприклад - $d01270917 - неділя,27 вересня 17 року
                setDate((((usart_data[2] - 48)*10) + usart_data[3] - 48),
                        (((usart_data[4] - 48)*10) + usart_data[5] - 48),
                        (((usart_data[6] - 48)*10) + usart_data[7] - 48),
                        (((usart_data[8] - 48)*10)+(usart_data[9] - 48)));
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'b': // налаштування через синій зуб яскравості
                // формат "$bXN  X - тип(1 - автомат 0 - вручну, N - значення ручної - 1-8)
                // 
                if (((usart_data[2]-48) == 0) || ((usart_data[2]-48) == 1)) {
                    brg_type = usart_data[2] - 48;
                    brig = usart_data[3] - 48;
                    write_eep(EE_TYPE_BRG, brg_type);
                } else {
                    EUSART_Write('E');
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                    break;
                }
                if (brg_type == 0) {
                    write_eep(EE_DAT_BRG, brig);
                    Cmd7221(INTENSITY_R, brig); //Intensity Register
                }
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 's': // налаштування через синій зуб щогодинного сигналу
                // формат "$sX  X - тип(1 - є 0 - нема)
                // 
                if (((usart_data[2] - 48) == 0) || ((usart_data[2] - 48) == 1)) {
                    en_h_snd = usart_data[2] - 48;
                    write_eep(EE_EN_SND_H, en_h_snd);
                } else {
                    EUSART_Write('E');
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                    break;
                }
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'a': // налаштування через синій зуб показу датчиків температури
                // формат "$aXZ  XZ - датчики(X - кімнатний, Z - вуличний)(1 - показувати 0 - не показувати)
                // 
                if ((((usart_data[2] - 48) == 0) || ((usart_data[2] - 48) == 1)) && (((usart_data[3] - 48) == 0) || ((usart_data[3] - 48) == 1))) {
                    en_ds_1 = usart_data[2] - 48;
                    en_ds_2 = usart_data[3] - 48;
                    write_eep(EE_EN_DS1, en_ds_1);
                    write_eep(EE_EN_DS2, en_ds_2);
                } else {
                    EUSART_Write('E');
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                    break;
                }
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'e': // налаштування через синій зуб показу датчика тиску
                // формат "$eX  X - датчик(1 - показувати 0 - не показувати)
                // 
                if (((usart_data[2] - 48) == 0) || ((usart_data[2] - 48) == 1)) {
                    en_bmp280 = usart_data[2] - 48;
                    write_eep(EE_EN_BMP, en_bmp280);
                } else {
                    EUSART_Write('E');
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                    break;
                }
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'w': // налаштування через синій літнього часу
                // формат "$wX  X - 1 - літній час 0 - нема
                // 
                if (((usart_data[2] - 48) == 0) || ((usart_data[2] - 48) == 1)) {
                    en_dst = usart_data[2] - 48;
                    write_eep(EE_EN_DST, en_dst); // запишемо в еепром
                } else {
                    EUSART_Write('E');
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                    break;
                }
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'v': // налаштування через синій типу показу температури
                // формат "$vX  X - 1 - тип 1 2 - тип 2
                // 
                if (((usart_data[2] - 48) == TYPE_TEMP_1) || ((usart_data[2] - 48) == TYPE_TEMP_2)) {
                    type_temp = usart_data[2] - 48;
                    write_eep(EE_TYPE_TEMP, type_temp); // запишемо в еепром
                } else {
                    EUSART_Write('E');
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                    break;
                }
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'z': // reset controller
                // скидаємо контролер через 4 сек. після команди.
                // потрібно для bootloader

                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');
                __delay_ms(1000);
                __delay_ms(1000);
                __delay_ms(1000);
                __delay_ms(1000);
                asm("reset");

                break;
            case 'q': // виводимо біг строку температура
                // формат "$qt - температура, якщо є дозвіл на показ датчиків
                // формат "$qp - атмосферний тиск
                if (usart_data[2] == 't') {
                    events = KEY_DOWN_EVENT;  // показати температуру
                } else if (usart_data[2] == 'p') {
                    events = KEY_UP_EVENT;  // показати тиск
                } else {
                    EUSART_Write('E');
                    EUSART_Write('R');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                    break;
                }
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'S': // виводимо біг строку
                // формат "$Sxtext -  текст, до 100 символів
                // x - признак UTF8 or ASCII. u - utf8, a - ascii
                if (usart_data[2] == 'a') {
                    for (j = 0; j <= (strlen(usart_data)) - 3; j++)
                        rs_text_buf[j] = usart_data[j + 3];
                } else if (usart_data[2] == 'u')
                    convert_utf(&usart_data);
                else if (usart_data[2] == 'o') {
                    mess_show = 0; // вимикаэмо вивід строки
                    EUSART_Write('O');
                    EUSART_Write('K');
                    EUSART_Write('\r');
                    EUSART_Write('\n');
                    break;
                }
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                day_mess = TTime.Tdt;
                mess_show = 1;
                blk_dot = 0;
                RTOS_DeleteTask(home_temp); //видаляємо задачу
                RTOS_DeleteTask(radio_temp); //видаляємо задачу
                interval_scroll_text(&rs_text_buf);
                RTOS_SetTask(time_led, 0, cycle_main); //додаємо задачу
                blk_dot = 1;
                break;                 
                
            case 'r': // читаємо тестові значення
                // 
                switch (usart_data[2]) {
                    case 'a': // $ra - показати значеняя АЦП
                        EUSART_Write(((adc_res / 100) % 10) + 48); // передаємо першу цифру
                        EUSART_Write(((adc_res / 10) % 10) + 48); //......
                        EUSART_Write((adc_res % 10) + 48);
                        EUSART_Write('_');
                        break;
                    case 'v': // $rv - показати ID
                        //j = strlen(VERSION);
                        //for (j = 0; j <= (strlen(VERSION)) - 1; j++)
                        for (j = 0; j <= 11; j++)
                            EUSART_Write(compile_date[j]); // передаємо першу цифру
                        //  EUSART_Write(VERSION[j]); // передаємо першу цифру
                        EUSART_Write('\r');
                        EUSART_Write('\n');
                        for (j = 0; j <= 8; j++)
                            EUSART_Write(compile_time[j]); // передаємо першу цифру
                        //  EUSART_Write(VERSION[j]); // передаємо першу цифру
                        EUSART_Write('\r');
                        EUSART_Write('\n');
                        break;
                    case 't': // $ra - показати значеняя АЦП
                        EUSART_Write(minus_radio); // передаємо першу цифру
                        EUSART_Write(((temperature_radio / 100) % 10) + 48); // передаємо першу цифру
                        EUSART_Write(((temperature_radio / 10) % 10) + 48); //......
                        EUSART_Write((temperature_radio % 10) + 48);
                        EUSART_Write('_');
                        break;

                    default:
                        EUSART_Write('E');
                        EUSART_Write('R');
                        EUSART_Write('\r');
                        EUSART_Write('\n');
                        break;
                        
                }
                
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
        }
    }
    usart_data[0] = 0;
    usart_data[1] = 0;    
    usart_data[2] = 0;
    i = 0;
    reinit_rx();

}

//  вивід версії
void version(void)
{
    uint8_t i;

    sprintf(text_buf,"%s %s %s", VERSION, compile_date, compile_time); // формуємо строку
    interval_scroll_text(&text_buf);// виводимо біг. строку
    //    while(scroll_text())
    //    {
    //        Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
//        for(i=0; i<SPEED_STRING; i++)
//            __delay_ms(1);
//
//    };
}

//===========================================
// АЦП перетворення
//===========================================

void read_adc() {
    ADCON0bits.GO = 1; // Вимірюємо
    while (ADCON0bits.GO); // чекаємо закінчення перетворення
    adc_res = (ADRESH << 8) + ADRESL;
    asm("nop");    
    asm("nop");
#ifdef DEBUG

    printf("ADC_RESULT:%u\r\n", adc_res);
#endif  

}

//*****************************************
//  Перетворення кирилиці/ UTF8 --> ASCII
//*****************************************
void convert_utf(uint8_t *buf) {
    uint16_t temp;
    uint8_t i, j = 0;
    uint8_t len;

    len = (strlen(buf)) - 3; // довжина строки

    for (i = 0; i <= (len); i++) { // проходимо всю строку
        if (*(buf + j + 3) > 192) { // Якщо символ unicode
            temp = (uint16_t)(*(buf + j + 3) << 8) | *(buf + j + 3 + 1);
            if (temp == 53380)// літера Є
                temp -= 53210;
            else if (temp == 53382)// літера І
                temp -= 53204;
            else if (temp == 53383)// літера Ї
                temp -= 53208;
            else if (temp == 53652)// літера є
                temp -= 53466;
            else if (temp == 53654)// літера і
                temp -= 53475;
            else if (temp == 53655)// літера ї
                temp -= 53464;
            else if (temp <= 53439)
                temp = temp - 53200;
            else if ((temp >= 53632)&&(temp <= 53647))
                temp = temp - 53392;
            rs_text_buf[i] = (uint8_t) temp;
            j += 2;
            len -= 1;

        } else{
            rs_text_buf[i] = *(buf + j + 3);
            j++;
        }
    }

}


//===========================================
//          Яскравість матриці
//===========================================

void adj_brig() {

    if (adc_res >= 70)
        Cmd7221(INTENSITY_R, 0x03); //Intensity Register - 1/16
    else if (adc_res <= 50)
        Cmd7221(INTENSITY_R, 0x00); //Intensity Register - 2/16


    //        Cmd7221(INTENSITY_R, 0x00); //Intensity Register - 2/16
    //    if (adc_res >= 600)
    //        Cmd7221(INTENSITY_R, 0x04); //Intensity Register - 1/16
    //    else if (adc_res >= 450)
    //        Cmd7221(INTENSITY_R, 0x03); //Intensity Register - 2/16
    //    else if (adc_res >= 300)
    //        Cmd7221(INTENSITY_R, 0x02); //Intensity Register - 2/16
    //    else if (adc_res >= 150)
    //        Cmd7221(INTENSITY_R, 0x01); //Intensity Register - 2/16
    //    else if (adc_res <= 100)
    //        Cmd7221(INTENSITY_R, 0x00); //Intensity Register - 2/16

}




// функція переривання по входу RB0

void INT0_ISR(void) {
    if (blk_dot) {
        idx_pnt++;
        if (idx_pnt == 8) idx_pnt = 0;
        switch (idx_pnt) {
            case 0:
                Dis_Buff[15] = 0x36;
                Dis_Buff[16] = 0x36;
                break;
            case 1:
                Dis_Buff[15] = 0x24;
                Dis_Buff[16] = 0x36;
                break;
            case 2:
                Dis_Buff[15] = 0x24;
                Dis_Buff[16] = 0x24;
                break;
            case 3:
                Dis_Buff[15] = 0x24;
                Dis_Buff[16] = 0x00;
                break;
            case 4:
                Dis_Buff[15] = 0x00;
                Dis_Buff[16] = 0x00;
                break;
            case 5:
                Dis_Buff[15] = 0x12;
                Dis_Buff[16] = 0x00;
                break;
            case 6:
                Dis_Buff[15] = 0x12;
                Dis_Buff[16] = 0x12;
                break;
            case 7:
                Dis_Buff[15] = 0x12;
                Dis_Buff[16] = 0x36;
                break;
        }
    }
    //               putchar_b_buf(13, 23 + idx_pnt, &Font);
}
 
 
 // функція переривання таймер1

 void TMR1_ISR(void)
{

     
     if(++timer_val >= 150)      // затримка > 375мс
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
     
     //Update_Matrix(Dis_Buff);          // обновити дані на дисплеї
//        RTOS_SetTask(GetTime, 0, 0); // додаємо одноразовий запуск задачі в диспетчер
                                        // кожні 500мс.
}

