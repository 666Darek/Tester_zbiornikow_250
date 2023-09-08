/*
 * adc.c
 *
 *  Created on: 5 mar 2017
 *      Author: DidekxD
 */


#include <avr/io.h>

#include "adc.h"
#include "../common.h"


//uint16_t ADC_CZUJNIK_1, ADC_CZUJNIK_2, ADC_CZUJNIK_3;
//uint16_t ADC_CZUJNIK_4, ADC_CZUJNIK_5, ADC_CZUJNIK_6;
//
//ADCS ADCS_CZUJNIK_1, ADCS_CZUJNIK_2, ADCS_CZUJNIK_3, ADCS_CZUJNIK_4;
//ADCS ADCS_CZUJNIK_5, ADCS_CZUJNIK_6, ADCS_CZUJNIK_7, ADCS_CZUJNIK_8;

void init_ADC(){
	//ADC init
	ADCSRA |= (1<<ADEN); //w³¹cz ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // preskaler 128
	ADMUX |= (1<<REFS0); // Ÿród³o odniesienia AVCC 5V
}

uint16_t pomiar( uint8_t kanal ){

//	ADMUX = (ADMUX & 0b11111000) | kanal;
//	ADCSRA |= (1<<ADSC);	//Start konwersji
//	while( ADCSRA & (1<<ADSC) );
//	return ADCW;

	ADMUX = (ADMUX & 0b11111000) | kanal;
	ADCSRA|=_BV(ADSC);//start konwersji
	while((ADCSR&_BV(ADIF))==0);//czakaj az pojawi sie znacznik konca konwersji
	uint16_t pom=ADCL;//Czytaj najpierw 8 mlodszych bitow (wazne !!!)
	pom|=(short)ADCH << 8;//przepisanie kolejnych 8 bitow
	ADCSR|=_BV(ADIF);//skasowanie flagi przez wpisanie jedynki logicznej

	return pom;
}

uint16_t Licz_Srednia( ADCS * srednia, uint8_t kanal ){
	srednia->srednia = srednia->srednia * DT;
	srednia->srednia = srednia->srednia + pomiar( kanal );
	srednia->srednia = srednia->srednia / (DT + 1);

	return srednia->srednia;
}

void ADC_GETALL()
{
//	ADC_CZUJNIK_1 = Licz_Srednia( &ADCS_CZUJNIK_1, CZUJNIK_1 );
//	ADC_CZUJNIK_2 = Licz_Srednia( &ADCS_CZUJNIK_2, CZUJNIK_2 );
//	ADC_CZUJNIK_3 = Licz_Srednia( &ADCS_CZUJNIK_3, CZUJNIK_3 );
//	ADC_CZUJNIK_4 = Licz_Srednia( &ADCS_CZUJNIK_4, CZUJNIK_4 );
//	ADC_CZUJNIK_5 = Licz_Srednia( &ADCS_CZUJNIK_5, CZUJNIK_5 );
//	ADC_CZUJNIK_6 = Licz_Srednia( &ADCS_CZUJNIK_6, CZUJNIK_6 );

	if(pressSensorType == MPX4250AP)
	{
		ADC_CZUJNIK[0] = Licz_Srednia( &ADCS_CZUJNIK_1, CZUJNIK_1 ) - PRESSSENSOR_OFFSET;
		ADC_CZUJNIK[1] = Licz_Srednia( &ADCS_CZUJNIK_2, CZUJNIK_2 ) - PRESSSENSOR_OFFSET;
		ADC_CZUJNIK[2] = Licz_Srednia( &ADCS_CZUJNIK_3, CZUJNIK_3 ) - PRESSSENSOR_OFFSET;
		ADC_CZUJNIK[3] = Licz_Srednia( &ADCS_CZUJNIK_4, CZUJNIK_4 ) - PRESSSENSOR_OFFSET;
		ADC_CZUJNIK[4] = Licz_Srednia( &ADCS_CZUJNIK_5, CZUJNIK_5 ) - PRESSSENSOR_OFFSET;
		ADC_CZUJNIK[5] = Licz_Srednia( &ADCS_CZUJNIK_6, CZUJNIK_6 ) - PRESSSENSOR_OFFSET;
	}
	else
	{
		ADC_CZUJNIK[0] = Licz_Srednia( &ADCS_CZUJNIK_1, CZUJNIK_1 );
		ADC_CZUJNIK[1] = Licz_Srednia( &ADCS_CZUJNIK_2, CZUJNIK_2 );
		ADC_CZUJNIK[2] = Licz_Srednia( &ADCS_CZUJNIK_3, CZUJNIK_3 );
		ADC_CZUJNIK[3] = Licz_Srednia( &ADCS_CZUJNIK_4, CZUJNIK_4 );
		ADC_CZUJNIK[4] = Licz_Srednia( &ADCS_CZUJNIK_5, CZUJNIK_5 );
		ADC_CZUJNIK[5] = Licz_Srednia( &ADCS_CZUJNIK_6, CZUJNIK_6 );
	}

//	ADC_CZUJNIK[0] = pomiar( CZUJNIK_1 );
//	ADC_CZUJNIK[1] = pomiar( CZUJNIK_2 );
//	ADC_CZUJNIK[2] = pomiar( CZUJNIK_3 );
//	ADC_CZUJNIK[3] = pomiar( CZUJNIK_4 );
//	ADC_CZUJNIK[4] = pomiar( CZUJNIK_5 );
//	ADC_CZUJNIK[5] = pomiar( CZUJNIK_6 );

//	adc_get_CZUJNIK_1();
//	adc_get_CZUJNIK_2();
//	adc_get_CZUJNIK_3();
//	adc_get_CZUJNIK_4();
//	adc_get_CZUJNIK_5();
//	adc_get_CZUJNIK_6();
}

//void adc_get_CZUJNIK_1()
//{
//	ADC_CZUJNIK_1 = Licz_Srednia( &ADCS_CZUJNIK_1, CZUJNIK_1 );
//}

//***********************************************************************************************************
//
//Funkcja zamieniajaca wartosc bitowa z przetwornika ADC na cisnienie podawane w kPa
//
//***********************************************************************************************************
int PRESSURE_CONVERT(int measure)
{
	if(pressSensorType == MPX4250AP)
	{
		return 5 * (measure*0.05-0.04);
	}

	static float pressure=0;
	static float Vout=0;
	int PkPa;

	//dla czujnika cisnienia MPX5100
	//funkcja kozysta z podanego w specyfikacji wzoru Vout = Vs (P x 0.009 + 0.04)
	//przy czym Vs = 5V dla tego testera

	float AV_REF = ((float)S_AV_REF+100)/100;

	Vout=(AV_REF*measure)/1024;//wyliczenie napiecia wejscia ADC ze wzoru (dokumentacja Atmegi128)

	pressure=2*((Vout-0.2)/0.043);//wyliczenie cisnienia ze wzoru (dokumentacja czujnika cisnienia MPX5100)
//	pressure=2*((Vout-0.2)/0.09);//wyliczenie cisnienia ze wzoru (dokumentacja czujnika cisnienia MPXV5050GP)
	if(pressure < 0)pressure=0;

	PkPa = (int)pressure + (S_P_offset-100);
	return PkPa;
}



