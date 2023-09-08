#include <avr/io.h>

#include "showTestInfo.h"
#include "testy.h"
#include "common.h"
#include "ds_GFX/ds_GFX.h"
#include "T6963C.h"
#include "ADC/adc.h"
#include "DS1Wire/ds_1wire.h"

enum testInfo { DEBUG, NORMAL };
uint8_t shoTest = NORMAL;

void ShowTestInfo(uint8_t zbNR, uint8_t test)
{
	if(shoTest == DEBUG)
		ShowDebug(zbNR, test);
	else if(shoTest == NORMAL)
		ShowNormal(zbNR, test);
}

void ShowDebug(uint8_t zbNR, uint8_t test)
{
	uint8_t xPos = zbNR*8;

	LCD_TextGoTo(xPos,0);
	LCD_WRiteString("        ");
	LCD_TextGoTo(xPos,0);
	LCD_WRiteString("TEST Z");
	LCD_WriteINT(zbNR+1);
	LCD_TextGoTo(xPos,1);
	LCD_WRiteString("        ");
	LCD_TextGoTo(xPos,1);
	LCD_WriteINT(ADC_CZUJNIK[zbNR]);
	LCD_WRiteString("|");
	LCD_WriteINT(zbRefPressure[zbNR]);
	LCD_TextGoTo(xPos,2);
	LCD_WRiteString("     ");
	LCD_TextGoTo(xPos,2);
	LCD_WriteINT(zbTimers[zbNR]);

	if(testResult[zbNR] == TESTING || testResult[zbNR] == FAILED)
	{
		LCD_TextGoTo(xPos,3);
		LCD_WRiteString("DIF:");
		LCD_TextGoTo(xPos+4,3);
		LCD_WRiteString("    ");
		LCD_TextGoTo(xPos+4,3);
		LCD_WriteINT(zbDifPressure[zbNR]);

		LCD_TextGoTo(xPos,5);
		LCD_WRiteString("        ");
		LCD_TextGoTo(xPos,5);
		if(zbErrorResult[zbNR] == 1)
			LCD_WRiteString("MINPRES ");
		else if(zbErrorResult[zbNR] == 2)
			LCD_WRiteString("MAXDROP ");
		else if(zbErrorResult[zbNR] == ERR_1W_NOPRESENCE)
			LCD_WRiteString("PRESENCE");
		else if(zbErrorResult[zbNR] == ERR_1W_COMPARE_DATA_FROM_SCRATCHPAD)
			LCD_WRiteString("COMPARE ");
		else
			LCD_WriteINT(zbErrorResult[zbNR]);
	}
	else
	{
		LCD_TextGoTo(xPos,3);
		LCD_WRiteString("        ");
		LCD_TextGoTo(xPos,5);
		LCD_WRiteString("        ");
	}

	LCD_TextGoTo(xPos,4);
	if(test != NONE && testResult[zbNR] == IDLE)
		LCD_WRiteString("STOPED  ");
	else if(test == NONE)
		LCD_WRiteString("        ");
	else if(test == HTR)
		LCD_WRiteString("HTR     ");
	else if(test == WOREK)
		LCD_WRiteString("WOREK   ");
	else if(test == ZBIORNIK)
		LCD_WRiteString("ZBIORNIK");
	else if(test == PODCISNIENIE)
		LCD_WRiteString("VACUM   ");

	LCD_TextGoTo(xPos,7);
	if(testResult[zbNR] == IDLE)
		LCD_WRiteString("IDLE    ");
	else if(testResult[zbNR] == TESTING)
		LCD_WRiteString("TESTING ");
	else if(testResult[zbNR] == FAILED)
		LCD_WRiteString("FAILED  ");
	else if(testResult[zbNR] == SUCCED || testResult[zbNR] == END)
		LCD_WRiteString("GOOD    ");

	if(testResult[zbNR] == TESTING)
	{
		uint16_t time = 0;
		uint16_t czasC = CZAS_CALKOWITY;

		if(test == WOREK)
			time = zbTimers[zbNR];
		else if(test == ZBIORNIK)
			time = CZAS_WOREK + zbTimers[zbNR];
		else if(test == PODCISNIENIE)
			time = CZAS_WOREK + CZAS_ZBIORNIK + zbTimers[zbNR];

		LCD_TextGoTo(xPos,6);
		LCD_WRiteString("        ");
		LCD_TextGoTo(xPos,6);
		LCD_WriteINT(((time+1)*100UL)/czasC);
		LCD_WRiteString("%");
	}
}

