/*
 * testy.c
 *
 *  Created on: 30 lip 2020
 *      Author: Dawid Sobków
 */
#include <avr/io.h>

#include "testy.h"
#include "common.h"
#include "ds_GFX/ds_GFX.h"
#include "T6963C.h"
#include "ADC/adc.h"
#include "DS1Wire/ds_1wire.h"

enum status StartTesting(uint8_t zbNR)
{
	uint8_t result=0;

	//Testy
	if(testResult[zbNR] == IDLE)
	{
		//Zerowanie timerow i innych
		ResetZmiennych(zbNR);
		actualTest[zbNR] = HTR;

		return TESTING;
	}

	if(testResult[zbNR] == TESTING)
	{
		if(actualTest[zbNR] == HTR) // ############################################# HTR
		{
			result = TestHTR(zbNR);
			if(result == SUCCED)
			{
				//Zerowanie timerow i innych
				ResetZmiennych(zbNR);

				actualTest[zbNR] = WOREK;
				return TESTING;
			}
			else if(result == FAILED)
				return FAILED;

			return testResult[zbNR];
		}

		if(actualTest[zbNR] == WOREK) // ############################################# WOREK
		{
			result = TestWorka(zbNR);
			if(result == SUCCED)
			{
				//Zerowanie timerow i innych
				ResetZmiennych(zbNR);

				actualTest[zbNR] = ZBIORNIK;
				return TESTING;
			}
			else if(result == FAILED)
			{
				return FAILED;
			}

			return testResult[zbNR];
		}

		if(actualTest[zbNR] == ZBIORNIK)	// ####################################### ZBIORNIK
		{
			result = TestZbiornika(zbNR);
			if(result == SUCCED)
			{
				//Zerowanie timerow i innych
				ResetZmiennych(zbNR);

				actualTest[zbNR] = PODCISNIENIE;
				return TESTING;
			}
			else if(result == FAILED)
				return FAILED;

			return testResult[zbNR];
		}

		if(actualTest[zbNR] == PODCISNIENIE)	// ####################################### PODCISNIENIE
		{
			result = TestPodcisnieniaWorka(zbNR);
			if(result == SUCCED)
			{

				actualTest[zbNR] = NONE;
				return SUCCED;
			}
			else if(result == FAILED)
				return FAILED;

			return testResult[zbNR];
		}
	}
	else if(testResult[zbNR] == SUCCED)
	{
		LED_STATUS_ON(zbNR, 1);	// Zapal zielon¹
		VALVE_CONTROL(zbNR, 4);	//	Wy³¹czenie zaworów

		return SUCCED;
	}
	else if(testResult[zbNR] == FAILED)
	{
		LED_STATUS_ON(zbNR, 0);	// Zapal czerwona
		VALVE_CONTROL(zbNR, 4);	//	Wy³¹czenie zaworów

		return FAILED;
	}

	return END;
}

enum status TestWorka(uint8_t zbNR)
{
	zbTimers[zbNR]++;

	if(zbTimers[zbNR] < CZAS_WOREK_NAP1)	// Czas nape³niania
	{
		VALVE_CONTROL(zbNR,1);
	}
	else if(zbTimers[zbNR] < CZAS_WOREK_STB1+CZAS_WOREK_NAP1) // czasStabilizacji+czasNapelniania)
	{
		VALVE_CONTROL(zbNR,2);	//przelaczenie zaworu glownego na pomiar
		if(zbMaxPressure[zbNR] < ADC_CZUJNIK[zbNR])		// £apiemy najwiêkszy pomiar
			zbMaxPressure[zbNR] = ADC_CZUJNIK[zbNR];

		zbRefPressure[zbNR] = zbMaxPressure[zbNR];
		zbDifPressure[zbNR] = GetDifPressure(zbMaxPressure[zbNR], ADC_CZUJNIK[zbNR]);

		if(zbTimers[zbNR] > CZAS_WOREK_STB1/3 + CZAS_WOREK_NAP1) // czasNapelniania)		// Czekamy N czasu
		{
			if(zbRefPressure[zbNR] < ERROR_WOREK_MINPRESSURE)
			{
				zbErrorResult[zbNR] = 1;
				return FAILED;
			}
			if(zbDifPressure[zbNR] > ERROR_WOREK_MAXDROPOFF)
			{
				zbErrorResult[zbNR] = 2;
				return FAILED;
			}
		}
	}
	else if(zbTimers[zbNR] < CZAS_WOREK_POM1+(CZAS_WOREK_STB1+CZAS_WOREK_NAP1))// czasPomiaru+(czasStabilizacji+czasNapelniania))
	{
		zbEndPressure[zbNR] = ADC_CZUJNIK[zbNR];
		zbDifPressure[zbNR] = GetDifPressure(zbRefPressure[zbNR], zbEndPressure[zbNR]);
	}
	else if(zbTimers[zbNR] < CZAS_WOREK_PRE1+(CZAS_WOREK_POM1+CZAS_WOREK_STB1+CZAS_WOREK_NAP1))// czasPrezentacjiWyniku+(czasPomiaru+czasStabilizacji+czasNapelniania))	// WYNIK
	{
		if(zbDifPressure[zbNR] > ERROR_WOREK_MAX_DIF)
		{
			return FAILED;
		}
	}
	else if(zbTimers[zbNR] < CZAS_WOREK_SPU1+(CZAS_WOREK_PRE1+CZAS_WOREK_POM1+CZAS_WOREK_STB1+CZAS_WOREK_NAP1))// czasSpuszczania+(czasPrezentacjiWyniku+czasPomiaru+czasStabilizacji+czasNapelniania))
	{
		VALVE_CONTROL(zbNR,4);	//	Wy³¹czenie zaworów
	}
	else
	{
		return SUCCED;
	}

