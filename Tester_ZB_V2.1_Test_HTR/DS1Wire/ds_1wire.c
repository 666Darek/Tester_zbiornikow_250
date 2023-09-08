/*
 * ds_1wire.c
 *
 *  Created on: 24 mar 2022
 *      Author: Dawid Sobków
 *
 *      1 Wire DS2431
 *      1024-Bit 1-Wire EEPROM
 */

#include "../DS1Wire/ds_1wire.h"

#include <avr/io.h>
#include <util/delay.h>

enum ds1wSlots ds1wSlot;

uint8_t DS1W_READLINE(uint8_t slot)
{
	OW_DDR &= (~(1 << slot ));
	uint8_t state = OW_IN & (1<<slot);

	if(state)
		return 1;
	else
		return 0;
}

void DS1W_SET_HIGH(uint8_t slot)
{
	OW_OUT |= (1 << slot);
	OW_DDR &= (~(1 << slot ));
}

void DS1W_SET_LOW(uint8_t slot)
{
	OW_OUT &= (~(1 << slot));
	OW_DDR |= (1 << slot);
}

uint8_t DS1W_Reset(uint8_t slot)
{
	uint8_t presence;

	DS1W_SET_LOW(slot);
	_delay_us(500);
	DS1W_SET_HIGH(slot);
	_delay_us(63);//BY£O 30

	presence = DS1W_READLINE(slot);

	_delay_us(470);

	if(presence == 0 && DS1W_READLINE(slot))
		presence = 1;
	else
		presence = 0;

	return presence;
}

void DS1W_SendBit(uint8_t slot, uint8_t bit)
{
	DS1W_SET_LOW(slot);
	_delay_us(8);

	if(bit==1)
		DS1W_SET_HIGH(slot);

	_delay_us(80);
	DS1W_SET_HIGH(slot);
}

uint8_t DS1W_Read(uint8_t slot)
{
	DS1W_SET_LOW(slot);
	_delay_us(8);
	DS1W_SET_HIGH(slot);
	_delay_us(15);

	if(DS1W_READLINE(slot))
		return 1;
	else
		return 0;
}

void DS1W_SendByte(uint8_t slot, uint8_t byte)
{
	uint8_t pom;

	for(uint8_t i = 0; i<8; i++)
	{
		pom = byte>>i;
		pom &= 0x01;
		DS1W_SendBit(slot, pom);
	}

	_delay_us(100);//100
}

uint8_t DS1W_ReadByte(uint8_t slot)
{
	uint8_t byte=0;

	for(uint8_t i = 0; i<8; i++)
	{
		if(DS1W_Read(slot)) byte|=0x01<<i;
		_delay_us(15);
	}

	return byte;
}

unsigned char calcrc8(unsigned char *ptr, short count)
{//Polynominal: X^8+X^5+X^4+1
  unsigned char TempVal;
  unsigned char CRC=0;
  unsigned char f=0;
  unsigned char i,j=0;
  for(;j<count;j++)
  {
    TempVal=ptr[j];
    for (i = 0; i < 8; ++i)
    {
        f = (TempVal ^ CRC) & 1;
        CRC >>= 1;
        TempVal >>= 1;
        if (f) CRC ^= 0x8C;
    }
  }
  return CRC;
}

uint8_t DS1W_WriteDataToScratchpad(uint8_t slot, uint8_t *address, uint8_t *data)
{
	if(DS1W_Reset(slot) == 0)
		return ERR_1W_NOPRESENCE;

	DS1W_SendByte(slot, SKIP_ROM);
	DS1W_SendByte(slot, WRITE_SCRATCHPAD);
	DS1W_SendByte(slot, address[0]);
	DS1W_SendByte(slot, address[1]);

	for(uint8_t i=0; i<8; i++){
		DS1W_SendByte(slot, data[i]);
	}


	DS1W_ReadByte(slot);
	DS1W_ReadByte(slot);

	return ERR_1W_ALLOK;
}

uint8_t DS1W_ReadDataFromScratchpad(uint8_t slot, uint8_t *data)
{
	if(DS1W_Reset(slot) == 0)
		return ERR_1W_NOPRESENCE;

	DS1W_SendByte(slot, SKIP_ROM);
	DS1W_SendByte(slot, READ_SCRATCHPAD);

	for(uint8_t i=0; i<13; i++){
		data[i] = DS1W_ReadByte(slot);
	}


	return ERR_1W_ALLOK;
}

