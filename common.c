#include "common.h"

extern uint8_t TxtBuf[6]; // ����� �� ����
extern uint8_t Dis_Buff[BUF_SIZE + BUF_SIZE_TEMP]; // ����� �������
extern uint8_t key_event; // ���� ������
extern uint8_t text_buf[50]; // ����� ��� �� ������
extern uint8_t rs_text_buf[100];
uint8_t events = MAIN_EVENT; // ����, ��� ����������
//uint8_t delay_digit = 0;            // ��� ����� ������� ����
bit show_digit; // �� ���������� �����, � ���. �������
bit en_put; // �� ����� ������ � ����� �������
uint8_t temperature; // ����������� � ��������� �������
uint16_t temperature_radio; // ����������� � ���� �������
bit err_ds18 = 0;// ������� ������� ����
uint16_t err_ds_count = 0;
uint8_t minus; // ���� �����������
uint8_t minus_radio = '+'; // ���� �����������
uint8_t timer_val = 0, time_flag = 0; // ��� ������������� �����������
extern uint8_t(*pFont)[][5];
uint8_t type_font = 0; // ����� �����
uint8_t type_temp = 2;
uint32_t press, temperbmp280; // ����������� ����
extern uint8_t play_sound; //  �� ����� ����������
uint8_t type_clk = TYPE_CLK_1; // ������ ���������
uint8_t idx_pnt = 0; // ������, ��� ������� �������� ������
volatile uint8_t x1, x2, x3, x4, y1, y2, y3, y4; //��� ����� ������� ����
uint16_t adc_res = 500; // ��������� ���������� ���
uint8_t oldsec, oldmin;// ������� ��������
bit oldsec_flag, oldmin_flag, bmp_show, mess_show;
uint8_t brg_type;// ��������� �� �������, �� �������
uint8_t brig;// �������� ���������
uint8_t usart_data[EUSART_RX_BUFFER_SIZE];
uint8_t blk_dot = 0; // ����� �� ������� ������
extern uint8_t en_h_snd; // �� ����� ���������� ������
extern uint8_t h_snd_t; //������ ������� � ��������
extern uint8_t snd_flag; // ���� ��� �����������
uint8_t en_ds_1;    //  �� ���. ����������� � ������� 1
uint8_t en_ds_2;    //  �� ���. ����������� � ������� 2
uint8_t en_bmp280; //  �� �������� ����
uint8_t en_dst; // ������� �� ���� ���
uint8_t count_min = 0; // �������� ��������� ������
uint8_t day_mess = 0; // ���� � ���� ���� ���������� �����������
uint8_t dst_flag = 0; // �� ����� ���� ���????
uint8_t const compile_date[12]   = __DATE__;     // Mmm dd yyyy
uint8_t const compile_time[9]    = __TIME__;     // hh:mm:ss
 



