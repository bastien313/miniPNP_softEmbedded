/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

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

#endif /* __SPI_H__ */

