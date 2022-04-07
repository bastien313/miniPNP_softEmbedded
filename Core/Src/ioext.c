/*
 * ioext.c
 *
 *  Created on: 8 oct. 2019
 *      Author: bastien
 */


unsigned int actualState;

#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "ioext.h"





void updateIo(unsigned int value)
{
	ioExtLatchOff();
	delay_us(1);
	spiIOSend(value);
	actualState = value;
	while(spiIoIsBusy());
	ioExtLatchOn();
	delay_us(10);
	ioExtLatchOff();
}

void initExtIo(void)
{
	actualState = 0x00000000;
	actualState |= (EN_X|EN_Y|EN_Z|EN_T);
	ioExtLatchOff();
	updateIo(actualState);
}

void driverEnable(drvChan drv)
{
	if(drv == X)
		resetIo(EN_X);
	else if(drv == Y)
		resetIo(EN_Y);
	else if(drv == Z)
		resetIo(EN_Z);
	else if(drv == T)
		resetIo(EN_T);
}

void driverDisable(drvChan drv)
{
	if(drv == X)
		setIo(EN_X);
	else if(drv == Y)
		setIo(EN_Y);
	else if(drv == Z)
		setIo(EN_Z);
	else if(drv == T)
		setIo(EN_T);
}

void driverResetOn(drvChan drv)
{
	if(drv == X)
		resetIo(RST_X);
	else if(drv == Y)
		resetIo(RST_Y);
	else if(drv == Z)
		resetIo(RST_Z);
	else if(drv == T)
		resetIo(RST_T);
}

void driverResetOff(drvChan drv)
{
	if(drv == X)
		setIo(RST_X);
	else if(drv == Y)
		setIo(RST_Y);
	else if(drv == Z)
		setIo(RST_Z);
	else if(drv == T)
		setIo(RST_T);
}



void setStepDiv(drvChan drv, stepDiv div)
{
	unsigned int mask;
	unsigned int rolValue;

	if(drv == X)
	{
		mask = 	MOD0_X + MOD1_X + MOD2_X;
		rolValue = 0;
	}
	else if(drv == Y)
	{
		mask = 	MOD0_Y + MOD1_Y + MOD2_Y;
		rolValue = 5;
	}
	else if(drv == Z)
	{
		mask = 	MOD0_Z + MOD1_Z + MOD2_Z;
		rolValue = 10;
	}
	else if(drv == T)
	{
		mask = 	MOD0_T + MOD1_T + MOD2_T;
		rolValue = 15;
	}

	actualState &= (~mask);
	actualState |= (div << rolValue);

	updateIo(actualState);
}

void driverDir(drvChan drv, int dir)
{
	if(drv == X)
	{
		if(dir == -1)
			DIR_X_GPIO_Port->ODR |= DIR_X_Pin;
		else
			DIR_X_GPIO_Port->ODR &= (~DIR_X_Pin);
	}
	else if(drv == Y)
	{
		if(dir == -1)
			DIR_Y_GPIO_Port->ODR |= DIR_Y_Pin;
		else
			DIR_Y_GPIO_Port->ODR &= (~DIR_Y_Pin);
	}
	else if(drv == Z)
	{
		if(dir == -1)
			DIR_Z_GPIO_Port->ODR |= DIR_Z_Pin;
		else
			DIR_Z_GPIO_Port->ODR &= (~DIR_Z_Pin);
	}
	else if(drv == T)
	{
		if(dir == -1)
			DIR_T_GPIO_Port->ODR |= DIR_T_Pin;
		else
			DIR_T_GPIO_Port->ODR &= (~DIR_T_Pin);
	}

}

/*
 * Set Decay mode of PinSel.
 * Slow = 0, Fast = 1, Mixed = Hi.
 */
void setDecayMode(drvChan drv,decayMode mode)
{
	GPIO_TypeDef *portSel;
	uint32_t pinSel;

	if(drv == X)
	{
		pinSel = DECAY_X_Pin;
		portSel = DECAY_X_GPIO_Port;
	}
	else if(drv == Y)
	{
		pinSel = DECAY_Y_Pin;
		portSel = DECAY_Y_GPIO_Port;
	}
	else if(drv == Z)
	{
		pinSel = DECAY_Z_Pin;
		portSel = DECAY_Z_GPIO_Port;
	}
	else if(drv == T)
	{
		pinSel = DECAY_T_Pin;
		portSel = DECAY_T_GPIO_Port;
	}

	if(mode == slow)
	{
		LL_GPIO_SetPinMode(portSel, pinSel, LL_GPIO_MODE_OUTPUT);
		LL_GPIO_ResetOutputPin(portSel, pinSel);
	}
	else if(mode == fast)
	{
		LL_GPIO_SetPinMode(portSel, pinSel, LL_GPIO_MODE_OUTPUT);
		LL_GPIO_SetOutputPin(portSel, pinSel);
	}
	else if(mode == mixed)
	{
		LL_GPIO_SetPinMode(portSel, pinSel, LL_GPIO_MODE_INPUT);
	}

}