__EEPROM_DATA(4, 2, 1, 2, 1, 1, 1, 1); // ����������� ������, 
// 0 - ��� ������ (�� 1 �� 5)
// 1 - ��� ��������� (1,2)
// 2 - ��� ���������(0, 1). 1 - �����������, 0 - ������
// 3 - �������� ��������� ��� �������
// 4 - ���������� ������
// 5 - �� ���������� ����������� � ������� 1 (��������)
// 6 - �� ���������� ����������� � ������� 2 (����������)
// 7 - �� ���������� ����������� ����
__EEPROM_DATA(1, 2, 0, 0, 0, 0, 0, 0); // ����������� ������, (������� ������ ���'��) 
// 0 - ������������ ������� �� ���� ���
// 1 - ��� ������ �����������
// ������ � DS3231 ������, �������, ������� �� ����
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
        snd_flag = 1; //���������� ����� ���������� ���������� ������

    if (en_dst)
        dst_time(&TTime, &dst_flag);
    else
        dst_flag = 0; // �������� ������� ������� ����

    if ((TTime.Thr >= 7)&&(TTime.Thr <= 23)&&(TTime.Tmin == 0)&&(TTime.Ts == 0)&&(snd_flag))
        h_snd_t = 1; //���������� ������
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
            RTOS_SetTask(default_state, 650, 0); // 3,5 ������ ��� ������

            // }

            break;
        case TEMP_EVENT:
            break;
        case KEY_EXIT_EVENT: // ����������� � ����� ����
            events = MAIN_EVENT;
            Rand_ef(); // ���������� �����
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
//      ����������� � ���� �������
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
                        fptmp = temperature_radio % 10; // ������
                        temperature_radio = temperature_radio / 10; // ���� �������
                        if (fptmp >= 6) temperature_radio += 1;

                        if (!((temperature_radio / 10) % 10)) // ���� ����� ����� 0
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
                    } else { // ���� ��� 2
                        for (i = 32; i < BUF_SIZE + BUF_SIZE_TEMP; i++)
                            Dis_Buff[i] = 0;
                        sprintf(text_buf, "%u%u%c%u%c", (temperature_radio / 100) % 10, (temperature_radio / 10) % 10, '.', temperature_radio % 10, '�'); // ������� ������

                        if (text_buf[0] != 48) { //���� ����� ����� �� 0
                            pic_to_led(3, 2, &Dis_Buff);
                            putchar_down(11, minus_radio, &Font);
                            putchar_down(17, text_buf[0] - 48, pFont); // ����� 1 - ����
                            putchar_down(23, text_buf[1] - 48, pFont); // ����� 2 - �������
                            putchar_down(29, text_buf[2], &Font);
                            //putchar_down(32, text_buf[3] - 48, pFont);
                            putchar_b_buf(32, text_buf[3] - 48, pFont, &Dis_Buff); // ����� 3 
                            putchar_b_buf(38, text_buf[4], &Font, &Dis_Buff);
                            __delay_ms(1000);
                            scroll_text_temp(10);
                        } else {
                            pic_to_led(1, 2, &Dis_Buff);
                            putchar_down(11, minus_radio, &Font);
                            putchar_down(17, text_buf[1] - 48, pFont); // ����� 1 - �������
                            putchar_down(23, text_buf[2]  , &Font); // ������
                            putchar_down(26, text_buf[3] - 48  , pFont); // ����� 2 
                            putchar_b_buf(32, text_buf[4], &Font, &Dis_Buff);//���� �������
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
            RTOS_SetTask(default_state, 650, 0); // 3,5 ������ ��� ������

            // }

            break;
        case TEMP_EVENT:
            break;
        case KEY_EXIT_EVENT: // ����������� � ����� ����
            events = MAIN_EVENT;
                       
            Rand_ef(); // ���������� �����
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
//   ����� ������
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
//  ���� ������������ �����
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
                interval_scroll_text(&text_buf);
            } else
                        //scroll_right();
            Rand_ef(); // ���������� �����
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
//  �������� ������ �� ������� -:)
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
            if ((TTime.Ts > 5)&&(TTime.Ts < 7)) //��������� �����������
            {
                readTemp_Single(&temperature, &minus, &time_flag, &timer_val);
            }
            if (((TTime.Ts > 14)&&(TTime.Ts < 16)))// ||((TTime.Ts>45)&&(TTime.Ts<47)))    //  �������� �����������
                events = KEY_DOWN_EVENT;
            if (en_bmp280) // ���� ����� �������� ���. ����
                if (((TTime.Ts > 39)&&(TTime.Ts < 41)&&((TTime.Tmin % 2) == 0))){    //  �������� ����������� ����
                    events = KEY_UP_EVENT;
                  //  bmp_show = 0;
                }
//            if (oldmin_flag) { // ������� �������
//                count_min++; // �������� �������� ������
//                oldmin_flag = 0;
//            }
//            if(count_min == 2){
//                count_min = 0;
//                bmp_show = 1;
//            }
            break;
        case KEY_OK_EVENT: // ���� ��������� ������ ��
            RTOS_DeleteTask(time_led); // ��������� ������ � ��� ������
            RTOS_SetTask(time_set_min, 0, 50); //  ������� ������ ������������ ���������
            RTOS_SetTask(default_state, 2000, 0); // 10 ������ ��� ������
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
            Rand_ef(); // ���������� �����
            RTOS_DeleteTask(time_led); //��������� ������
            RTOS_SetTask(pressure, 0, cycle_main); //������ ������ 
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
                RTOS_DeleteTask(time_led); //��������� ������
                RTOS_SetTask(home_temp, 0, cycle_main); //������ ������ 
                events = MAIN_EVENT;
                en_put = 0;
            } else if (en_ds_2) {
                blk_dot = 0;
                if (type_clk == TYPE_CLK_2)
                    putchar_b_buf(13, 23, &Font, &Dis_Buff);
                //scroll_left();
                //dissolve();
                Rand_ef();
                RTOS_DeleteTask(time_led); //��������� ������
                RTOS_SetTask(home_temp, 0, cycle_main); //������ ������ 
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
        Update_Matrix(Dis_Buff); // �������� ��� �� ������

    // ���������
    if (brg_type) {
        if ((TTime.Ts % 2 == 0)&&(oldsec_flag)) {
            oldsec_flag = 0;
            read_adc(); // ��������� ��� � ���
            adj_brig(); //  ��������� ���������
        }

    }
    en_put = 1;

    // ������ ����������
    if (nrf24_dataReady()) {
        nrf24_getData(&data_array);
        //spi_rw(FLUSH_RX); // �������� ��������� �����
        nrf24_powerUpRx();
        temperature_radio = data_array[1] | (uint16_t) (data_array[2] << 8);
        minus_radio = data_array[0];
        err_ds_count = 0;
        err_ds18 = 0;
    } else
        err_ds_count++;


    if (err_ds_count > 1000) // ������ ~1.6 �������. ���� �� ���� � ������ ����������
    {
        err_ds18 = 1; // �� ������� ������� ������� �����������
        nrf24_powerUpRx(); // ���������� ������ � ����� �������, �� ������� �� �����������
        nrf24_init(120, 4); // �� ��� ����������
    }
    if (TTime.Tdt == day_mess) { // ������ �������� ������. ��������� - ���������.
        if (((TTime.Tmin % 5) == 0) && (TTime.Ts == 35) && mess_show) { // ���� ��� � 5 ������
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
//          ������ � ��� ������
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
            case 't': // ������������ ����� ���� ��� �����
                // ������ "$tHHMM"
                TSTime.Thr = ((usart_data[2] - 48)*10)+(usart_data[3] - 48);
                TSTime.Tmin = ((usart_data[4] - 48)*10)+(usart_data[5] - 48);
                if (dst_flag) {// ���� ��� ����
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
            case 'c': // ��� ���������
                        // ������ - ""$cN; N - ���, 1, ��� 2
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
            case 'f': // �����
                    // ������ - $fN  N - ����� ������ �� 1 �� 5
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
                    EUSART_Write('E'); // ���� �� ��� ���
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
            case 'd': // ������������ ����� ���� ��� ����
                // ������ "$dDYDTMNYR  DY - ���� �����(1 - �����), DT - �����, MN - �����, YR - �� "
                // ��������� - $d01270917 - �����,27 ������� 17 ����
                setDate((((usart_data[2] - 48)*10) + usart_data[3] - 48),
                        (((usart_data[4] - 48)*10) + usart_data[5] - 48),
                        (((usart_data[6] - 48)*10) + usart_data[7] - 48),
                        (((usart_data[8] - 48)*10)+(usart_data[9] - 48)));
                EUSART_Write('O');
                EUSART_Write('K');
                EUSART_Write('\r');
                EUSART_Write('\n');

                break;
            case 'b': // ������������ ����� ���� ��� ���������
                // ������ "$bXN  X - ���(1 - ������� 0 - ������, N - �������� ����� - 1-8)
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
            case 's': // ������������ ����� ���� ��� ����������� �������
                // ������ "$sX  X - ���(1 - � 0 - ����)
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
            case 'a': // ������������ ����� ���� ��� ������ ������� �����������
                // ������ "$aXZ  XZ - �������(X - ��������, Z - ��������)(1 - ���������� 0 - �� ����������)
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
            case 'e': // ������������ ����� ���� ��� ������ ������� �����
                // ������ "$eX  X - ������(1 - ���������� 0 - �� ����������)
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
            case 'w': // ������������ ����� ���� ������� ����
                // ������ "$wX  X - 1 - ���� ��� 0 - ����
                // 
                if (((usart_data[2] - 48) == 0) || ((usart_data[2] - 48) == 1)) {
                    en_dst = usart_data[2] - 48;
                    write_eep(EE_EN_DST, en_dst); // �������� � ������
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
            case 'v': // ������������ ����� ���� ���� ������ �����������
                // ������ "$vX  X - 1 - ��� 1 2 - ��� 2
                // 
                if (((usart_data[2] - 48) == TYPE_TEMP_1) || ((usart_data[2] - 48) == TYPE_TEMP_2)) {
                    type_temp = usart_data[2] - 48;
                    write_eep(EE_TYPE_TEMP, type_temp); // �������� � ������
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
                // ������� ��������� ����� 4 ���. ���� �������.
                // ������� ��� bootloader

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
            case 'q': // �������� �� ������ �����������
                // ������ "$qt - �����������, ���� � ����� �� ����� �������
                // ������ "$qp - ����������� ����
                if (usart_data[2] == 't') {
                    events = KEY_DOWN_EVENT;  // �������� �����������
                } else if (usart_data[2] == 'p') {
                    events = KEY_UP_EVENT;  // �������� ����
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
            case 'S': // �������� �� ������
                // ������ "$Sxtext -  �����, �� 100 �������
                // x - ������� UTF8 or ASCII. u - utf8, a - ascii
                if (usart_data[2] == 'a') {
                    for (j = 0; j <= (strlen(usart_data)) - 3; j++)
                        rs_text_buf[j] = usart_data[j + 3];
                } else if (usart_data[2] == 'u')
                    convert_utf(&usart_data);
                else if (usart_data[2] == 'o') {
                    mess_show = 0; // ��������� ���� ������
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
                RTOS_DeleteTask(home_temp); //��������� ������
                RTOS_DeleteTask(radio_temp); //��������� ������
                interval_scroll_text(&rs_text_buf);
                RTOS_SetTask(time_led, 0, cycle_main); //������ ������
                blk_dot = 1;
                break;                 
                
            case 'r': // ������ ������ ��������
                // 
                switch (usart_data[2]) {
                    case 'a': // $ra - �������� �������� ���
                        EUSART_Write(((adc_res / 100) % 10) + 48); // �������� ����� �����
                        EUSART_Write(((adc_res / 10) % 10) + 48); //......
                        EUSART_Write((adc_res % 10) + 48);
                        EUSART_Write('_');
                        break;
                    case 'v': // $rv - �������� ID
                        //j = strlen(VERSION);
                        //for (j = 0; j <= (strlen(VERSION)) - 1; j++)
                        for (j = 0; j <= 11; j++)
                            EUSART_Write(compile_date[j]); // �������� ����� �����
                        //  EUSART_Write(VERSION[j]); // �������� ����� �����
                        EUSART_Write('\r');
                        EUSART_Write('\n');
                        for (j = 0; j <= 8; j++)
                            EUSART_Write(compile_time[j]); // �������� ����� �����
                        //  EUSART_Write(VERSION[j]); // �������� ����� �����
                        EUSART_Write('\r');
                        EUSART_Write('\n');
                        break;
                    case 't': // $ra - �������� �������� ���
                        EUSART_Write(minus_radio); // �������� ����� �����
                        EUSART_Write(((temperature_radio / 100) % 10) + 48); // �������� ����� �����
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

//  ���� ����
void version(void)
{
    uint8_t i;

    sprintf(text_buf,"%s %s %s", VERSION, compile_date, compile_time); // ������� ������
    interval_scroll_text(&text_buf);// �������� ��. ������
    //    while(scroll_text())
    //    {
    //        Update_Matrix(Dis_Buff);          // �������� ��� �� ������
//        for(i=0; i<SPEED_STRING; i++)
//            __delay_ms(1);
//
//    };
}

//===========================================
// ��� ������������
//===========================================

void read_adc() {
    ADCON0bits.GO = 1; // ��������
    while (ADCON0bits.GO); // ������ ��������� ������������
    adc_res = (ADRESH << 8) + ADRESL;
    asm("nop");    
    asm("nop");
#ifdef DEBUG

    printf("ADC_RESULT:%u\r\n", adc_res);
#endif  

}

//*****************************************
//  ������������ ��������/ UTF8 --> ASCII
//*****************************************
void convert_utf(uint8_t *buf) {
    uint16_t temp;
    uint8_t i, j = 0;
    uint8_t len;

    len = (strlen(buf)) - 3; // ������� ������

    for (i = 0; i <= (len); i++) { // ��������� ��� ������
        if (*(buf + j + 3) > 192) { // ���� ������ unicode
            temp = (uint16_t)(*(buf + j + 3) << 8) | *(buf + j + 3 + 1);
            if (temp == 53380)// ����� �
                temp -= 53210;
            else if (temp == 53382)// ����� �
                temp -= 53204;
            else if (temp == 53383)// ����� �
                temp -= 53208;
            else if (temp == 53652)// ����� �
                temp -= 53466;
            else if (temp == 53654)// ����� �
                temp -= 53475;
            else if (temp == 53655)// ����� �
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
//          ��������� �������
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




// ������� ����������� �� ����� RB0

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

