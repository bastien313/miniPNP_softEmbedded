/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx.h"
#include "stm32g4xx_ll_i2c.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_crs.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_spi.h"
#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_usart.h"
#include "stm32g4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DECAY_T_Pin GPIO_PIN_13
#define DECAY_T_GPIO_Port GPIOC
#define FAULT_X_Pin GPIO_PIN_0
#define FAULT_X_GPIO_Port GPIOC
#define FAULT_Y_Pin GPIO_PIN_1
#define FAULT_Y_GPIO_Port GPIOC
#define FAULT_Z_Pin GPIO_PIN_2
#define FAULT_Z_GPIO_Port GPIOC
#define FAULT_T_Pin GPIO_PIN_3
#define FAULT_T_GPIO_Port GPIOC
#define DIR_X_Pin GPIO_PIN_1
#define DIR_X_GPIO_Port GPIOA
#define DIR_Y_Pin GPIO_PIN_2
#define DIR_Y_GPIO_Port GPIOA
#define DIR_Z_Pin GPIO_PIN_3
#define DIR_Z_GPIO_Port GPIOA
#define DIR_T_Pin GPIO_PIN_4
#define DIR_T_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOB
#define TP_Pin GPIO_PIN_1
#define TP_GPIO_Port GPIOB
#define LIMIT_X_Pin GPIO_PIN_10
#define LIMIT_X_GPIO_Port GPIOB
#define LIMIT_Y_Pin GPIO_PIN_11
#define LIMIT_Y_GPIO_Port GPIOB
#define LIMIT_Z_Pin GPIO_PIN_12
#define LIMIT_Z_GPIO_Port GPIOB
#define LIMIT_T_Pin GPIO_PIN_13
#define LIMIT_T_GPIO_Port GPIOB
#define IO_LATCH_Pin GPIO_PIN_12
#define IO_LATCH_GPIO_Port GPIOA
#define DECAY_X_Pin GPIO_PIN_10
#define DECAY_X_GPIO_Port GPIOC
#define DECAY_Y_Pin GPIO_PIN_11
#define DECAY_Y_GPIO_Port GPIOC
#define DECAY_Z_Pin GPIO_PIN_12
#define DECAY_Z_GPIO_Port GPIOC
#define HOME_T_Pin GPIO_PIN_5
#define HOME_T_GPIO_Port GPIOB
#define HOME_Z_Pin GPIO_PIN_6
#define HOME_Z_GPIO_Port GPIOB
#define HOME_Y_Pin GPIO_PIN_7
#define HOME_Y_GPIO_Port GPIOB
#define HOME_X_Pin GPIO_PIN_9
#define HOME_X_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef struct vector3d vector3d;
struct vector3d{
	float x;
	float y;
	float z;
};

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