	return TESTING;
}

enum status TestZbiornika(uint8_t zbNR)
{
	zbTimers[zbNR]++;

	if(zbTimers[zbNR] < CZAS_ZBIORNIK_NAP1)	// Czas nape³niania
	{
		VALVE_CONTROL(zbNR,3);	//napelnianie zbiornika
	}
	else if(zbTimers[zbNR] < CZAS_ZBIORNIK_STB1+CZAS_ZBIORNIK_NAP1) // czasStabilizacji+czasNapelniania)
	{
		VALVE_CONTROL(zbNR,2);	//przxelaczenie zaworu glownego na pomiar

		if(zbMaxPressure[zbNR] < ADC_CZUJNIK[zbNR])		// £apiemy najwiêkszy pomiar
			zbMaxPressure[zbNR] = ADC_CZUJNIK[zbNR];

		zbRefPressure[zbNR] = zbMaxPressure[zbNR];
		zbDifPressure[zbNR] = GetDifPressure(zbMaxPressure[zbNR], ADC_CZUJNIK[zbNR]);

		if(zbTimers[zbNR] > CZAS_ZBIORNIK_STB1/3 + CZAS_ZBIORNIK_NAP1) // czasNapelniania)		// Czekamy N czasu
		{
			if(zbRefPressure[zbNR] < ERROR_ZBIORNIK_MINPRESSURE)
			{
				zbErrorResult[zbNR] = 1;
				return FAILED;
			}
			if(zbDifPressure[zbNR] > ERROR_ZBIORNIK_MAXDROPOFF)
			{
				zbErrorResult[zbNR] = 1;
				return FAILED;
			}
		}
	}
	else if(zbTimers[zbNR] < CZAS_ZBIORNIK_POM1+(CZAS_ZBIORNIK_STB1+CZAS_ZBIORNIK_NAP1))// czasPomiaru+(czasStabilizacji+czasNapelniania))
	{
		zbEndPressure[zbNR] = ADC_CZUJNIK[zbNR];
		zbDifPressure[zbNR] = GetDifPressure(zbRefPressure[zbNR], zbEndPressure[zbNR]);
	}
	else if(zbTimers[zbNR] < CZAS_ZBIORNIK_PRE1+(CZAS_ZBIORNIK_POM1+CZAS_ZBIORNIK_STB1+CZAS_ZBIORNIK_NAP1))// czasPrezentacjiWyniku+(czasPomiaru+czasStabilizacji+czasNapelniania))	// WYNIK
	{
		if(zbDifPressure[zbNR] > ERROR_ZBIORNIK_MAX_DIF)
		{
			return FAILED;
		}
	}
	else if(zbTimers[zbNR] < CZAS_ZBIORNIK_SPU1+(CZAS_ZBIORNIK_PRE1+CZAS_ZBIORNIK_POM1+CZAS_ZBIORNIK_STB1+CZAS_ZBIORNIK_NAP1))// czasSpuszczania+(czasPrezentacjiWyniku+czasPomiaru+czasStabilizacji+czasNapelniania))
	{
		VALVE_CONTROL(zbNR,5);	//	spuszczanie powietsza z zbiornika
	}
	else
	{
		return SUCCED;
	}

	return TESTING;
}

enum status TestPodcisnieniaWorka(uint8_t zbNR)
{
	zbTimers[zbNR]++;

