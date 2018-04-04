
#include "display.h"

uint8_t Dis_Buff[BUF_SIZE + BUF_SIZE_TEMP]; // ����� �������
uint8_t Dis_Buff_t[BUF_SIZE]; // ����� ������� temp
//uint8_t TxtBuf[6];          // ����� �� ����
uint8_t text_buf[50];      // ����� ��� �� ������
uint8_t rs_text_buf[100]; // ����� ��� �� ������ � ��������
uint8_t i_char, i_bchar;    // ������ ����� �� ���� � ����
uint8_t(*pFont)[][5] = &dFont1; // �������� �� ����� ���� ��� ������ �����     
extern uint8_t idx_pnt;
extern volatile uint8_t x1 = 0, x2 = 0, x3 = 0, x4 = 0, y1, y2, y3, y4; //��� ����� ������� ����
extern uint8_t blk_dot; // ����� �� ������� ������

const uint8_t dissolve_arr[16] = {253,191,239,127,251,223,247,254,253,191,239,127,251,223,247,254}; //����� ��� ������ ���������
//uint8_t Hours, Min, Sec;

static void (*function) (void); // �������� �� �������
//void (*p_MyFunc[4])(void) = {dissolve, scroll_left, scroll_right, hide_two_side};
const p_MyFunc my_func[5] = {dissolve, scroll_left, scroll_right, hide_two_side, scroll_down_one}; //����� ��������� �� �������
//*****************************************
//       �������� ������ �� �������
//*****************************************
void pixel_on(uint8_t x, uint8_t y, uint8_t *buf)
{
    *(buf+x) |= (1<<y);    
    //Dis_Buff[x] |= (1<<y);
}

//*****************************************
//      �������� ������ �� �������
//*****************************************
void pixel_off(uint8_t x, uint8_t y, uint8_t *buf)
{
   *(buf+x) &= ~(1<<y);
    //Dis_Buff[x] &= ~(1<<y);
}

//**********************************************
//  ���� �������� �� ������� � ������� x
//**********************************************

void pic_to_led(uint8_t x, uint8_t pic, uint8_t *buf) {
    uint8_t i, j, mask = 0x01;

    for (i = 0; i < 7; i++) { //��������
        for (j = 0; j < 8; j++) { //������ 
            if (pic7x8[pic][i] & mask) {
                pixel_on(x + i, j, buf);
            }
            else {
                pixel_off(x + i, j, buf);
            };
            mask = mask << 1;
        };
        mask = 0x01;
    };
}

//***********************************************
//�������� ������ � �������� ������� �� �������
//***********************************************
void putchar_b_buf(uint8_t x, uint8_t symbol, uint8_t (*pF)[][5], uint8_t *buf)
{
uint8_t i, j, mask=0x01;

    
    for(i=0; i<5; i++)  // ������ ��������, ����� 5�8
    {        
        for(j=0;j<8;j++)//������ ����� - 8��
        {      
            if((*pF)[symbol] [i] & mask) // ���� ��������� �� �� = 0
                pixel_on(x+i,j,buf);        //�������� ������
            else                        //���� �
                pixel_off(x+i,j,buf);       //������
                
                mask = mask<<1;         // 
        };
        //������� �� 8 �� �������, ���������� �� ���������
    mask=0x01; 
    };    
}

//*******************************************************
//�������� ������� ����� � �������� ������� �� �������
//*******************************************************
void putchar_s_buf(uint8_t x, uint8_t symbol)
{
uint8_t i, j, mask=0x01;

symbol -= 47; 
    for(i=0; i<3; i++)  // ������ ��������,��������� ����� 3�8
    {        
        for(j=0;j<8;j++)//������ ����� - 8��
        {      
            if(FontS[symbol] [i] & mask) // ���� ��������� �� �� = 0
                pixel_on(x+i,j,&Dis_Buff);        //�������� ������
            else                        //���� �
                pixel_off(x+i,j,&Dis_Buff);       //������
                
                mask = mask<<1;         // 
        };
        //������� �� 8 �� �������, ���������� �� ���������
    mask=0x01; 
    };    
}