void ShowNormal(uint8_t zbNR, uint8_t test)
{
	uint8_t xPos = zbNR*8;

	LCD_TextGoTo(xPos,0);
	LCD_WRiteString("        ");
	LCD_TextGoTo(xPos,0);
	LCD_WRiteString("SLOT ");
	LCD_WriteINT(zbNR+1);

	if(testResult[zbNR] == TESTING || testResult[zbNR] == FAILED)
	{
		LCD_TextGoTo(xPos,1);
		LCD_WRiteString("DIF:");
		LCD_TextGoTo(xPos+4,1);
		LCD_WRiteString("    ");
		LCD_TextGoTo(xPos+4,1);
		LCD_WriteINT(zbDifPressure[zbNR]);

		LCD_TextGoTo(xPos,3);
		LCD_WRiteString("        ");
		LCD_TextGoTo(xPos,3);
		if(zbErrorResult[zbNR] == 1)
			LCD_WRiteString("MINPRES ");
		else if(zbErrorResult[zbNR] == 2)
			LCD_WRiteString("MAXDROP ");
	}
	else
	{
		LCD_TextGoTo(xPos,1);
		LCD_WRiteString("        ");
		LCD_TextGoTo(xPos,3);
		LCD_WRiteString("        ");
	}

	LCD_TextGoTo(xPos,2);
	if(test != NONE && testResult[zbNR] == IDLE)
		LCD_WRiteString("STOP    ");
	else if(test == NONE)
		LCD_WRiteString("        ");
	else if(test == HTR)
		LCD_WRiteString("HTR     ");
	else if(test == WOREK)
		LCD_WRiteString("WOREK   ");
	else if(test == ZBIORNIK)
		LCD_WRiteString("OBUDOWA ");
	else if(test == PODCISNIENIE)
		LCD_WRiteString("VACUM   ");

	LCD_TextGoTo(xPos,7);
	if(testResult[zbNR] == IDLE)
		LCD_WRiteString("CZEKAM  ");
	else if(testResult[zbNR] == TESTING)
		LCD_WRiteString("TESTUJE ");
	else if(testResult[zbNR] == FAILED)
		LCD_WRiteString("ZLY     ");
	else if(testResult[zbNR] == SUCCED || testResult[zbNR] == END)
		LCD_WRiteString("DOBRY   ");

	if(testResult[zbNR] == TESTING)
	{
		uint16_t time = 0;
		uint16_t czasC = CZAS_CALKOWITY;

		if(test == WOREK)
			time = zbTimers[zbNR];
		else if(test == ZBIORNIK)
			time = CZAS_WOREK + zbTimers[zbNR];
		else if(test == PODCISNIENIE)
			time = CZAS_WOREK + CZAS_ZBIORNIK + zbTimers[zbNR];

		LCD_TextGoTo(xPos,5);
		LCD_WRiteString("        ");
		LCD_TextGoTo(xPos+2,5);
		LCD_WriteINT(((time+1)*100UL)/czasC);
		LCD_WRiteString("%");

		uint16_t pasekLenght = 42;
		uint16_t plenght = ((((time+1)*100UL)/czasC) * pasekLenght) / 100;
		if(plenght < 1)
			plenght = 1;

		drawFastHLine(zbNR * 48,50,plenght,1);
		drawFastVLine(zbNR * 48,48,5,1);
		drawFastVLine((zbNR * 48)+42,48,5,1);
	}
	else
	{
		LCD_TextGoTo(xPos,5);
		LCD_WRiteString("        ");

		drawFastHLine(zbNR * 48,50,42,0);
		drawFastVLine(zbNR * 48,48,5,0);
		drawFastVLine((zbNR * 48)+42,48,5,0);
	}
}









