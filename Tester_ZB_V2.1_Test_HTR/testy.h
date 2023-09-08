/*
 * testy.h
 *
 *  Created on: 30 lip 2020
 *      Author: Dawid Sobków
 */

#ifndef TESTY_H_
#define TESTY_H_

#include <avr/io.h>

enum zbiornik	 { ZBIORNIK_1, ZBIORNIK_2, ZBIORNIK_3, ZBIORNIK_4, ZBIORNIK_5 };
enum tests  { NONE, WOREK, ZBIORNIK, PODCISNIENIE, HTR };
enum tests actualTest[5];

enum status { IDLE, TESTING, FAILED, SUCCED, END };
enum status testResult[5];

uint16_t zbTimers	  [5];
int16_t zbMaxPressure[5];	// cisnienie zarejestrowane zaraz po napelnieniu
int16_t zbRefPressure[5];	// cisnienie po stabilizacji
int16_t zbEndPressure[5];	// cisnienie po pomiarze kontrolnym
int16_t zbDifPressure [5];
uint8_t zbErrorResult[5];	// 1 jezeli blad byl spowodowany nie osiagnieciem minimalnego cisnienia ERROR_WOREK_MINPRESSURE. 2 jezeli spadej podczas stabilizacji byl wiekszy od ERROR_WOREK_MAXDROPOFF
//uint8_t zbRelayInUse;	// stan wysoki informuje o tym ¿e zosta³ uzyty przekaŸnik/zawor powietrza. Bêdzie u¿yte do zabezpieczenia przed potencialnym b³êdem, w momencie gdy za³¹cza siê zawór i wywala b³¹d na innym gnieŸdzie.
//uint8_t zbRelayInUsePos[5];	// pomocnicza

// czas 1/3 sekundy
#define ERROR_WOREK_MAXDROPOFF	4 	// Wstêpny dozwolony spadek ciœnienia. Wy³apuj¹cy du¿e nieszczelnoœci.
#define ERROR_WOREK_MINPRESSURE 100	// Minimalne cisnienie
#define ERROR_WOREK_MAX_DIF		2	// Dozwolony spadek ciœnienia
#define CZAS_WOREK_NAP1 30			// Czas nape³niania
#define CZAS_WOREK_STB1 30			// czas stabilizacji
#define CZAS_WOREK_POM1 60			// czas pomiaru
#define CZAS_WOREK_SPU1 20 			// czas spuszczania powietrza
#define CZAS_WOREK_PRE1 9 			// czas prezentacji wyniku. OpóŸnienie przed przejœciem do kolejnego testu
#define CZAS_WOREK		CZAS_WOREK_NAP1+CZAS_WOREK_STB1+CZAS_WOREK_POM1+CZAS_WOREK_SPU1+CZAS_WOREK_PRE1 // ca³kowity czas trwania testu worka

#define ERROR_ZBIORNIK_MAXDROPOFF  4
#define ERROR_ZBIORNIK_MINPRESSURE 100
#define ERROR_ZBIORNIK_MAX_DIF     2
#define CZAS_ZBIORNIK_NAP1 40
#define CZAS_ZBIORNIK_STB1 40
#define CZAS_ZBIORNIK_POM1 240
#define CZAS_ZBIORNIK_SPU1 15
#define CZAS_ZBIORNIK_PRE1 9
#define CZAS_ZBIORNIK      CZAS_ZBIORNIK_NAP1+CZAS_ZBIORNIK_STB1+CZAS_ZBIORNIK_POM1+CZAS_ZBIORNIK_SPU1+CZAS_ZBIORNIK_PRE1

#define ERROR_PODCISNIENIE_MAXDROPOFF  	-4
#define ERROR_PODCISNIENIE_MINPRESSURE -15
#define ERROR_PODCISNIENIE_MAX_DIF		-2
#define CZAS_PODCISNIENIE_NAP1 30
#define CZAS_PODCISNIENIE_STB1 40
#define CZAS_PODCISNIENIE_POM1 50
#define CZAS_PODCISNIENIE_SPU1 10
#define CZAS_PODCISNIENIE_PRE1 9
#define CZAS_PODCISNIENIE 	   CZAS_PODCISNIENIE_NAP1+CZAS_PODCISNIENIE_STB1+CZAS_PODCISNIENIE_POM1+CZAS_PODCISNIENIE_SPU1+CZAS_PODCISNIENIE_PRE1

#define CZAS_CALKOWITY CZAS_WOREK + CZAS_ZBIORNIK + CZAS_PODCISNIENIE // Czas ca³kowity testu zbiornika

enum status StartTesting		 (uint8_t zbNR);
enum status TestWorka			 (uint8_t zbNR);
enum status TestZbiornika		 (uint8_t zbNR);
enum status TestPodcisnieniaWorka(uint8_t zbNR);
enum status TestHTR				 (uint8_t zbNR);

void ResetZmiennych(uint8_t zbNR);
uint16_t GetDifPressure(uint16_t start, uint16_t stop);

// ustawianie i kasowanie flag uzywanych do zapobiegania przed potencjalnym bledem gdy moze wyskoczyr blad w momencie gdy zalacza sie zawor powietrza
//void SetHighRelayFlag();
//void SetLowRelayFlag();
//uint8_t IsRelayInUse();

#endif /* TESTY_H_ */
