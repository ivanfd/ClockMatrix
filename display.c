
#include "display.h"

uint8_t Dis_Buff[BUF_SIZE]; // ����� �������
uint8_t TxtBuf[6];          // ����� �� ����
uint8_t text_buf[100];      // ����� ��� �� ������
uint8_t i_char, i_bchar;    // ������ ����� �� ����� � ����
//uint8_t Hours, Min, Sec;

//*****************************************
//       �������� ������ �� �������
//*****************************************
void pixel_on(uint8_t x, uint8_t y)
{
    Dis_Buff[x] |= (1<<y);
}

//*****************************************
//      �������� ������ �� �������
//*****************************************
void pixel_off(uint8_t x, uint8_t y)
{
    Dis_Buff[x] &= ~(1<<y);
}

//**********************************************
//  ���� �������� �� ������� � ������� x
//**********************************************
void pic_to_led(uint8_t x, uint8_t pic)
{
uint8_t i, j, mask=0x01;

 for(i=0; i<7; i++){            //��������
        for(j=0;j<8;j++){       //������ 
            if(pic7x8[pic][i] & mask){  
                pixel_on(x+i,j);
                } 
                else {                    
                pixel_off(x+i,j);         
                };
        mask = mask<<1;  
        };
    mask=0x01; 
 };   
}

//***********************************************
//�������� ������ � �������� ������� �� �������
//***********************************************
void putchar_b_buf(uint8_t x, uint8_t symbol)
{
uint8_t i, j, mask=0x01;

    
    for(i=0; i<5; i++)  // ������ ��������, ����� 5�8
    {        
        for(j=0;j<8;j++)//������ ����� - 8��
        {      
            if(Font[symbol] [i] & mask) // ���� ��������� �� �� = 0
                pixel_on(x+i,j);        //�������� ������
            else                        //���� �
                pixel_off(x+i,j);       //������
                
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
                pixel_on(x+i,j);        //�������� ������
            else                        //���� �
                pixel_off(x+i,j);       //������
                
                mask = mask<<1;         // 
        };
        //������� �� 8 �� �������, ���������� �� ���������
    mask=0x01; 
    };    
}

//*************************************************************
//�������� ������ � �������� ������� �� ������� � ������ ����
//*************************************************************
void putchar_down(uint8_t x, uint8_t symbol)
{
uint8_t i, j, k;

  for(j=0;j<=7;j++)     //��������� �� 8 �����
  {
    for(i=0;i<5;i++)    // ������ ���� �� 1 �� ������
    {
        Dis_Buff[x+i] = (Dis_Buff[x+i]<<1) | (Font[symbol][i] >> (8-1-j));
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
void FillBuf(void)
   {
      uint8_t i;
 
     for(i=0; i<5; ++i)
      {
        if((TTime.Thr/10) % 10)
         Dis_Buff[i] = Font[(TTime.Thr/10) % 10 + 48][i];
        else
         Dis_Buff[i] = 0x00;

         Dis_Buff[i + 6] = Font[TTime.Thr % 10 + 48][i];
         Dis_Buff[i + 13] = Font[(TTime.Tmin/10) % 10 + 48][i];
         Dis_Buff[i + 19] = Font[TTime.Tmin % 10 + 48][i];

      }
      
      for (i=0; i<3; ++i)
      {
         Dis_Buff[i + 25] = FontS[(TTime.Ts/10) % 10 + 1][i];   
         Dis_Buff[i + 29] = FontS[TTime.Ts % 10 + 1][i];   
      }

         Dis_Buff[5] = 0;
         Dis_Buff[11] = 0x80;
         Dis_Buff[12] = 0x80;
         Dis_Buff[18] = 0;
         Dis_Buff[24] = 0;
         Dis_Buff[28] = 0;
    

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
uint8_t scroll_text(void)
{
    uint8_t i;
    
        for (i=0; i<31; i++)            // ������� �� ����� �� ������� ���� �� 1 ��
         Dis_Buff[i] = Dis_Buff[i+1];

    if ((i_bchar) >= 5)      // ���� ������ ���� ������
      {
        if (text_buf[i_char] == 0)  // ���� ������ ����������
        {
            i_bchar++;
            if (i_bchar >= 32)      // ���� ������ ����� �� ���� ������
            {
                i_char = 0;
                i_bchar = 0;
                return 0;           // ��������
            }    
        }    
        else
        {
            Dis_Buff[31] = 0;   // �������� �����
            i_bchar = 0;
            i_char++;           // �������� ������
        }
    }
     else
     {
        Dis_Buff[31] = Font[ text_buf[i_char]][i_bchar++]; // � 31 ������� ���� �������

     }


    
    return 1;
    
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
    