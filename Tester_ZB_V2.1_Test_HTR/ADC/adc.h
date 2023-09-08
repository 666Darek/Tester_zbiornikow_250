/*
 * adc.h
 *
 *  Created on: 5 mar 2017
 *      Author: DidekxD
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

// Piny
#define CZUJNIK_1	PF0
#define CZUJNIK_2	PF1
#define CZUJNIK_3 	PF2
#define CZUJNIK_4	PF3
#define CZUJNIK_5	PF4
#define CZUJNIK_6	PF5
#define CZUJNIK_7 	PF6
#define CZUJNIK_8 	PF7

#define DT 8	// ilosc probek

int16_t ADC_CZUJNIK[6];
uint16_t ADC_CZUJNIK_1, ADC_CZUJNIK_2, ADC_CZUJNIK_3, ADC_CZUJNIK_4;
uint16_t ADC_CZUJNIK_5, ADC_CZUJNIK_6, ADC_CZUJNIK_7, ADC_CZUJNIK_8;

typedef struct {
	uint16_t wynik;
	uint32_t srednia;
} ADCS;

ADCS ADCS_CZUJNIK_1, ADCS_CZUJNIK_2, ADCS_CZUJNIK_3, ADCS_CZUJNIK_4;
ADCS ADCS_CZUJNIK_5, ADCS_CZUJNIK_6, ADCS_CZUJNIK_7, ADCS_CZUJNIK_8;

void init_ADC( void );
uint16_t pomiar( uint8_t kanal );
uint16_t Licz_Srednia( ADCS * srednia, uint8_t kanal );

void ADC_GETALL( void );

//void adc_get_CZUJNIK_1( void );
//void adc_get_CZUJNIK_2( void );
//void adc_get_CZUJNIK_3( void );
//void adc_get_CZUJNIK_4( void );
//void adc_get_CZUJNIK_5( void );
//void adc_get_CZUJNIK_6( void );
//void adc_get_CZUJNIK_7( void );
//void adc_get_CZUJNIK_8( void );

int PRESSURE_CONVERT(int measure);

#endif /* ADC_ADC_H_ */
