/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C2_Init(void);
void MX_I2C3_Init(void);

/* USER CODE BEGIN Prototypes */
unsigned char I2CWrite(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned char addrReg, unsigned int size, unsigned char *data);
unsigned char I2CSimpleRead(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned int size, unsigned char *data);
unsigned char I2CRead(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned char addrReg, unsigned int size, unsigned char *data);
unsigned char I2CReadF(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned char addrReg, float *val);
unsigned char I2CWriteF(I2C_TypeDef *I2Cx,unsigned char slaveAddr, unsigned char addrReg, float val);
void myI2Cinit(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