//*************************************************************
//�������� ������ � �������� ������� �� ������� � ������ ����
//*************************************************************
void putchar_down(uint8_t x, uint8_t symbol,uint8_t (*pF)[][5])
{
uint8_t i, j, k;

  for(j=0;j<=7;j++)     //��������� �� 8 �����
  {
    for(i=0;i<5;i++)    // ������ ���� �� 1 �� ������
    {
        Dis_Buff[x+i] = (Dis_Buff[x+i]<<1) | ((*pF)[symbol][i] >> (8-1-j));
    }
    
    for(k=0;k<DELAY_SHIFT_DOWN;k++)
        __delay_ms(1);
                Update_Matrix(Dis_Buff);          // �������� ��� �� ������
    
  }
    
}

//***********************************************************************
//�������� ��������� ������ � �������� ������� �� ������� � ������ ����
//***********************************************************************
void putchar_down_s(uint8_t x, uint8_t symbol)
{
uint8_t i, j, k;

//if(symbol != 0)
 //   symbol -= 47;
    
  for(j=0;j<=7;j++)     //��������� �� 8 �����
  {
    for(i=0;i<3;i++)    // ������ ���� �� 1 �� ������
    {
        Dis_Buff[x+i] = (Dis_Buff[x+i]<<1) | (FontS[symbol][i] >> (8-1-j));
    }
    
    for(k=0;k<DELAY_SHIFT_DOWN;k++)
        __delay_ms(1);
                Update_Matrix(Dis_Buff);          // �������� ��� �� ������
    
  }
    
}

//********************
//   �������� �������
//********************
void clear_matrix(void)
{
   uint8_t i;
   for (i=0; i<=BUF_SIZE; i++)
       Dis_Buff[i] = 0;
}


//***********************************
// ���������� ������ ������� ������
//***********************************

void FillBuf(uint8_t type) {
    uint8_t i;
    switch (type) {
        case TYPE_CLK_1:
            blk_dot = 0;
            for (i = 0; i < 5; ++i) {
                if ((TTime.Thr / 10) % 10)
                    Dis_Buff[i] = (*pFont)[(TTime.Thr / 10) % 10][i];
                else
                    Dis_Buff[i] = 0x00;


                Dis_Buff[i + 6] = (*pFont)[TTime.Thr % 10 ][i];
                Dis_Buff[i + 13] = (*pFont)[(TTime.Tmin / 10) % 10 ][i];
                Dis_Buff[i + 19] = (*pFont)[TTime.Tmin % 10 ][i];

            }

            for (i = 0; i < 3; ++i) {
                Dis_Buff[i + 25] = FontS[(TTime.Ts / 10) % 10 + 1][i];
                Dis_Buff[i + 29] = FontS[TTime.Ts % 10 + 1][i];
            }

            Dis_Buff[5] = 0;
            Dis_Buff[11] = 0x80;
            Dis_Buff[12] = 0x80;
            Dis_Buff[18] = 0;
            Dis_Buff[24] = 0;
            Dis_Buff[28] = 0;
            break;
        case TYPE_CLK_2:// ���� ��� ��������� 2
            blk_dot = 1;
            y1 = (TTime.Thr / 10) % 10;
            y2 = TTime.Thr % 10;
            y3 = (TTime.Tmin / 10) % 10;
            y4 = TTime.Tmin % 10;
            //���� ���� � ���� ��������
            if ((x1 != y1) || (x2 != y2) || (x3 != y3) || (x4 != y4)) {
                //putchar_b_buf(13, ':', &Font);
                if (x4 != y4)
                    putchar_down(25, TTime.Tmin % 10, pFont);
                else
                    putchar_b_buf(25, TTime.Tmin % 10, pFont, &Dis_Buff);

                if (x3 != y3)
                    putchar_down(19, (TTime.Tmin / 10) % 10, pFont);
                else
                    putchar_b_buf(19, (TTime.Tmin / 10) % 10, pFont, &Dis_Buff);

                if (x2 != y2)
                    putchar_down(7, TTime.Thr % 10, pFont);
                else
                    putchar_b_buf(7, TTime.Thr % 10, pFont, &Dis_Buff);

                if ((TTime.Thr / 10) % 10) {
                    if (x1 != y1)
                        putchar_down(1, (TTime.Thr / 10) % 10, pFont);
                    else
                        putchar_b_buf(1, (TTime.Thr / 10) % 10, pFont, &Dis_Buff);

                } else
                    putchar_down(1, 0, &Font);
                // putchar_b_buf(13, 23 + idx_pnt, &Font);
                //idx_pnt = 0;


                x1 = y1;
                x2 = y2;
                x3 = y3;
                x4 = y4;

            } else {
                if ((TTime.Thr / 10) % 10)
                    putchar_b_buf(1, (TTime.Thr / 10) % 10, pFont, &Dis_Buff);
                else
                    putchar_b_buf(1, 0, &Font, &Dis_Buff);
                putchar_b_buf(7, TTime.Thr % 10, pFont, &Dis_Buff);
               // putchar_b_buf(13, 23 + idx_pnt, &Font);
                putchar_b_buf(19, (TTime.Tmin / 10) % 10, pFont, &Dis_Buff);
                putchar_b_buf(25, TTime.Tmin % 10, pFont, &Dis_Buff);
            }
            break;
    }


}

