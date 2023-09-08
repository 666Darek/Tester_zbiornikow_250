/*
 * initTests.c
 *
 *  Created on: 23 lip 2020
 *      Author: Dawid Sobków
 */

#include "initTests.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "ADC/adc.h"
#include "ds_GFX/ds_GFX.h"
#include "T6963C.h"
#include "common.h"

char isInitTestsDone = 0;	// Tu mo¿na w³¹czyc 0 lub wylaczyc intro z testem 1

// Pokazuje intro
void Intro()
{
	static char isIntroDone = 0;

	if(isIntroDone == 1)
		return;

	mk_ssd1306_puts(50, 7, "EBS", 7, 1, 0);
	mk_ssd1306_puts(176, 7, "r", 1, 1, 0);
	drawCircle(178,11,5,1);

	_delay_ms(1500);

	LCD_ClearText();
	LCD_ClearGraphic();
	LCD_TextGoTo(0,0);
	LCD_WRiteString("          TESTER ZBIORNIKOW"); // write text
	LCD_TextGoTo(0,2);
	LCD_WRiteString("     DATA KOMPILACJI 07.12.2022"); // write text
	LCD_TextGoTo(0,4);
	LCD_WRiteString("                   G.Zieba 2016"); // write text
	LCD_TextGoTo(0,6);
	LCD_WRiteString("                  D.Sobkow 2021"); // write text

	_delay_ms(3000);

	isIntroDone = 1;
}
// Test wstêpny
char InitTests()
{
	static char POS=0;
	static char TimerDelay = 0;

	if(isInitTestsDone == 1)
		return isInitTestsDone;

	if(POS == 0 && TimerDelay++ < 5)	// czekamy N czasu
	{
		LCD_ClearText();
		LCD_TextGoTo(0,3);
		LCD_WRiteString("             TEST NAPIECIA"); // write text
		return 0;
	}
	else if(POS == 0)
	{
		TimerDelay = 0;	// zerujemy licznik
		POS++;
	}

	//sprawdzenie napiecia zasilania
	if(POWER_DOWN==0)//gdy znacznik napiecia jest na poziomie logicznego '0'
	{
		LCD_TextGoTo(0,2);
		LCD_WRiteString("             BLAD ZASILACZA"); // write text
		LCD_TextGoTo(0,4);
		LCD_WRiteString("          ZMIEN ZASILACZ NA 24V"); // write text

		while(POWER_DOWN==0)//dopoki zasilanie nie powroci
		{
			//wlaczenie beeperka sygnal dzwiekowy
			BEEP_ERROR();
		}
		//gdy nastapi ustawienie prawidlowego napiecia zasilania (znacznik napiecia - mlogiczna '1')
		//	FALL_DOWN_FLAG=0;//ustawienie flagi na zbocze opadajace
		LCD_ClearText();
		LCD_TextGoTo(0,2);
		LCD_WRiteString("       USTAWIONO POPRAWNE NAPIECIE"); // write text
		LCD_TextGoTo(0,4);
		LCD_WRiteString("         ROZPOCZECIE TESTOWANIA"); // write text

		_delay_ms(1500);// BLOKUJE ##################################
	}

	if(POS == 1 && TimerDelay++ < 12)	// czekamy N czasu
	{
		LCD_ClearText();
		LCD_TextGoTo(0,3);
		LCD_WRiteString("         TEST DIOD (Czerwone)"); // write text
		//zapalenie diod do testu
		if(TimerDelay <= 2)
			R1_ON;

		if(TimerDelay > 2 && TimerDelay <= 4)
		{
			R1_OFF;
			R2_ON;
		}

		if(TimerDelay > 4 && TimerDelay <= 6)
		{
			R2_OFF;
			R3_ON;
		}

		if(TimerDelay > 6 && TimerDelay <= 8)
		{
			R3_OFF;
			R4_ON;
		}

		if(TimerDelay > 8 && TimerDelay <= 10)
		{
			R4_OFF;
			R5_ON;
		}
		//	R5_OFF;

		if(TimerDelay > 10)
		{
			R1_ON;
			R2_ON;
			R3_ON;
			R4_ON;
			R5_ON;
		}

		return 0;
	}
	else if(POS == 1)
	{
		R1_OFF;
		R2_OFF;
		R3_OFF;
		R4_OFF;
		R5_OFF;

		TimerDelay = 0;	// zerujemy licznik
		POS++;
	}

	if(POS == 2 && TimerDelay++ < 12)	// czekamy N czasu
	{
		LCD_ClearText();
		LCD_TextGoTo(0,3);
		LCD_WRiteString("         TEST DIOD (Zielone)"); // write text
			//zapalenie diod do testu
		if(TimerDelay <= 2)
			G1_ON;

		if(TimerDelay > 2 && TimerDelay <= 4)
		{
			G1_OFF;
			G2_ON;
		}

		if(TimerDelay > 4 && TimerDelay <= 6)
		{
			G2_OFF;
			G3_ON;
		}

		if(TimerDelay > 6 && TimerDelay <= 8)
		{
			G3_OFF;
			G4_ON;
		}

		if(TimerDelay > 8 && TimerDelay <= 10)
		{
			G4_OFF;
			G5_ON;
		}
		//	G5_OFF;

		if(TimerDelay > 10)
		{
			G1_ON;
			G2_ON;
			G3_ON;
			G4_ON;
			G5_ON;
		}

		return 0;
	}
	else if(POS == 2)
	{
		G1_OFF;
		G2_OFF;
		G3_OFF;
		G4_OFF;
		G5_OFF;

		TimerDelay = 0;	// zerujemy licznik
		POS++;
	}

	if(POS == 3 && TimerDelay++ < 5)	// czekamy N czasu
	{
		//test bipera
		LCD_ClearText();
		LCD_TextGoTo(0,3);
		LCD_WRiteString("              TEST BEEPERA"); // write text
		BUZER_ON;//wlczenie bipera na 1s
		return 0;
	}
	else if(POS == 3)
	{
		BUZER_OFF;

		TimerDelay = 0;	// zerujemy licznik
		POS++;
	}

	if(POS == 4 && TimerDelay++ < 5)	// czekamy N czasu
	{
		LCD_ClearText();
		LCD_TextGoTo(0,3);
		LCD_WRiteString("             TEST CISNIENIA"); // write text
		return 0;
	}
	else if(POS == 4)
	{
		TimerDelay = 0;	// zerujemy licznik
		POS++;
	}

	return OnStartPressureTest();
}

