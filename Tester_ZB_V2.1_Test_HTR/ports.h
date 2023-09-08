#ifndef _ports_h
#define _ports_h

#include <avr/io.h>

//-------------------------------------------------------------------------------------------------
//
// Deklaracje portow - konfiguracja sygna³ów procesora
//
//-------------------------------------------------------------------------------------------------

//#define F_CPU 6144000UL  // czestotliwosc pracy 6,144 MHz
#define szerokosc 48
#define CMP 0 //deklaracja wyboru sygnalizacji zaniku napiecia zasilania, gdy CMP=0 to wybor przez INT

//-------------------------------------------------------------------------------------------------
//
// definicja portu A (BIT DATA)
//
//-------------------------------------------------------------------------------------------------
#define BIT_DATA_DIR		DDRA
#define BIT_DATA_PORT 		PORTA
#define BIT_DATA_PIN		PINA
//-------------------------------------------------------------------------------------------------
//
// definicja portu c (LCD DATA)
//
//-------------------------------------------------------------------------------------------------
#define LCD_DATA_DDR DDRC
#define LCD_DATA_PORT PORTC
#define LCD_DATA_PIN PINC
//-------------------------------------------------------------------------------------------------
//
// definicja portu B (BIT CONTROL, BUZER i LED green 5)
//
//-------------------------------------------------------------------------------------------------

//styk gniazda 5
#define SW_G4_DIR		DDRB
#define SW_G4_PORT		PORTB
#define SW_G4_PIN		PINB
#define SW_G4			(1 << PB0)

//PORT B1 ZAJETY PRZEZ SCK INTERFEJSU J-TAG

#define BIT_ADR2_DIR		DDRB
#define BIT_ADR2_PORT		PORTB
#define BIT_ADR2			(1 << PB2)

#define BIT_ADR1_DIR		DDRB
#define BIT_ADR1_PORT		PORTB
#define BIT_ADR1			(1 << PB3)

#define BIT_ADR0_DIR		DDRB
#define BIT_ADR0_PORT		PORTB
#define BIT_ADR0			(1 << PB4)

#define BIT_EN_DIR			DDRB
#define BIT_EN_PORT			PORTB
#define BIT_EN				(1 << PB5)

#define G5_DIR				DDRB
#define G5_PORT				PORTB
#define G5					(1 << PB6)

#define BUZER_DIR			DDRB
#define BUZER_PORT			PORTB
#define BUZER				(1 << PB7)

//-------------------------------------------------------------------------------------------------
//
// definicja portu D i G (WEJSCIA HTR I LCD CONTROL)
//
//-------------------------------------------------------------------------------------------------

/*#define HTR_G0_DIR		DDRD
#define HTR_G0_PORT		PORTD	
#define HTR_G0_PIN		PIND		
#define HTR_G0			(1 << PD0)
*/

#define HTR_G1_DIR		DDRD
#define HTR_G1_PORT		PORTD			
#define HTR_G1_PIN		PIND
#define HTR_G1			(1 << PD1)

#define HTR_G2_DIR		DDRD
#define HTR_G2_PORT		PORTD			
#define HTR_G2_PIN		PIND
#define HTR_G2			(1 << PD2)

#define HTR_G3_DIR		DDRD
#define HTR_G3_PORT		PORTD			
#define HTR_G3_PIN		PIND
#define HTR_G3			(1 << PD3)

#define HTR_G4_DIR		DDRD
#define HTR_G4_PORT		PORTD			
#define HTR_G4_PIN		PIND
#define HTR_G4			(1 << PD4)

//LCD CONTROL

#define LCD_RD_DIR		DDRD
#define LCD_RD_PORT		PORTD			
#define LCD_RD			(1 << PD5)

#define LCD_WR_DIR		DDRD
#define LCD_WR_PORT		PORTD			 
#define LCD_WR			(1 << PD6)

#define LCD_CD_DIR		DDRD
#define LCD_CD_PORT		PORTD			 
#define LCD_CD			(1 << PD7)

//	port G

#define LCD_CE_DIR		DDRG
#define LCD_CE_PORT		PORTG			 
#define LCD_CE			(1 << PG0)

