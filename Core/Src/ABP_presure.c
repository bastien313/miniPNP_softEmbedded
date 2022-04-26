/*
 * ABP_presure.c
 *
 *  Created on: 18 avr. 2022
 *      Author: bastien
 */

#include "ABP_presure.h"


const uint8_t ABP_ADDR = 0x50;//0x28;


void ABP_Init(ABP_presure *dev, I2C_TypeDef *i2cPort, float pMax, float pMin)
{
  dev->_i2cPort = i2cPort; //Grab which port the user wants us to use
  dev->_presureMax = pMax;
  dev->_presureMin = pMin;
}

ABPError_e ABP_readPresureNum(ABP_presure *dev, uint16_t *data)
{
	uint8_t dataRead[2];
	ABPError_e err;
	err = (I2CSimpleRead(dev->_i2cPort ,ABP_ADDR, 2, dataRead))? ABP_ERROR_OK : ABP_ERROR_TIMEOUT;
	*data = (((uint16_t)(dataRead[0] & 0x3F)) << 8) | dataRead[1];
	return err;
}

ABPError_e ABP_readPresureFloat(ABP_presure *dev, float *data)
{
	uint16_t readData;
	ABPError_e err;
	err = ABP_readPresureNum(dev, &readData);
	float fData = (float)readData;
	*data = (((fData - 1638.0) * (dev->_presureMax - dev->_presureMin)) / (14745.0 - 1638.0)) + dev->_presureMin;
	return err;
}

