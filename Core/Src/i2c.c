/**
  ******************************************************************************
  * File Name          : I2C.c
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

#define DELAYMS_TIMEOUT 2
volatile unsigned int I2C_timeout = 0;

unsigned char I2CWrite(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned char addrReg, unsigned int size, unsigned char *data)
{
	LL_I2C_SetSlaveAddr(I2Cx, slaveAddr);

	LL_I2C_SetTransferSize(I2Cx, size+1);
	LL_I2C_SetTransferRequest(I2Cx,LL_I2C_REQUEST_WRITE);

	LL_I2C_GenerateStartCondition(I2Cx);

	I2C_timeout = DELAYMS_TIMEOUT;
	while(!LL_I2C_IsActiveFlag_TXIS(I2Cx))
	{
		if(!I2C_timeout)
			return 0;
	}
	LL_I2C_TransmitData8(I2Cx, addrReg);

	for(unsigned int i = 0; i<size; i++)
	{
		I2C_timeout = DELAYMS_TIMEOUT;
		while(!LL_I2C_IsActiveFlag_TXE(I2Cx))
		{
			if(!I2C_timeout)
				return 0;
		}
		LL_I2C_TransmitData8(I2Cx, data[i]);
	}
	//delay_us(200);
	return 1;

}

unsigned char I2CRead(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned char addrReg, unsigned int size, unsigned char *data)
{
	unsigned char ghost;
	I2CWrite(I2Cx,slaveAddr,addrReg,0, &ghost);
	I2C_timeout = DELAYMS_TIMEOUT;
	while(LL_I2C_IsActiveFlag_BUSY(I2Cx))
	{
		if(!I2C_timeout)
			return 0;
	}
	LL_I2C_SetTransferSize(I2Cx, size);
	LL_I2C_SetTransferRequest(I2Cx,LL_I2C_REQUEST_READ);
	LL_I2C_GenerateStartCondition(I2Cx);

	for(unsigned int i = 0; i<size; i++)
	{
		I2C_timeout = DELAYMS_TIMEOUT;
		while(!LL_I2C_IsActiveFlag_RXNE(I2Cx))
		{
			if(!I2C_timeout)
				return 0;
		}
		data[i] = LL_I2C_ReceiveData8(I2Cx);
	}
	while(LL_I2C_IsActiveFlag_BUSY(I2Cx))
	{
		/*if(!I2C_timeout)
			return 0;*/
	}
	//delay_us(100);
	return 1;
}

/*
 * return 0 if timeout occur.
 */
unsigned char I2CReadF(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned char addrReg, float *val)
{
	return I2CRead(I2Cx, slaveAddr, addrReg, 4, (unsigned char *)val);
}

/*
 * return 0 if timeout occur.
 */
unsigned char I2CWriteF(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned char addrReg, float val)
{
	return I2CWrite(I2Cx, slaveAddr, addrReg, 4, (unsigned char *)&val);
}



void myI2Cinit(void)
{
	LL_I2C_Disable(I2C3);
	LL_I2C_EnableAutoEndMode(I2C3);
	LL_I2C_Enable(I2C3);
}


/* USER CODE END 0 */

/* I2C2 init function */
void MX_I2C2_Init(void)
{
  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**I2C2 GPIO Configuration
  PC4   ------> I2C2_SCL
  PA8   ------> I2C2_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

  /** I2C Initialization
  */
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x30A0A7FB;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C2, &I2C_InitStruct);
  LL_I2C_EnableAutoEndMode(I2C2);
  LL_I2C_SetOwnAddress2(I2C2, 0, LL_I2C_OWNADDRESS2_NOMASK);
  LL_I2C_DisableOwnAddress2(I2C2);
  LL_I2C_DisableGeneralCall(I2C2);
  LL_I2C_EnableClockStretching(I2C2);

}
/* I2C3 init function */
void MX_I2C3_Init(void)
{
  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  /**I2C3 GPIO Configuration
  PC8   ------> I2C3_SCL
  PC9   ------> I2C3_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C3);

  /** I2C Initialization
  */
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x30A0A7FB;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C3, &I2C_InitStruct);
  LL_I2C_EnableAutoEndMode(I2C3);
  LL_I2C_SetOwnAddress2(I2C3, 0, LL_I2C_OWNADDRESS2_NOMASK);
  LL_I2C_DisableOwnAddress2(I2C3);
  LL_I2C_DisableGeneralCall(I2C3);
  LL_I2C_EnableClockStretching(I2C3);

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