uint8_t DS1W_CopyScratchpadToMemory(uint8_t slot, uint8_t *authorizationData)
{
	if(DS1W_Reset(slot) == 0)
		return ERR_1W_NOPRESENCE;

	DS1W_SendByte(slot, SKIP_ROM);
	DS1W_SendByte(slot, COPY_SCRATCHPAD);

	for(uint8_t i=0; i<3; i++){
		DS1W_SendByte(slot, authorizationData[i]);
	}

	_delay_ms(13); //Wait 13 ms for the copy function to complete

	uint8_t copyStatus = DS1W_ReadByte(slot); // AAh = succes

	if(copyStatus == 0xAA)
		copyStatus = ERR_1W_ALLOK;
	else
		copyStatus = ERR_1W_COPYSTATUS;

	return copyStatus;
}

uint8_t DS1W_ReadDataFromMemory(uint8_t slot, uint8_t *address, uint8_t *data, uint8_t dataLenght)
{
	if(DS1W_Reset(slot) == 0)
		return ERR_1W_NOPRESENCE;

	DS1W_SendByte(slot, SKIP_ROM);
	DS1W_SendByte(slot, READ_MEMORY);
	DS1W_SendByte(slot, address[0]);
	DS1W_SendByte(slot, address[1]);

	for(uint8_t i=0; i<dataLenght; i++){
		data[i] = DS1W_ReadByte(slot);
	}

	return ERR_1W_ALLOK;
}


uint8_t DS1W_ReadSerialAndChackCRC(uint8_t slot)
{
	if(DS1W_Reset(slot) == 0)
		return ERR_1W_NOPRESENCE;

	uint8_t read[8];

	DS1W_SendByte(slot, 0x33);

	for(uint8_t i=0; i<8; i++){
		read[i] = DS1W_ReadByte(slot);
	}

	uint8_t myCRC =  calcrc8(read, 7);

	if(read[7] == myCRC)
		return ERR_1W_ALLOK;
	else
		return ERR_1W_SERIALCRC;
}

uint8_t DS1W_TestHTRWriteAndReadData(uint8_t slot)
{
	uint8_t result = 0;
	uint8_t addr[3] = {0,0,0};
	uint8_t data[8] = {10,20,30,40,50,60,70,80};
	result = DS1W_WriteDataToScratchpad(slot, addr, data);

	if(result != ERR_1W_ALLOK) return result;

	uint8_t dataR[13] = {};
	result = DS1W_ReadDataFromScratchpad(slot, dataR);

	if(result != ERR_1W_ALLOK) return result;

	uint8_t readDataRes = ERR_1W_ALLOK;
	for(uint8_t i=0; i<8; i++){
		if(data[i] != dataR[i+3])
			readDataRes = ERR_1W_COMPARE_DATA_FROM_SCRATCHPAD;
	}

	if(readDataRes != ERR_1W_ALLOK) return readDataRes;

	addr[2] = 0x07;
	uint8_t copyStatus = DS1W_CopyScratchpadToMemory(slot, addr);

	if(copyStatus != ERR_1W_ALLOK) return copyStatus;

	result = DS1W_ReadDataFromMemory(slot, addr, dataR, 8);

	if(result != ERR_1W_ALLOK) return result;

	uint8_t readDataRam = ERR_1W_ALLOK;
	for(uint8_t i=0; i<8; i++){
		if(data[i] != dataR[i])
			readDataRam = ERR_1W_COMPARE_DATA_FROM_MEMORY;
	}

	if(readDataRam != ERR_1W_ALLOK) return readDataRam;

	//Clear ram
	uint8_t dataN[8] = {};
	DS1W_WriteDataToScratchpad(slot, addr, dataN);
	DS1W_CopyScratchpadToMemory(slot, addr);
	DS1W_ReadDataFromMemory(slot, addr, dataR, 8);

	readDataRam = ERR_1W_ALLOK;
	for(uint8_t i=0; i<8; i++){
		if(dataN[i] != dataR[i])
			readDataRam = ERR_1W_COMPARE_DATA_FROM_MEMORY;
	}

	if(readDataRam != ERR_1W_ALLOK) return readDataRam;

	return ERR_1W_ALLOK;
}










