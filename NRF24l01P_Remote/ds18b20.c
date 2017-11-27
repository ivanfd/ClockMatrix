#include "ds18b20.h"


uint8_t scratch[9];



//===================================================
//      �����������
//===================================================
void init_ds18b20(void)
{

    if (!ow_reset())  // ���� ������ ��������
    {
        
        write_byte(0xCC);    
        write_byte(0x4E);    
        write_byte(0x00);    
        write_byte(0x00);    
        write_byte(0x7F); //12 -�� �����  
    }
}


//====================================================
//  ������ ����������� � �������
//  ���� ������ �� ���!
//====================================================

uint8_t readTemp_Single(uint8_t *buf, uint8_t *minus) {

    uint16_t temp = 0; // �����������
    uint8_t i, tmp, fptmp;

    *minus = '+';


    if (!ow_reset()) // ���� ������ ��������
    {
        write_byte(0xCC); //  ������� ������� ROM
        write_byte(0x44); //  ������������ �����������

    } else
        return FALSE;


    __delay_ms(750);
    ow_reset();
    write_byte(0xCC); //  ������� ������� ROM
    write_byte(0xBE); // Read scratch pad command
    for (i = 0; i < 2; i++)//  ������ ��� ����� �����������
    {
        scratch[i] = read_byte();
    }

    temp = (((uint16_t) scratch[1]) << 8) | ((uint16_t) scratch[0]);
    // temp |= scratch[0];

    if (temp & 0x8000) {
        temp = -temp; // ���� ��`���� 
        *minus = '-';
    }

    tmp = temp & 0x0f; // ����� ��������
    tmp = fract[tmp]; //  ����� ���� ����
    temp = temp >> 4; //  ������� ������ �����������
    temp = (temp & 0x00ff) * 10;
    temp = temp + tmp;

#ifdef DEBUG
    printf("Temperature: %d\r\n ", temp);
#endif

    fptmp = temp % 10; // ������
    temp = temp / 10; // ���� �������

    if (fptmp >= 6) temp += 1;
    //  temp = floor(temp);

    *buf = temp;
    return TRUE;


}




