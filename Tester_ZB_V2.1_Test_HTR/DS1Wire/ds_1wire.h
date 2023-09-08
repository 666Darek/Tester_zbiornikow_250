/*
 * ds_1wire.h
 *
 *  Created on: 24 mar 2022
 *      Author: Dawid Sobków
 *
 *      1 Wire DS2431
 *      1024-Bit 1-Wire EEPROM
 */

#ifndef DS_1WIRE_H_
#define DS_1WIRE_H_

#include <avr/io.h>

enum ds1wSlots { OW_SLOT1, OW_SLOT2, OW_SLOT3, OW_SLOT4, OW_SLOT5 };

#define OW_IN   PIND
#define OW_OUT  PORTD
#define OW_DDR  DDRD

//Commands
#define SKIP_ROM 			 0xCC
#define WRITE_SCRATCHPAD	 0x0F
#define READ_SCRATCHPAD		 0xAA
#define COPY_SCRATCHPAD		 0x55
#define READ_MEMORY			 0xF0

//ErrorCodes
#define ERR_1W_ALLOK 	   1
#define ERR_1W_NOPRESENCE  10
#define ERR_1W_SERIALCRC   11
#define ERR_1W_COPYSTATUS  12
#define ERR_1W_COMPARE_DATA_FROM_SCRATCHPAD  13
#define ERR_1W_COMPARE_DATA_FROM_MEMORY		 14

uint8_t DS1W_READLINE(uint8_t slot);
void DS1W_SET_HIGH(uint8_t slot);
void DS1W_SET_LOW(uint8_t slot);

uint8_t DS1W_Reset(uint8_t slot);
void DS1W_SendBit(uint8_t slot, uint8_t bit);
uint8_t DS1W_ReadBit(uint8_t slot);
void DS1W_SendByte(uint8_t slot, uint8_t byte);
uint8_t DS1W_ReadByte(uint8_t slot);

unsigned char calcrc8(unsigned char *ptr, short count);

//
uint8_t DS1W_WriteDataToScratchpad(uint8_t slot, uint8_t *address, uint8_t *data);
uint8_t DS1W_ReadDataFromScratchpad(uint8_t slot, uint8_t *data);
uint8_t DS1W_CopyScratchpadToMemory(uint8_t slot, uint8_t *authorizationData);
uint8_t DS1W_ReadDataFromMemory(uint8_t slot, uint8_t *address, uint8_t *data, uint8_t dataLenght);

//Testy
uint8_t DS1W_ReadSerialAndChackCRC(uint8_t slot);
uint8_t DS1W_TestHTRWriteAndReadData(uint8_t slot);


#endif /* DS_1WIRE_H_ */
