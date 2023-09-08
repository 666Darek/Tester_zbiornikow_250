	/*
 * main.c
 *
 *  Created on: 13.07.2020
 *      Author: D. Sobków
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>

#include "ADC/adc.h"
#include "ds_GFX/ds_GFX.h"
#include "T6963C.h"
#include "ports.h"
#include "common.h"
#include "initTests.h"
#include "testy.h"
#include "showTestInfo.h"

int main( void ) {

	/* Timer0 – inicjalizacja przerwania co 10ms */
	TCCR0 |= (1<<WGM01);		// tryb pracy CTC
	TCCR0 |= (1<<CS02)|(1<<CS01)|(1<<CS00);	// preskaler = 1024
	OCR0 = (F_CPU/1024UL)/100UL;	// 10ms - 100Hz
	TIMSK |= (1<<OCIE0);	// Odblokowanie przerwania CompareMatch

	PORTS_Initalize();//inicializacja portow
	init_ADC();
	LCD_Initalize(); // inicizlizacja LCD
	LCD_ClearText(); // czyszczenie obszaru tekstu
	LCD_ClearCG(); // czyszczenie generatora znakow
	LCD_ClearGraphic(); // czyszczenie obszaru grafiki

	uint8_t hb=0; // heart beat

	LATCH_ON;
	//zerowanie wyjsc zatrzaskow
	PORTA=0;//wyzerowanie szyny danych dla zatrzaskow
	PORTB|=_BV(4);//uaktywnienie EN zatrzaskow by skasowac ich wyjscia
	PORTB|=_BV(3);
	PORTB|=_BV(2);
	_delay_ms(50);
	PORTB&=~_BV(4);//zablokowanie wartosci w zatrzaskach
	PORTB&=~_BV(3);
	PORTB&=~_BV(2);

	sei();
	while(1)
	{
			//ADC
		if(!Timer150)
		{
			Timer150=15;

			ADC_GETALL();
		}

		if(!Timer1)
		{
			Timer1=33;

			if(isInitTestsDone==0)
			{
				Intro();
				isInitTestsDone = InitTests();

				if(isInitTestsDone)
				{
					LCD_ClearText(); // czyszczenie obszaru tekstu
					LCD_ClearCG(); // czyszczenie generatora znakow
					LCD_ClearGraphic(); // czyszczenie obszaru grafiki
				}
			}
			else
			{

				if(ledButtonBlink[0] == 1)
				{
					ledButtonBlink[0] = 0;
					G1_OFF;
					G2_OFF;
					G3_OFF;
					G4_OFF;
					G5_OFF;
				}

				if(ledButtonBlink[1] == 1)
				{
					ledButtonBlink[1] = 0;
					R1_OFF;
					R2_OFF;
					R3_OFF;
					R4_OFF;
					R5_OFF;
				}

				if(startMode == START_MODE_WHENALL)
				{
					for (uint8_t i = 0;  i < 5;  i++) {
						startButtonFlag = 1;

						if(zbIsButtonPressed[i] != 1 || actualTest[i] != 0)
						{
							startButtonFlag = 0;
							break;
						}
					}

					if(startButtonFlag == 1)
					{
						for (uint8_t i = 0;  i < 5;  i++) {
								zbIsStarted[i] = 1;
						}
					}

				}
				else
					startButtonFlag = 0;

				//D.Michalak 31.03.2023
				//Czêœc kodu odpowiedzialna za gaszenie diody
				//po wykrêceniu zbiornika zanim rusz¹ testy.
				if(zbIsButtonPressed[0] == 1 && Z0 != 0)
				{
					G1_OFF;
					zbIsButtonPressed[0] = 0;

				}
				if(zbIsButtonPressed[1] == 1 && Z1 != 0)
				{
					G2_OFF;
					zbIsButtonPressed[1] = 0;
				}
				if(zbIsButtonPressed[2] == 1 && Z2 != 0)
				{
					G3_OFF;
					zbIsButtonPressed[2] = 0;;
				}
				if(zbIsButtonPressed[3] == 1 && Z3 != 0)
				{
					G4_OFF;
					zbIsButtonPressed[3] = 0;
				}
				if(zbIsButtonPressed[4] == 1 && Z4 != 0 )
				{
					G5_OFF;
					zbIsButtonPressed[4] = 0;
				}


				//guziki normal i when all dzia³aj¹ reszta wymaga testow
				// ################ 1
				if((startMethod == START_BUTTON && zbIsButtonPressed[0] == 0 && Z0==0)	// Start testu
				|| (startMethod == START_TOGGLE && zbIsStarted[0] == 0 && Z0==0))
				{
					G1_ON;

					if(startMode == START_MODE_NORMAL)
					{
						ledButtonBlink[0]=1;
						zbIsStarted[0] = 1;
					}
					else
					{
						 zbIsButtonPressed[0] = 1;
					}
				}
				else if((startMethod == START_BUTTON && zbIsButtonPressed[0] == 1 && Z0==0)	// Zatrzymanie testu
				     || (startMethod == START_TOGGLE && zbIsStarted[0] == 1 && Z0!=0))
				{
					R1_ON;
					G1_OFF;
					ledButtonBlink[1]=1;
					zbIsButtonPressed[0] = 0;

					zbIsStarted[0] = 0;
					VALVE_CONTROL(ZBIORNIK_1, 4);
					actualTest[ZBIORNIK_1] = NONE;
					testResult[ZBIORNIK_1] = IDLE;
				}
				else if(zbIsStarted[0] == 1)
				{

					testResult[ZBIORNIK_1] = StartTesting(ZBIORNIK_1);
				}

				// ################ 2
				if((startMethod == START_BUTTON && zbIsButtonPressed[1] == 0 && Z1==0)	// Start testu
				|| (startMethod == START_TOGGLE && zbIsStarted[1] == 0 && Z1==0))
				{
					G2_ON;

					if(startMode == START_MODE_NORMAL)
					{
						ledButtonBlink[0]=1;
						zbIsStarted[1] = 1;
					}
					else
					{
						 zbIsButtonPressed[1] = 1;
					}
				}
				else if((startMethod == START_BUTTON && zbIsButtonPressed[1] == 1 && Z1==0)	// Zatrzymanie testu
				     || (startMethod == START_TOGGLE && zbIsStarted[1] == 1 && Z1!=0))
				{
					R2_ON;
					G2_OFF;
					ledButtonBlink[1]=1;
					zbIsButtonPressed[1] = 0;

					zbIsStarted[1] = 0;
					VALVE_CONTROL(ZBIORNIK_2, 4);
					actualTest[ZBIORNIK_2] = NONE;
					testResult[ZBIORNIK_2] = IDLE;
				}
				else if(zbIsStarted[1] == 1)
				{
					testResult[ZBIORNIK_2] = StartTesting(ZBIORNIK_2);
				}

				// ################ 3
				if((startMethod == START_BUTTON && zbIsButtonPressed[2] == 0 && Z2==0)	// Start testu
				|| (startMethod == START_TOGGLE && zbIsStarted[2] == 0 && Z2==0))
				{
					G3_ON;

					if(startMode == START_MODE_NORMAL)
					{
						ledButtonBlink[0]=1;
						zbIsStarted[2] = 1;
					}
					else
					{
						 zbIsButtonPressed[2] = 1;
					}
				}
				else if((startMethod == START_BUTTON && zbIsButtonPressed[2] == 1 && Z2==0)	// Zatrzymanie testu
				     || (startMethod == START_TOGGLE && zbIsStarted[2] == 1 && Z2!=0))
				{
					R3_ON;
					G3_OFF;
					ledButtonBlink[1]=1;
					zbIsButtonPressed[2] = 0;

					zbIsStarted[2] = 0;
					VALVE_CONTROL(ZBIORNIK_3, 4);
					actualTest[ZBIORNIK_3] = NONE;
					testResult[ZBIORNIK_3] = IDLE;
				}
				else if(zbIsStarted[2] == 1)
				{
					testResult[ZBIORNIK_3] = StartTesting(ZBIORNIK_3);
				}

				// ################ 4
				if((startMethod == START_BUTTON && zbIsButtonPressed[3] == 0 && Z3==0)	// Start testu
				|| (startMethod == START_TOGGLE && zbIsStarted[3] == 0 && Z3==0))
				{
					G4_ON;

					if(startMode == START_MODE_NORMAL)
					{
						ledButtonBlink[0]=1;
						zbIsStarted[3] = 1;
					}
					else
					{
						 zbIsButtonPressed[3] = 1;
					}
				}
				else if((startMethod == START_BUTTON && zbIsButtonPressed[3] == 1 && Z3==0)	// Zatrzymanie testu
				     || (startMethod == START_TOGGLE && zbIsStarted[3] == 1 && Z3!=0))
				{
					R4_ON;
					G4_OFF;
					ledButtonBlink[1]=1;
					zbIsButtonPressed[3] = 0;

					zbIsStarted[3] = 0;
					VALVE_CONTROL(ZBIORNIK_4, 4);
					actualTest[ZBIORNIK_4] = NONE;
					testResult[ZBIORNIK_4] = IDLE;
				}
				else if(zbIsStarted[3] == 1)
				{
					testResult[ZBIORNIK_4] = StartTesting(ZBIORNIK_4);
				}

				// ################ 5
				if((startMethod == START_BUTTON && zbIsButtonPressed[4] == 0 && Z4==0)	// Start testu
				|| (startMethod == START_TOGGLE && zbIsStarted[4] == 0 && Z4==0))
				{
					G5_ON;

					if(startMode == START_MODE_NORMAL)
					{
						ledButtonBlink[0]=1;
						zbIsStarted[4] = 1;
					}
					else
					{
						 zbIsButtonPressed[4] = 1;
					}
				}
				else if((startMethod == START_BUTTON && zbIsButtonPressed[4] == 1 && Z4==0)	// Zatrzymanie testu
				     || (startMethod == START_TOGGLE && zbIsStarted[4] == 1 && Z4!=0))
				{
					R5_ON;
					G5_OFF;
					ledButtonBlink[1]=1;
					zbIsButtonPressed[4] = 0;

					zbIsStarted[4] = 0;
					VALVE_CONTROL(ZBIORNIK_5, 4);
					actualTest[ZBIORNIK_5] = NONE;
					testResult[ZBIORNIK_5] = IDLE;
				}
				else if(zbIsStarted[4] == 1)
				{
					testResult[ZBIORNIK_5] = StartTesting(ZBIORNIK_5);
				}

				// ****************************************************************
				for (uint8_t i = 0;  i < 5;  i++) {
					if(testResult[i] == FAILED)
					{
						BUZER_ON;
						break;
					}

					BUZER_OFF;
				}


				ShowTestInfo(ZBIORNIK_1, actualTest[ZBIORNIK_1]);
				ShowTestInfo(ZBIORNIK_2, actualTest[ZBIORNIK_2]);
				ShowTestInfo(ZBIORNIK_3, actualTest[ZBIORNIK_3]);
				ShowTestInfo(ZBIORNIK_4, actualTest[ZBIORNIK_4]);
				ShowTestInfo(ZBIORNIK_5, actualTest[ZBIORNIK_5]);
			}

			// heart beat
			hb=!hb;
			LCD_SetPixel(239,0,hb);
		}
	}
}
