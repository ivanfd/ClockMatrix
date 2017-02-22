//====================================================
//       Налаштування I2C переферії
//
//====================================================



#include "i2c.h"


void I2C_Init(void)  // ініціалізація I2C
{
    TRISC |=  0b00011000;           // наналаштування порта
    SSPCON1 = 0b00101000;           // ввімкнути модуль MSSP, ведучий режим I2C
    SSPSTAT = 0b10000000;           // стандартниц режим 100кГц
    //SSPADD = 0x63;                  // частота 100кГц SSPADD = ((Fosc / Fclock) / 4) - 1
    SSPADD = 0x1F;                  // частота 312.5кГц SSPADD = ((Fosc / Fclock) / 4) - 1
    PEN = 1;                        // формуємо стопбіт P
    while (PEN);                    // чекаємо закінчення формування стоп біту
}


// чекати закінчення активності
void I2C_idle (void)
{
    while((SSPCON2 & 0x1F) || R_W);
}//

// Формування старт
// adress - адреса відомого
// _R_W - 0 - запис, 1 - читання
uint8_t I2C_start (uint8_t adress, uint8_t _R_W)
{
	I2C_idle();
	SEN = 1;    // формувати старт біт
	I2C_idle(); //  чекаємо не активності
	if(_R_W == 0) adress &= 0b11111110;	// якщо запис 
              else adress |= 0b00000001;	//якщо читання
	SSPBUF=adress;
	I2C_idle();
	if (ACKSTAT==0) return 0; 	// підтвердження від відомого отримано
	else
	{ PEN = 1; while (PEN); return 1;} // помилка
}


//_t повторний старт
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

//  запис байту в пристрій
uint8_t I2C_write (uint8_t data)	
{
    I2C_idle();
    SSPBUF = data;
    SSPIF = 0;
    I2C_idle();
    if (ACKSTAT == 0) return 0; // підтвердження від відомого отримано
    else
    { PEN = 1; while (PEN); return 1;} // помилка
}

// читаємо байт без підтвердження
uint8_t I2C_read_noack (void)	
{
	I2C_idle();
	RCEN = 1;				//Дозволити прийом даних
	I2C_idle();
	ACKDT = 1;              //set the MASTER NOACK bit
	ACKEN = 1;              //сформувати біт підтвердження
    //	I2C_idle();
	return  SSPBUF;         //читаємо буфер
}

// формування стоп біту
void I2C_stop (void)
{
	I2C_idle(); //
	PEN = 1;    //
	while(PEN); //
}//