//================================================
//           ���������� ����� (5 ����) 
//  *buf - �������� �� ����� ����
//  edit_Flsg - ���� 1, �� �� 4-5-� ������ ���� �����������
//  scr_flag - ���� 1, �� ������� ������� ��� ������ ��������
//================================================

void FillBufS(uint8_t *buf, uint8_t edit_Flag, uint8_t scr_flag)
   {
      uint8_t i, j, d, nch = 0;
      d = 0;
      
      for (j=0;j<=4;++j)  
      {
         for (i=0;i<=4;++i)  
         {
            if((nch >= 3) && edit_Flag)
               Dis_Buff[d] = (Font[*(buf + j)] [i]) + 0x80;
            else
               Dis_Buff[d] = Font[*(buf + j)] [i];
            d++;
         }  

           Dis_Buff[d] = 0;
           d++;
           nch++;
         //}
      }
      if (scr_flag)
      {
         Dis_Buff[30] = Font[*(buf + 5)] [0];
         Dis_Buff[31] = Font[*(buf + 5)] [1];
      }
      else
      {
         Dis_Buff[30] = 0;
         Dis_Buff[31] = 0;
      }
   }


//*******************************
//   ���� ������� �����
//*******************************

uint8_t scroll_text(uint8_t *buf) {
    uint8_t i;

    for (i = 0; i < 31; i++) // ������� �� ����� �� ������� ���� �� 1 ��
        Dis_Buff[i] = Dis_Buff[i + 1];

    if ((i_bchar) >= 5) // ���� ������ ���� ������
    {
        if (*(buf + i_char) == 0) // ���� ������ ����������
        {
            i_bchar++;
            if (i_bchar >= 32) // ���� ������ ����� �� ���� ������
            {
                i_char = 0;
                i_bchar = 0;
                return 0; // ��������
            }
        }
        else {
            Dis_Buff[31] = 0; // �������� �����
            i_bchar = 0;
            i_char++; // ��������� ���� (�����) ������
        }
    } else {
        Dis_Buff[31] = Font[ *(buf + i_char)][i_bchar++]; // � 31 ������� ���� �������

    }



    return 1;
    
}

//******************************************
//   ���� ������� ����� � ������������
//******************************************

void scroll_text_temp(uint8_t pos) {
    uint8_t i, j, k;

    for (k = 0; k <= pos; k++) {
        for (i = 0; i < 31; i++) // ������� �� ����� �� ������� ���� �� 1 ��
            Dis_Buff[i] = Dis_Buff[i + 1];

        Dis_Buff[31] = Dis_Buff[32 + k ]; // � 31 ������� ���� �������
        Update_Matrix(Dis_Buff); // �������� ��� �� ������
        for (j = 0; j < SPEED_STRING; j++)
            __delay_ms(1);
    }
    return;
    
}

//********************************************
//  ������� ����� ������ �����
//********************************************
void scroll_left(void)
{
    uint8_t i,j,k,speed = 100;
   
    
    for (k=0;k<=31;k++)
    {
        for (i=0; i<=31; i++)
            Dis_Buff[i] = Dis_Buff[i+1];

        Dis_Buff[31] = 0;
        Update_Matrix(Dis_Buff);          // �������� ��� �� ������
        for(j=0; j<speed; j++)
                __delay_ms(1);
        if (speed > 10) speed -=10;   

    }     
    
    
}

