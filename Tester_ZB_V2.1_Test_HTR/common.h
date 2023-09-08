/*
 * common.h
 *
 *  Created on: 22 lip 2020
 *      Author: Dawid Sobków
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <avr/io.h>
#include "ports.h"

//presure sensor
#define MPX4250AP 0
#define MPX		  1
#define PRESSSENSOR_OFFSET 360
//uint16_t pressSensorOffset;
uint8_t pressSensorType;

#define EBS260 0
#define MI2400 1
uint8_t printerSelected;
#define START_TOGGLE  0
#define START_BUTTON  1
#define START_MODE_NORMAL 0
#define START_MODE_WHENALL 1
uint8_t startMethod;
uint8_t startMode;
uint8_t startButtonFlag;
uint8_t zbIsStarted[5];
uint8_t zbIsButtonPressed[5];

volatile uint16_t Timer150, Timer1, Timer2, TimerRelay;

int S_pressure;//cisnienie sprawdzania
int S_pressure_error;//jaki blad cisnienia mozliwy
int S_AV_REF;	//wartosc nbapiecia referencyjnego (tutaj 156 = 2,56 V) czyli razy 100 i - 100 by miec zakres wiekszy niz 255
int S_P_offset;	//wartosc dodawana lyb odejmowana od wartosci cisnienia by miec dobra wartosc zero jest na wartosci 100 mniejsze to odejmowanie

// Miganie diodami przy naciœnieciu klawisza
uint8_t ledButtonBlink[2];
//diody zielone
#define G1_ON  LATCH_CONTROL(1,7,1)
#define G1_OFF LATCH_CONTROL(1,7,0)
#define G2_ON  LATCH_CONTROL(2,5,1)
#define G2_OFF LATCH_CONTROL(2,5,0)
#define G3_ON  LATCH_CONTROL(2,6,1)
#define G3_OFF LATCH_CONTROL(2,6,0)
#define G4_ON  LATCH_CONTROL(2,7,1)
#define G4_OFF LATCH_CONTROL(2,7,0)
#define G5_ON  G5_PORT|=G5
#define G5_OFF G5_PORT&=~G5
//diody czerwone
#define R1_ON  LATCH_CONTROL(1,5,1)
#define R1_OFF LATCH_CONTROL(1,5,0)
#define R2_ON  LATCH_CONTROL(1,6,1)
#define R2_OFF LATCH_CONTROL(1,6,0)
#define R3_ON  LATCH_CONTROL(0,5,1)
#define R3_OFF LATCH_CONTROL(0,5,0)
#define R4_ON  LATCH_CONTROL(0,6,1)
#define R4_OFF LATCH_CONTROL(0,6,0)
#define R5_ON  LATCH_CONTROL(0,7,1)
#define R5_OFF LATCH_CONTROL(0,7,0)
void LED_STATUS_ON(char socket, char status);
void LED_STATUS_OFF(char socket);

//makro wlaczania zatrzaskow
#define LATCH_ON	BIT_EN_PORT|=BIT_EN
#define LATCH_OFF	BIT_EN_PORT&=~BIT_EN

//pierwszy blok
#define Z1A_ON  LATCH_CONTROL(2,0,1)
#define Z1A_OFF LATCH_CONTROL(2,0,0)
#define Z1B_ON  LATCH_CONTROL(2,1,1)
#define Z1B_OFF LATCH_CONTROL(2,1,0)
#define Z1C_ON  LATCH_CONTROL(2,2,1)
#define Z1C_OFF LATCH_CONTROL(2,2,0)
//drugi blok
#define Z2A_ON  LATCH_CONTROL(2,3,1)
#define Z2A_OFF LATCH_CONTROL(2,3,0)
#define Z2B_ON  LATCH_CONTROL(2,4,1)
#define Z2B_OFF LATCH_CONTROL(2,4,0)
#define Z2C_ON  LATCH_CONTROL(1,0,1)
#define Z2C_OFF LATCH_CONTROL(1,0,0)
//trzeci blok
#define Z3A_ON  LATCH_CONTROL(1,1,1)
#define Z3A_OFF LATCH_CONTROL(1,1,0)
#define Z3B_ON  LATCH_CONTROL(1,2,1)
#define Z3B_OFF LATCH_CONTROL(1,2,0)
#define Z3C_ON  LATCH_CONTROL(1,3,1)
#define Z3C_OFF LATCH_CONTROL(1,3,0)
//czwarty blok
#define Z4A_ON  LATCH_CONTROL(1,4,1)
#define Z4A_OFF LATCH_CONTROL(1,4,0)
#define Z4B_ON  LATCH_CONTROL(0,0,1)
#define Z4B_OFF LATCH_CONTROL(0,0,0)
#define Z4C_ON  LATCH_CONTROL(0,1,1)
#define Z4C_OFF LATCH_CONTROL(0,1,0)
//piaty blok
#define Z5A_ON  LATCH_CONTROL(0,2,1)
#define Z5A_OFF LATCH_CONTROL(0,2,0)
#define Z5B_ON  LATCH_CONTROL(0,3,1)
#define Z5B_OFF LATCH_CONTROL(0,3,0)
#define Z5C_ON  LATCH_CONTROL(0,4,1)
#define Z5C_OFF LATCH_CONTROL(0,4,0)

void LATCH_CONTROL(char adr, char bit, char on);

void BEEP_ERROR(void);
void BEEP_GOOD(void);

void EEPROM_BYTE_WRITE(unsigned int Address, unsigned char Data);
unsigned char EEPROM_BYTE_READ(unsigned int uiAddress);
void EEPROM_WRITE(unsigned int adress1, void * Data,short Data_length);
void EEPROM_READ(unsigned int adress1, void * Data,short Data_length);
void VALVE_CONTROL(char socket, char operate);

void CheckPressureMidWorking(void);


#endif /* COMMON_H_ */
