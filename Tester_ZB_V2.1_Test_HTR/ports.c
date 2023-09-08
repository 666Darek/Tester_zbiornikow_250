#include "ports.h"

//-------------------------------------------------------------------------------------------------
//
// Funkcja inicializujaca porty wejscia/wyjscia
//
//-------------------------------------------------------------------------------------------------

void PORTS_Initalize(void)
{
//WEJSCIA 

//wejscia gniazd
SW_G0_DIR	&=~SW_G0;
SW_G1_DIR	&=~SW_G1;
SW_G2_DIR	&=~SW_G2;
SW_G3_DIR	&=~SW_G3;
SW_G4_DIR	&=~SW_G4;
//PULL-UP
SW_G0_PORT|=SW_G0;
SW_G1_PORT|=SW_G1;
SW_G2_PORT|=SW_G2;
SW_G3_PORT|=SW_G3;
SW_G4_PORT|=SW_G4;
//wejscia przetwornika 
P0_DIR &=~P0;
P1_DIR &=~P1;
P2_DIR &=~P2;
P3_DIR &=~P3;
P4_DIR &=~P4;
P5_DIR &=~P5;
//-----------------------------------------------------------------------------------------------------
//WYJSCIA
//-----------------------------------------------------------------------------------------------------

//Wyjscia danych do zatrzaskow
BIT_DATA_DIR=255;
//wyjsica adresow do zatrzaskow
BIT_ADR0_DIR|=BIT_ADR0;
BIT_ADR1_DIR|=BIT_ADR1;
BIT_ADR2_DIR|=BIT_ADR2;
//wyjscie zalaczenia zatrzasku (LATCH)
BIT_EN_DIR|=BIT_EN;
//wyjscie wlaczania BUZERA
BUZER_DIR |=BUZER;
//wyjscie diody G5
G5_DIR|=G5;
}
//-------------------------------------------------------------------------------------------------
//
// Koniec pliku ports.c
//
//-------------------------------------------------------------------------------------------------


