#include "max7221.h"

//*************************************
// �������� ����� � ������� MAX7221
//*************************************
void Send_Byte_7221(uint8_t dat)
   {
      uint8_t i;
      
         for (i = 0; i < 8; i++)
         {
               if (dat & 0x80)
                  DAT_MAX = 1;
               else
                  DAT_MAX = 0;
                  CLK_MAX = 1;
                  asm("nop");
                  asm("nop");
//                  asm("nop");
//                  asm("nop");
//                  asm("nop");
//                  asm("nop");
                  CLK_MAX = 0;
                  dat <<= 1;   
         }
      

   }

//*************************************
// �������� ������� � ������� MAX7221
//*************************************
void Cmd7221(uint8_t adr, uint8_t val )
   {
      uint8_t i;
      CS_MAX = 0;
      for (i = 0; i < COUNT_MATRIX; i++ )
      {
         Send_Byte_7221(adr);
         Send_Byte_7221(val);
      
      }

       CS_MAX = 1;     

   }
   
//*************************************
//     ��������� ����� �� ������
//*************************************

void Update_Matrix(uint8_t *buf) {
    uint8_t i, j, data;
    

    for (i = 0; i < 8; i++) // ��������� 8 �� �������� (8 DIG)
    { // 
        CS_MAX = 0; // 
        for (j = 0; j < COUNT_MATRIX; j++) // ��������� �� ����� ������� 
        {
            data = buf[8 * (COUNT_MATRIX - 1 - j) + i]; // ��� � �������, �������� �� ������� ������

            Send_Byte_7221(1 + i); // �������� ������
            Send_Byte_7221(data); // �������� ���
        }
        CS_MAX = 1;
    }

}
  
  
//*************************************
//       ����������� MAX7221
//*************************************

void Init7221()
   {
      CS_MAX = 1;

      Cmd7221(SHUTDOWN_R,1);    //Shutdown Register - Normal Mode
      Cmd7221(DECODE_R,0);      //Decode-Mode Register - No decode for digits 7�0
      Cmd7221(SCAN_R,7);        //Scan-Limit Register - Display digits 0 1 2 3 4 5 6 7
      Cmd7221(INTENSITY_R,0x01);//Intensity Register - 5/16
      Cmd7221(TEST_R,0);        //test - Normal Operation
   }
