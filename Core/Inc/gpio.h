/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */



__attribute__((always_inline))
static inline unsigned char getLedState(void)
{
	if(LED_GPIO_Port->IDR & LED_Pin)
		return 1;
	else
		return 0;

}

__attribute__((always_inline))
static inline unsigned char getTpState(void)
{
	if(TP_GPIO_Port->IDR & TP_Pin)
		return 1;
	else
		return 0;

}

__attribute__((always_inline))
static inline void ledOn(void)
{
	LL_GPIO_SetOutputPin(LED_GPIO_Port,LED_Pin);
}

__attribute__((always_inline))
static inline void ledOff(void)
{
	LL_GPIO_ResetOutputPin(LED_GPIO_Port,LED_Pin);
}


__attribute__((always_inline))
static inline void ioExtLatchOff(void)
{
	LL_GPIO_ResetOutputPin(IO_LATCH_GPIO_Port,IO_LATCH_Pin);
}

__attribute__((always_inline))
static inline void ioExtLatchOn(void)
{
	LL_GPIO_SetOutputPin(IO_LATCH_GPIO_Port,IO_LATCH_Pin);
}

__attribute__((always_inline))
static inline void tpOn(void)
{
	LL_GPIO_SetOutputPin(TP_GPIO_Port,TP_Pin);
}

__attribute__((always_inline))
static inline void tpOff(void)
{
	LL_GPIO_ResetOutputPin(TP_GPIO_Port,TP_Pin);
}

__attribute__((always_inline))
static inline void tpToogle(void)
{
	LL_GPIO_TogglePin(TP_GPIO_Port,TP_Pin);
}

__attribute__((always_inline))
static inline void ledToogle(void)
{
	LL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
}

__attribute__((always_inline))
static inline unsigned char homeXisOn(void)
{
	if(HOME_X_GPIO_Port->IDR & HOME_X_Pin)
		return 1;
	else
		return 0;
}

__attribute__((always_inline))
static inline unsigned char homeYisOn(void)
{
	if(HOME_Y_GPIO_Port->IDR & HOME_Y_Pin)
		return 1;
	else
		return 0;
}

__attribute__((always_inline))
static inline unsigned char homeZisOn(void)
{
	if(HOME_Z_GPIO_Port->IDR & HOME_Z_Pin)
		return 1;
	else
		return 0;
}

__attribute__((always_inline))
static inline unsigned char homeTisOn(void)
{
	if(HOME_T_GPIO_Port->IDR & HOME_T_Pin)
		return 0;
	else
		return 1;
}

__attribute__((always_inline))
static inline void pumpOn(void)
{
	LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_7);
	LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_6);
}

__attribute__((always_inline))
static inline void pumpOff(void)
{
	LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_7);
	LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_6);
}



/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