#define LCD_RST_DIR		DDRG
#define LCD_RST_PORT	PORTG			 
#define LCD_RST			(1 << PG1)

//-------------------------------------------------------------------------------------------------
//
// definicja portu E (STYKI GNIZAD)
//
//-------------------------------------------------------------------------------------------------

//PORT E PINY (0,1,2) ZAREZERWOWANE DLA J-TAG

//gniazdo wybierane w zaleznosci od ustawienia zworek na plytce dla INT -> PE3 dla CMP -> PE4


//#if ( CMP==0 )//gdy CMP = 0 to przerwanie zaniku napiecia wskazuje przerwanie zewnetrzne

#define SW_G0_DIR		DDRE
#define SW_G0_PORT 		PORTE
#define SW_G0_PIN		PINE
#define SW_G0			(1 << PE3)

#define POWER_DOWN (SW_G0_PIN&_BV(4))

//#endif
/*
#if ( CMP==1 )//gdy CMP = 1 to przerwanie zaniku napiecia sygnalizuje komparator

#define SW_G0_DIR		DDRE
#define SW_G0_PORT 		PORTE
#define SW_G0_PIN		PINE
#define SW_G0			(1 << PE4)

#endif
*/
#define SW_G1_DIR		DDRE
#define SW_G1_PORT 		PORTE
#define SW_G1_PIN 		PINE
#define SW_G1			(1 << PE5)

#define SW_G2_DIR		DDRE
#define SW_G2_PORT		PORTE
#define SW_G2_PIN		PINE
#define SW_G2			(1 << PE6)

#define SW_G3_DIR		DDRE
#define SW_G3_PORT		PORTE
#define SW_G3_PIN		PINE
#define SW_G3			(1 << PE7)

//-------------------------------------------------------------------------------------------------
//
// definicja portu F (MATRYCA CZUJNIKOW CISNIENIA)
//
//-------------------------------------------------------------------------------------------------

#define P0_DIR			DDRF
#define P0_PORT			PORTF			 
#define P0_PIN 			PINF
#define P0				(1 << PF0)

#define P1_DIR			DDRF
#define P1_PORT			PORTF			 
#define P1_PIN 			PINF
#define P1				(1 << PF1)

#define P2_DIR			DDRF
#define P2_PORT			PORTF			 
#define P2_PIN 			PINF
#define P2				(1 << PF2)

#define P3_DIR			DDRF
#define P3_PORT			PORTF			 
#define P3_PIN 			PINF
#define P3				(1 << PF3)

#define P4_DIR			DDRF
#define P4_PORT			PORTF			 
#define P4_PIN 			PINF
#define P4				(1 << PF4)

#define P5_DIR			DDRF
#define P5_PORT			PORTF			 
#define P5_PIN 			PINF
#define P5				(1 << PF5)

//-------------------------------------------------------------------------------------------------
//
// Deklaracje makra 
//
//-------------------------------------------------------------------------------------------------

//definicja makr stykow zbiornikow
#define Z0 (SW_G0_PIN&_BV(3))
#define Z1 (SW_G1_PIN&_BV(5))
#define Z2 (SW_G2_PIN&_BV(6))
#define Z3 (SW_G3_PIN&_BV(7))
#define Z4 (SW_G4_PIN&_BV(0))
//makro buzera
#define BUZER_ON 	BUZER_PORT|=BUZER
#define BUZER_OFF 	BUZER_PORT&=~BUZER
//makro wlaczania zatrzaskow
#define LATCH_ON	BIT_EN_PORT|=BIT_EN
#define LATCH_OFF	BIT_EN_PORT&=~BIT_EN


#define ON G5_PORT|=_BV(G5)
#define OFF G5_PORT&=~_BV(G5)
//-------------------------------------------------------------------------------------------------
//
// Deklaracje funkcji
//
//-------------------------------------------------------------------------------------------------

void PORTS_Initalize(void);

#endif
//-------------------------------------------------------------------------------------------------
//
// Koniec pliku ports.h
//
//-------------------------------------------------------------------------------------------------
