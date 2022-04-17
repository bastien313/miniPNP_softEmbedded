/*
Copyright (c) 2012 Vishay GmbH, www.vishay.com
author: DS, version 1.21

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "VCNL40x0.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_Init(VCNL40x0 *dev, I2C_TypeDef *I2Cx)
{
	dev->_i2c = I2Cx;
	return VCNL40x0_ERROR_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetCommandRegister (VCNL40x0 *dev, unsigned char Command)
{
	if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_COMMAND, 1, &Command))
		return VCNL40x0_ERROR_TIMEOUT;
	return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadCommandRegister (VCNL40x0 *dev, unsigned char *Command)
{
	if(!I2CRead(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_COMMAND, 1, Command))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadID (VCNL40x0 *dev, unsigned char *ID)
{
	if(!I2CRead(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_ID, 1, ID))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetCurrent (VCNL40x0 *dev, unsigned char Current)
{
	if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_PROX_CURRENT, 1, &Current))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadCurrent (VCNL40x0 *dev, unsigned char *Current)
{
	if(!I2CRead(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_PROX_CURRENT, 1, Current))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetProximityRate (VCNL40x0 *dev, unsigned char ProximityRate)
{
	if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_PROX_RATE, 1, &ProximityRate))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetAmbiConfiguration (VCNL40x0 *dev, unsigned char AmbiConfiguration)
{
	if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_AMBI_PARAMETER, 1, &AmbiConfiguration))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetInterruptControl (VCNL40x0 *dev, unsigned char InterruptControl)
{
	if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_INTERRUPT_CONTROL, 1, &InterruptControl))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadInterruptControl (VCNL40x0 *dev, unsigned char *InterruptControl)
{
	if(!I2CRead(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_INTERRUPT_CONTROL, 1, InterruptControl))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetInterruptStatus (VCNL40x0 *dev, unsigned char InterruptStatus)
{
	if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_INTERRUPT_STATUS, 1, &InterruptStatus))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetModulatorTimingAdjustment (VCNL40x0 *dev, unsigned char ModulatorTimingAdjustment)
{
	if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_PROX_TIMING, 1, &ModulatorTimingAdjustment))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadInterruptStatus (VCNL40x0 *dev, unsigned char *InterruptStatus)
{
	if(!I2CRead(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_INTERRUPT_STATUS, 1, InterruptStatus))
		return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadProxiValue (VCNL40x0 *dev, unsigned int *ProxiValue)
{
	unsigned char _receive[2];
	if(!I2CRead(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_PROX_VALUE, 2, _receive))
		return VCNL40x0_ERROR_TIMEOUT;
    *ProxiValue = ((unsigned int)_receive[0] << 8 | (unsigned char)_receive[1]);
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadAmbiValue (VCNL40x0 *dev, unsigned int *AmbiValue)
{
	unsigned char _receive[2];
	if(!I2CRead(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_AMBI_VALUE, 2, _receive))
		return VCNL40x0_ERROR_TIMEOUT;
    *AmbiValue = ((unsigned int)_receive[0] << 8 | (unsigned char)_receive[1]);
    return VCNL40x0_ERROR_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetLowThreshold (VCNL40x0 *dev, unsigned int LowThreshold)
{
    unsigned char LoByte=0, HiByte=0;
    LoByte = (unsigned char)(LowThreshold & 0x00ff);
    HiByte = (unsigned char)((LowThreshold & 0xff00)>>8);
    
    if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_INTERRUPT_LOW_THRES, 1, &HiByte))
    	return VCNL40x0_ERROR_TIMEOUT;
    if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_INTERRUPT_LOW_THRES+1, 1, &LoByte))
    	return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_SetHighThreshold (VCNL40x0 *dev, unsigned int HighThreshold)
{
    unsigned char LoByte=0, HiByte=0;
    
    LoByte = (unsigned char)(HighThreshold & 0x00ff);
    HiByte = (unsigned char)((HighThreshold & 0xff00)>>8);
    
    if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_INTERRUPT_HIGH_THRES, 1, &HiByte))
    	return VCNL40x0_ERROR_TIMEOUT;
    if(!I2CWrite(dev->_i2c, VCNL40x0_ADDRESS, REGISTER_INTERRUPT_HIGH_THRES+1, 1, &LoByte))
    	return VCNL40x0_ERROR_TIMEOUT;
    return VCNL40x0_ERROR_OK;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadProxiOnDemand (VCNL40x0 *dev, unsigned int *ProxiValue)
{
    unsigned char Command=0;
    VCNL40x0Error_e err = VCNL40x0_ERROR_OK;

    // enable prox value on demand
    err = VCNL40x0_SetCommandRegister(dev, COMMAND_PROX_ENABLE | COMMAND_PROX_ON_DEMAND) ;
    if(err != VCNL40x0_ERROR_OK)
    	return err;
 
    // wait on prox data ready bit
    do {
    	err = VCNL40x0_ReadCommandRegister(dev, &Command);
        if(err != VCNL40x0_ERROR_OK)
        	return err;// read command register
    } while (!(Command & COMMAND_MASK_PROX_DATA_READY));

    err = VCNL40x0_ReadProxiValue(dev, ProxiValue);
    if(err != VCNL40x0_ERROR_OK)
    	return err;// read prox value

    err = VCNL40x0_SetCommandRegister(dev, COMMAND_ALL_DISABLE);               // stop prox value on demand
    
    return err;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VCNL40x0Error_e VCNL40x0_ReadAmbiOnDemand (VCNL40x0 *dev, unsigned int *AmbiValue)
{
    unsigned char Command=0;
    VCNL40x0Error_e err = VCNL40x0_ERROR_OK;

    // enable ambi value on demand
    err = VCNL40x0_SetCommandRegister(dev, COMMAND_PROX_ENABLE | COMMAND_AMBI_ON_DEMAND);
    if(err != VCNL40x0_ERROR_OK)
    	return err;
    // wait on ambi data ready bit
    do {
    	err = VCNL40x0_ReadCommandRegister(dev, &Command);
        if(err != VCNL40x0_ERROR_OK)
        	return err;// read command register
    } while (!(Command & COMMAND_MASK_AMBI_DATA_READY));

    err = VCNL40x0_ReadAmbiValue(dev, AmbiValue);
    if(err != VCNL40x0_ERROR_OK)
    	return err;// read ambi value

    err = VCNL40x0_SetCommandRegister(dev, COMMAND_ALL_DISABLE);               // stop ambi value on demand

    return err;
}