char OnStartPressureTest()
{
	int Pressure;
	int Pressure_KPA;

	static char ERROR_OF_PRESSURE=0;
	static char TimerDelay = 0;

	Pressure=ADC_CZUJNIK[CZUJNIK_6];//pomiar z czujnika glownego
//	Pressure=ADC_CZUJNIK_6;//pomiar z czujnika glownego
//	Pressure=pomiar(CZUJNIK_1);//pomiar z czujnika glownego
	Pressure_KPA=(PRESSURE_CONVERT(Pressure));//pomiar cisnienia dla czujnika absolutnego dlatego odejmujemy 100kPa - czyli cisnienie atmosferyczne
	//powyzej bez odejmowania 100 kPa bo jest przez wzmacniacza i tylko 85 - 115 kPa dzia³a

	if(TimerDelay++ < 18)	// czekamy N czasu
	{
		LCD_ClearText();
		LCD_TextGoTo(13,2);
		LCD_WRiteString("STABILIZACJA");
		return 0;
	}
	else
		TimerDelay = 255;	// zatrzymujemy licznik

	if(ERROR_OF_PRESSURE == 1 || (Pressure_KPA < (S_pressure-S_pressure_error) || Pressure_KPA > (S_pressure+S_pressure_error)))
	{
		ERROR_OF_PRESSURE = 1;

		LCD_ClearText();

		if(Pressure_KPA < (S_pressure-S_pressure_error))
		{
			LCD_TextGoTo(0,1);
			LCD_WRiteString("             BLAD CISNIENIA");
			LCD_TextGoTo(0,3);

			LCD_WRiteString("     CISNIENIE JEST ZA NISKIE ");
			LCD_WRiteString(" P=");
			LCD_WriteINT(Pressure_KPA);

			LCD_TextGoTo(0,5);
			LCD_WRiteString("          ZWIEKSZ DO P=");
			LCD_WriteINT(S_pressure);

			LCD_TextGoTo(26,5);
			LCD_WRiteString(" [kPa]");
		}
		else if(Pressure_KPA > (S_pressure+S_pressure_error))
		{
			LCD_TextGoTo(0,1);
			LCD_WRiteString("             BLAD CISNIENIA");
			LCD_TextGoTo(0,3);

			LCD_WRiteString("      CISNIENIE JEST ZA WYSOKIE");
			LCD_WRiteString(" P=");
			LCD_WriteINT(Pressure_KPA);

			LCD_TextGoTo(0,5);
			LCD_WRiteString("          ZMNIEJSZ DO P=");
			LCD_WriteINT(S_pressure);
			LCD_TextGoTo(28,5);
			LCD_WRiteString("[kPa]");
		}
		else
			ERROR_OF_PRESSURE = 0;
	}
	else if(ERROR_OF_PRESSURE == 0)
	{
		static char TimerDelay = 0;

		if(TimerDelay++ < 8)	// czekamy N czasu
		{
			LCD_ClearText();
			LCD_TextGoTo(0,2);
			LCD_WRiteString("   CISNIENIE JEST POPRAWNE P="); // write text
			LCD_WriteINT(Pressure_KPA);//wyswietlenie wartosci cisnienia
			LCD_TextGoTo(33,2);
			LCD_WRiteString("[kPa]"); // write text
			LCD_TextGoTo(0,4);
			LCD_WRiteString("         PRZEJSCIE DO TESTOWANIA"); // write text
			return 0;
		}
		else
		{
			TimerDelay = 255;	// zatrzymujemy licznik
			ERROR_OF_PRESSURE = 0;
			LCD_ClearText(); // czyszczenie obszaru tekstu

			return 1;
		}
	}

	return 0;
}

















