
//-------------------------------------------------------------------------------------------------
// Graphic LCD with Toshiba T6963 controller
// Copyright (c) Radoslaw Kwiecien, 2007r , modyfikacja G. Zieba 09.2008 
// http://en.radzio.dxp.pl/t6963/
// Compiler : avr-gcc
//-------------------------------------------------------------------------------------------------
#include "T6963C.h"
#include "ports.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
//-------------------------------------------------------------------------------------------------
//
// Funkcja opoznienia
//	
//-------------------------------------------------------------------------------------------------
#define czas 3
void delay(void)
{
volatile unsigned char i;
for(i = 0; i < 3; i++)
  {
  asm("nop");
  }
}

//-------------------------------------------------------------------------------------------------
//
// Inicializacja portow LCD
//
//-------------------------------------------------------------------------------------------------
void LCD_InitalizeInterface(void)
{
LCD_DATA_DDR = 0xFF;

//ustawienie na wyjscia rejestry sterujace
DDRD=0xE0;
DDRG=0x03;

PORTD=0xE0;
PORTG=0x03;
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja odczytu statusu wyswietlacza
//
//-------------------------------------------------------------------------------------------------
unsigned char LCD_ChceckStatus(void)
{
uint8_t tmp;
LCD_DATA_DDR = 0x00;

//LCD_CTRL_PORT &= ~((1 << LCD_RD) | (1 << LCD_CE));
PORTD&=~_BV(5);
PORTG&=~_BV(0);

_delay_us(czas);
tmp = LCD_DATA_PIN;
LCD_DATA_DDR = 0xFF;

//LCD_CTRL_PORT |= ((1 << LCD_RD) | (1 << LCD_CE));
PORTD|=_BV(5);
PORTG|=_BV(0);
return tmp;
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wpisujaca instrukcje
//
//-------------------------------------------------------------------------------------------------
void LCD_WRiteCommand(unsigned char command)
{
while(!(LCD_ChceckStatus()&0x03));
LCD_DATA_PORT = command;

//LCD_CTRL_PORT &= ~((1 << LCD_WR) | (1 << LCD_CE));
PORTD&=~_BV(6);
PORTG&=~_BV(0);

_delay_us(czas);
//LCD_CTRL_PORT |= ((1 << LCD_WR) | (1 << LCD_CE));
PORTD|=_BV(6);
PORTG|=_BV(0);
}

//-------------------------------------------------------------------------------------------------
//
// Funkcja wpisujaca dane
//
//-------------------------------------------------------------------------------------------------
void LCD_WRiteData(unsigned char data)
{
while(!(LCD_ChceckStatus()&0x03));
LCD_DATA_PORT = data;

//LCD_CTRL_PORT &= ~((1 << LCD_WR) | (1 << LCD_CE) | (1 << LCD_CD));
PORTD&=~(_BV(6)|_BV(7));
PORTG&=~_BV(0);
//LCD_CD_PORT&=~_BV(LCD_CD);

_delay_us(czas);
//LCD_CTRL_PORT |= ((1 << LCD_WR) | (1 << LCD_CE) | (1 << LCD_CD));
PORTD|=_BV(6)|_BV(7);
PORTG|=_BV(0);
//LCD_CD_PORT|=_BV(LCD_CD);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja odczytu danych
//
//-------------------------------------------------------------------------------------------------
unsigned char LCD_ReadData(void)
{
uint8_t tmp;
while(!(LCD_ChceckStatus()&0x03));
LCD_DATA_DDR = 0x00;

//LCD_CTRL_PORT &= ~((1 << LCD_RD) | (1 << LCD_CE) | (1 << LCD_CD));
PORTD&=~(_BV(5)|_BV(7));
PORTG&=~_BV(0);
//LCD_CD_PORT&=~_BV(LCD_CD);

_delay_us(czas);
tmp = LCD_DATA_PIN;
//LCD_CTRL_PORT |= ((1 << LCD_RD) | (1 << LCD_CE) | (1 << LCD_CD));
PORTD|=_BV(5)|_BV(7);
PORTG|=_BV(0);
//LCD_CD_PORT|=_BV(LCD_CD);
LCD_DATA_DDR = 0xFF;
return tmp;
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia adresu w pamieci RAM
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetAddressPointer(unsigned int address)
{
LCD_WRiteData(address & 0xFF);
LCD_WRiteData(address >> 8);
LCD_WRiteCommand(T6963_SET_ADDRESS_POINTER);
}
//-------------------------------------------------------------------------------------------------
//
// Wyczyszczenie pamieci RAM wyswietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_ClearText(void)
{
int i;
GLCD_SetAddressPointer(GLCD_TEXT_HOME);

for(i = 0; i < GLCD_TEXT_SIZE; i++)
  {
  LCD_WRiteDisplayData(0);
  }
}

//-------------------------------------------------------------------------------------------------
//
// Wyczyszczenie pamieci RAM wyswietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_ClearTextBlock(char Asocket)
{
	int i,j;
	GLCD_SetAddressPointer(GLCD_TEXT_HOME);

	unsigned int address;	
	unsigned int base_block_addr = 8 * (Asocket-1);
	
	for(j = 2; j< 8;j++) //tutaj od 2 bo zeby nie znikala linia z wynikami
	{
		address = GLCD_TEXT_HOME + base_block_addr + (GLCD_TEXT_AREA * j);
		GLCD_SetAddressPointer(address);
		for(i = 0; i < 8; i++)
			LCD_WRiteDisplayData(0);
	}
}

//-------------------------------------------------------------------------------------------------
//
// Wyczyszczenie pamieci RAM generatora znakow
//
//-------------------------------------------------------------------------------------------------
void LCD_ClearCG(void)
{
unsigned int i;
GLCD_SetAddressPointer(GLCD_EXTERNAL_CG_HOME);

for(i = 0; i < 256 * 8; i++)
  {
  LCD_WRiteDisplayData(0);
  }
}
//-------------------------------------------------------------------------------------------------
//
// Wyczyszczenie pamieci RAM obszaru grafiki
//
//-------------------------------------------------------------------------------------------------
void LCD_ClearGraphic(void)
{
int i;
GLCD_SetAddressPointer(GLCD_GRAPHIC_HOME);
for(i = 0; i < GLCD_GRAPHIC_SIZE; i++)
  {
  LCD_WRiteDisplayData(0x00);
  }
}
//-------------------------------------------------------------------------------------------------
//
// Wpisanie pojedynczego znaku (ASCII code)do pamieci RAM
//
//-------------------------------------------------------------------------------------------------
void LCD_WRiteChar(char charCode)
{
LCD_WRiteDisplayData(charCode - 32);
}
//-------------------------------------------------------------------------------------------------
//
// Wpisanie stringa do pamieci RAM
//
//-------------------------------------------------------------------------------------------------
void LCD_WRiteString(char * string)
{
while(*string)
  {
  LCD_WRiteChar(*string++);
  }
}

// INT
void LCD_WriteINT(int data)
{
	char buf[16];
	LCD_WRiteString(itoa( data, buf, 10 ));
}

//-------------------------------------------------------------------------------------------------
//
// Wpisanie stringa z pamieci programu do RAM
//
//-------------------------------------------------------------------------------------------------
void LCD_WRiteStringPgm_p(const char *string)
{
char ch;
while((ch = pgm_read_byte(string++)))
  {
  LCD_WRiteChar(ch);
  }
}
//-------------------------------------------------------------------------------------------------
//
// Ustawienie wspolzednych wyswietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_TextGoTo(unsigned char x, unsigned char y)
{
unsigned int address;

address = GLCD_TEXT_HOME +  x + (GLCD_TEXT_AREA * y);

GLCD_SetAddressPointer(address);
}
//-------------------------------------------------------------------------------------------------
//
// Wpisanie pojedynczego znaku do generatora znakow w RAM
//
//-------------------------------------------------------------------------------------------------
void LCD_DefineCharacter(unsigned char charCode, unsigned char * defChar)
{
unsigned int address;
unsigned char i; 

address = GLCD_EXTERNAL_CG_HOME + (8 * charCode);

GLCD_SetAddressPointer(address);

for(i = 0; i < 8 ; i++)
  {
  LCD_WRiteDisplayData(*(defChar + i));
  }
}
//-------------------------------------------------------------------------------------------------
//
// Ustawienie pisela (if color==1) lub (if color==0) wyczyszczenie piksela
//
//-------------------------------------------------------------------------------------------------
void LCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
unsigned char tmp;
unsigned int address;

address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);

GLCD_SetAddressPointer(address);

LCD_WRiteCommand(T6963_DATA_READ_AND_NONVARIABLE);
tmp = LCD_ReadData();

if(color)
  tmp |= (1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
else
 tmp &= ~(1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));

LCD_WRiteDisplayData(tmp);

}
//-------------------------------------------------------------------------------------------------
//
// Wpisanie danych i przesuniecie adresu
//
//-------------------------------------------------------------------------------------------------
void LCD_WRiteDisplayData(unsigned char x)
{
LCD_WRiteData(x);
LCD_WRiteCommand(T6963_DATA_WRITE_AND_INCREMENT);
}
//-------------------------------------------------------------------------------------------------
//
// Ustawienie wspolzednych obszaru grafiki
//
//-------------------------------------------------------------------------------------------------
void GLCD_GraphicGoTo(unsigned char x, unsigned char y)
{
unsigned int address;
address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
GLCD_SetAddressPointer(address);
}
//-------------------------------------------------------------------------------------------------
//
// Wyswietlenie bitmapy z pamieci
//
//-------------------------------------------------------------------------------------------------
void GLCD_Bitmap(unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
unsigned char i, j;

for(j = 0; j < height; j++)
{
GLCD_GraphicGoTo(x, y + j);
for(i = 0; i < width/GLCD_FONT_WIDTH; i++)
  {
  LCD_WRiteDisplayData(pgm_read_byte(bitmap + i + (GLCD_GRAPHIC_AREA * j))); 	
  }
}
}
//-------------------------------------------------------------------------------------------------
//
// Inicializacja wyswietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_Initalize(void)
{
LCD_InitalizeInterface();

//LCD_CTRL_PORT &= ~(1 << LCD_RST);
PORTG&=~_BV(1);
_delay_ms(1);
//LCD_CTRL_PORT |= (1 << LCD_RST);
PORTG|=_BV(1);

LCD_WRiteData(GLCD_GRAPHIC_HOME & 0xFF);
LCD_WRiteData(GLCD_GRAPHIC_HOME >> 8);
LCD_WRiteCommand(T6963_SET_GRAPHIC_HOME_ADDRESS);

LCD_WRiteData(GLCD_GRAPHIC_AREA);
LCD_WRiteData(0x00);
LCD_WRiteCommand(T6963_SET_GRAPHIC_AREA);

LCD_WRiteData(GLCD_TEXT_HOME);
LCD_WRiteData(GLCD_TEXT_HOME >> 8);
LCD_WRiteCommand(T6963_SET_TEXT_HOME_ADDRESS);

LCD_WRiteData(GLCD_TEXT_AREA);
LCD_WRiteData(0x00);
LCD_WRiteCommand(T6963_SET_TEXT_AREA);

LCD_WRiteData(GLCD_OFFSET_REGISTER);
LCD_WRiteData(0x00);
LCD_WRiteCommand(T6963_SET_OFFSET_REGISTER);

//LCD_WRiteCommand(T6963_DISPLAY_MODE | T6963_TEXT_DISPLAY_ON /*| T6963_CURSOR_DISPLAY_ON*/);
LCD_WRiteCommand(T6963_DISPLAY_MODE  | T6963_GRAPHIC_DISPLAY_ON   | T6963_TEXT_DISPLAY_ON /*| T6963_CURSOR_DISPLAY_ON*/);

LCD_WRiteCommand(T6963_MODE_SET | 1);//tryb pracy EXOR

}
