/*
 * feeder.c
 *
 *  Created on: Jun 4, 2020
 *      Author: bastien
 */

#include "feeder.h"
#include <string.h>
#include <stdio.h>

unsigned char feederlist[127] = {0}; // list of all feeder address.
unsigned char amountFeeder;


/*
 * Scan all address for find connected feeder.
 */
void feederScan(void)
{
	amountFeeder = 0;
	for(unsigned int i = 1; i<127; i++)
	{
		unsigned char val;
		feederlist[i] = 0;
		if(I2CRead(I2C_FEEDER, i << 1, FEEDER_STATUS, 1, &val))
		{
			feederlist[amountFeeder] = i << 1;
			amountFeeder++;
		}
	}
}


void feederLedOn(unsigned char addrF)
{
	unsigned char ctrlValue;
	I2CRead(I2C_FEEDER, addrF, FEEDER_CTRL, 1, &ctrlValue);
	ctrlValue |= LED_BIT;
	I2CWrite(I2C_FEEDER, addrF, FEEDER_CTRL, 1, &ctrlValue);
}

void feederLedOff(unsigned char addrF)
{
	unsigned char ctrlValue;
	I2CRead(I2C_FEEDER, addrF, FEEDER_CTRL, 1, &ctrlValue);
	ctrlValue &= (~LED_BIT);
	I2CWrite(I2C_FEEDER, addrF, FEEDER_CTRL, 1, &ctrlValue);
}



unsigned char feederGetStatus(unsigned char addrF)
{
	unsigned char stat = 0;
	I2CRead(I2C_FEEDER, addrF, FEEDER_STATUS, 1, &stat);
	return stat;
}

unsigned char feederIsBusy(unsigned char addrF)
{
	if(feederGetStatus(addrF) & BUSY_BIT)
		return 1;
	else
		return 0;
}

void feederFlashSave(unsigned char addrF)
{
	while(feederIsBusy(addrF));
	unsigned char ctrlValue;
	I2CRead(I2C_FEEDER, addrF, FEEDER_CTRL, 1, &ctrlValue);
	ctrlValue |= FLASH_BIT;
	I2CWrite(I2C_FEEDER, addrF, FEEDER_CTRL, 1, &ctrlValue);
}

void feederSendStep(unsigned char addrF)
{
	while(feederIsBusy(addrF));
	unsigned char ctrlValue;
	I2CRead(I2C_FEEDER, addrF, FEEDER_CTRL, 1, &ctrlValue);
	ctrlValue |= STEP_BIT;
	I2CWrite(I2C_FEEDER, addrF, FEEDER_CTRL, 1, &ctrlValue);
}

void feederGetName(unsigned char addrF, char *buffer)
{
	unsigned char sizeName = 0;
	*buffer = 0;
	if(I2CRead(I2C_FEEDER, addrF, FEEDER_SIZENAME, 1, &sizeName))
	{
		if(sizeName)
		{
			I2CRead(I2C_FEEDER, addrF, FEEDER_NAME, sizeName, (unsigned char*)buffer);
			buffer[sizeName] = 0;
		}

		else
		{

			buffer[1] = 0;
		}


	}

}

void feederSetName(unsigned char addrF, char *buffer)
{
	unsigned char sizeName = 0;
	sizeName = strlen(buffer);
	buffer[sizeName] = 0;
	I2CWrite(I2C_FEEDER, addrF, FEEDER_NAME, sizeName+1, (unsigned char*)buffer);
}



void feederGetPos(unsigned char addrF, vector3d *pos)
{
	I2CReadF(I2C_FEEDER, addrF, FEEDER_POSX, &pos->x);
	I2CReadF(I2C_FEEDER, addrF, FEEDER_POSY, &pos->y);
	I2CReadF(I2C_FEEDER, addrF, FEEDER_POSZ, &pos->z);
}

void feederSetPos(unsigned char addrF, vector3d pos)
{
	I2CWriteF(I2C_FEEDER, addrF, FEEDER_POSX, pos.x);
	I2CWriteF(I2C_FEEDER, addrF, FEEDER_POSY, pos.y);
	I2CWriteF(I2C_FEEDER, addrF, FEEDER_POSZ, pos.z);
}

void feederSetStepSize(unsigned char addrF, unsigned char val)
{
	I2CWrite(I2C_FEEDER, addrF, FEEDER_STPSIZE, 1, &val);
}

unsigned char feederGetStepSize(unsigned char addrF)
{
	unsigned char val;
	I2CRead(I2C_FEEDER, addrF, FEEDER_STPSIZE, 1, &val);
	return val;
}

void feederSetCMPB(unsigned char addrF, unsigned char val)
{
	I2CWrite(I2C_FEEDER, addrF, FEEDER_CMPBS, 1, &val);
}

unsigned char feederGetCMPB(unsigned char addrF)
{
	unsigned char val;
	I2CRead(I2C_FEEDER, addrF, FEEDER_CMPBS, 1, &val);
	return val;
}