//********************************************
//  ������� ����� ���� �������
//********************************************
void scroll_right(void)
{
    uint8_t i,j,k,speed = 100;


    for (k = 0; k <= 31; k++) {

        for (i = 31; i > 0; i--)
            Dis_Buff[i] = Dis_Buff[i - 1];

        Dis_Buff[0] = 0;
        Update_Matrix(Dis_Buff); // �������� ��� �� ������
        for (j = 0; j < speed; j++)
            __delay_ms(1);
        if (speed > 10) speed -= 10;

    }
    
    
}

//===============================================
//      �������� ������ ��� ��. ������
//===============================================
void interval_scroll_text(uint8_t *buf)
{
  uint8_t i;
        
while(scroll_text(buf))
    {
        Update_Matrix(Dis_Buff);          // �������� ��� �� ������
        for(i=0; i<SPEED_STRING; i++)
            __delay_ms(1);

    };    
}

//**************************************************
//             ����� �������� � ���� ����� :-)
//**************************************************

void hide_two_side(void) {
    uint8_t a = 0, b = 31;
    uint8_t i, j;

    for (i = 0; i <= 15; i++) {
        Dis_Buff[a++] = 0;
        Dis_Buff[b--] = 0;
        Update_Matrix(Dis_Buff); // �������� ��� �� ������
        for (j = 0; j < 30; j++) // �����
            __delay_ms(1);

    }
}


//**************************************************
//             ����� ��������� :-)
//**************************************************

void dissolve(void) {
    uint8_t i, j, n = 0, z, k;
    
    
    for (z = 0; z <= 7; z++) {
        for (i = 0; i <= 31; i++) {
            Dis_Buff[i] &= dissolve_arr[n + z];
            n++;
            if (n > 7)
                n = 0;
        }
        Update_Matrix(Dis_Buff); // �������� ��� �� ������
        for (k = 0; k < 100; k++) // �����
            __delay_ms(1);
    }
}


//**************************************************
//             ����� �������� ���� :-)
//**************************************************

void scroll_down_one(void) {
    uint8_t i, j, k;
    

    for (i = 0; i <= 31; i++) {
        if (Dis_Buff[i] == 0)
            continue;
        for (j = 0; j <= 7; j++) {
            //Dis_Buff[x+i] = (Dis_Buff[x+i]<<1) | ((*pF)[symbol][i] >> (8-1-j));
            Dis_Buff[i] = Dis_Buff[i] << 1;
            Update_Matrix(Dis_Buff); // �������� ��� �� ������
            for (k = 0; k < 10; k++) // �����
                __delay_ms(1);
        }

    }
}


//=========================================
//  ���� ����������� ������
//=========================================
void Rand_ef(void) {
    static uint8_t eff = 0;
    static uint8_t old_eff = 0;

    while (old_eff == eff) {
        eff = (0 + rand() % 5);
    }
    old_eff = eff;
    function = my_func[eff];
    (*function)();                               // �������� ������
}

//*******************************************
//  ���������� ����������� ������ ������
//*******************************************
void fill_buff_t(uint16_t data){
    uint8_t i, j;


    for (i = 0; i <= BUF_SIZE; i++)
        Dis_Buff_t[i] = 0;

    pic_to_led(3, 4, &Dis_Buff_t);
    putchar_b_buf(11, (data / 100) % 10, pFont, &Dis_Buff_t);
    putchar_b_buf(17, (data / 10) % 10, pFont, &Dis_Buff_t);
    putchar_b_buf(23, data % 10, pFont, &Dis_Buff_t);

}

//*******************************************
//  ����� ������ ������ ���������
//*******************************************
void center_two_side(void){
    uint8_t i, k, shiftr = 16, shiftl = 15;

    for (i = 0; i <= 15; i++) {
        Dis_Buff[shiftr++] = Dis_Buff_t[shiftr];
        Dis_Buff[shiftl--] = Dis_Buff_t[shiftl];


        Update_Matrix(Dis_Buff); // �������� ��� �� ������
        for (k = 0; k < 30; k++) // �����
            __delay_ms(1);
    }
}