	if(zbTimers[zbNR] < CZAS_PODCISNIENIE_NAP1)	// Czas nape³niania
	{
		VALVE_CONTROL(zbNR,3);	//napelnianie zbiornika
	}
	else if(zbTimers[zbNR] < CZAS_PODCISNIENIE_STB1+CZAS_PODCISNIENIE_NAP1) // czasStabilizacji+czasNapelniania)
	{
		VALVE_CONTROL(zbNR,2);	//Pomiar
		VALVE_CONTROL(zbNR,5);	//	spuszczanie powietsza z zbiornika

		if(zbTimers[zbNR] > CZAS_PODCISNIENIE_NAP1 + 9)	// delay
		{
			if(zbMaxPressure[zbNR] > ADC_CZUJNIK[zbNR])		// £apiemy Najmniejszy pomiar
				zbMaxPressure[zbNR] = ADC_CZUJNIK[zbNR];

			zbRefPressure[zbNR] = zbMaxPressure[zbNR];
			zbDifPressure[zbNR] = zbRefPressure[zbNR] - ADC_CZUJNIK[zbNR];
		}

		if(zbTimers[zbNR] > CZAS_PODCISNIENIE_STB1/3 + CZAS_PODCISNIENIE_NAP1) // czasNapelniania)		// Czekamy N czasu
		{
			if(zbRefPressure[zbNR] > ERROR_PODCISNIENIE_MINPRESSURE)
			{
				zbErrorResult[zbNR] = 1;
				return FAILED;
			}
			if(zbDifPressure[zbNR] < ERROR_PODCISNIENIE_MAXDROPOFF)
			{
				zbErrorResult[zbNR] = 1;
				return FAILED;
			}
		}
	}
	else if(zbTimers[zbNR] < CZAS_PODCISNIENIE_POM1+(CZAS_PODCISNIENIE_STB1+CZAS_PODCISNIENIE_NAP1))// czasPomiaru+(czasStabilizacji+czasNapelniania))
	{
		zbDifPressure[zbNR] = zbRefPressure[zbNR] - ADC_CZUJNIK[zbNR];
	}
	else if(zbTimers[zbNR] < CZAS_PODCISNIENIE_PRE1+(CZAS_PODCISNIENIE_POM1+CZAS_PODCISNIENIE_STB1+CZAS_PODCISNIENIE_NAP1))// czasPrezentacjiWyniku+(czasPomiaru+czasStabilizacji+czasNapelniania))	// WYNIK
	{
		if(zbDifPressure[zbNR] < ERROR_PODCISNIENIE_MAX_DIF)
		{
			return FAILED;
		}
	}
	else if(zbTimers[zbNR] < CZAS_PODCISNIENIE_SPU1+(CZAS_PODCISNIENIE_PRE1+CZAS_PODCISNIENIE_POM1+CZAS_PODCISNIENIE_STB1+CZAS_PODCISNIENIE_NAP1))// czasSpuszczania+(czasPrezentacjiWyniku+czasPomiaru+czasStabilizacji+czasNapelniania))
	{
		VALVE_CONTROL(zbNR,4);	//	Wy³¹czenie zaworów
	}
	else
	{
		return SUCCED;
	}

	return TESTING;
}

enum status TestHTR(uint8_t zbNR)
{
	uint8_t result = ERR_1W_ALLOK;
	result = DS1W_ReadSerialAndChackCRC(zbNR);

	if(result != ERR_1W_ALLOK)
	{
		zbErrorResult[zbNR] = result;
		return FAILED;
	}

	result = DS1W_TestHTRWriteAndReadData(zbNR);

	if(result != ERR_1W_ALLOK)
	{
		zbErrorResult[zbNR] = result;
		return FAILED;
	}

	return SUCCED;
}

void ResetZmiennych(uint8_t zbNR)
{
	zbTimers[zbNR] = 0;
	zbDifPressure[zbNR] = 0;
	zbMaxPressure[zbNR] = 0;
	zbRefPressure[zbNR] = 0;
	zbEndPressure[zbNR] = 0;
	LED_STATUS_OFF(zbNR);	//Wygaœ diody

	zbErrorResult[zbNR] = 0;
//	zbRelayInUsePos[zbNR] = 0;
}

uint16_t GetDifPressure(uint16_t start, uint16_t stop)
{
	uint16_t dif = start - stop;
//	if(start < stop)
//		dif = 0;

	return dif;
}

//void SetHighRelayFlag()
//{
//	TimerRelay=200;
//
//	zbRelayInUse = 1;
//}
//
//void SetLowRelayFlag()
//{
//	zbRelayInUse = 0;
//}
//
//uint8_t IsRelayInUse()
//{
//	return zbRelayInUse;
//}
























