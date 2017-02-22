//====================================================
//       ������������ I2C �������
//
//====================================================



#include "i2c.h"


void I2C_Init(void)  // ����������� I2C
{
    TRISC |=  0b00011000;           // �������������� �����
    SSPCON1 = 0b00101000;           // �������� ������ MSSP, ������� ����� I2C
    SSPSTAT = 0b10000000;           // ����������� ����� 100���
    //SSPADD = 0x63;                  // ������� 100��� SSPADD = ((Fosc / Fclock) / 4) - 1
    SSPADD = 0x1F;                  // ������� 312.5��� SSPADD = ((Fosc / Fclock) / 4) - 1
    PEN = 1;                        // ������� ������ P
    while (PEN);                    // ������ ��������� ���������� ���� ���
}


// ������ ��������� ���������
void I2C_idle (void)
{
    while((SSPCON2 & 0x1F) || R_W);
}//

// ���������� �����
// adress - ������ �������
// _R_W - 0 - �����, 1 - �������
uint8_t I2C_start (uint8_t adress, uint8_t _R_W)
{
	I2C_idle();
	SEN = 1;    // ��������� ����� ��
	I2C_idle(); //  ������ �� ���������
	if(_R_W == 0) adress &= 0b11111110;	// ���� ����� 
              else adress |= 0b00000001;	//���� �������
	SSPBUF=adress;
	I2C_idle();
	if (ACKSTAT==0) return 0; 	// ������������ �� ������� ��������
	else
	{ PEN = 1; while (PEN); return 1;} // �������
}


//_t ��������� �����
uint8_t I2C_restart (uint8_t adres, uint8_t _R_W)
{
	I2C_idle();
	RSEN = 1;
	I2C_idle();
	if(_R_W == 0) adres &= 0b11111110;	// 
	      else adres |= 0b00000001;	//
	SSPBUF = adres;
	I2C_idle();
	if (ACKSTAT == 0) return 0; // 
	else
	{ PEN = 1; while (PEN); return 1;} // 
}//

//  ����� ����� � �������
uint8_t I2C_write (uint8_t data)	
{
    I2C_idle();
    SSPBUF = data;
    SSPIF = 0;
    I2C_idle();
    if (ACKSTAT == 0) return 0; // ������������ �� ������� ��������
    else
    { PEN = 1; while (PEN); return 1;} // �������
}

// ������ ���� ��� ������������
uint8_t I2C_read_noack (void)	
{
	I2C_idle();
	RCEN = 1;				//��������� ������ �����
	I2C_idle();
	ACKDT = 1;              //set the MASTER NOACK bit
	ACKEN = 1;              //���������� �� ������������
    //	I2C_idle();
	return  SSPBUF;         //������ �����
}

// ���������� ���� ���
void I2C_stop (void)
{
	I2C_idle(); //
	PEN = 1;    //
	while(PEN); //
}//

