/**
  ******************************************************************************
  * File Name          : SPI.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */
void mySpiInit(void);

__attribute__((always_inline))
static inline void spiIOSend(unsigned int data)
{
	while(!(SPI1->SR & SPI_SR_TXE));
	LL_SPI_TransmitData8(SPI1, (data & 0x00FF0000) >> 16);
	while(!(SPI1->SR & SPI_SR_TXE));
	LL_SPI_TransmitData8(SPI1, (data & 0x0000FF00) >> 8);
	while(!(SPI1->SR & SPI_SR_TXE));
	LL_SPI_TransmitData8(SPI1, data & 0x000000FF);
}


__attribute__((always_inline))
static inline unsigned char spiIoIsBusy(void)
{
	if(SPI1->SR & SPI_SR_BSY)
		return 1;
	else
		return 0;
}

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
