/*
 * ABP_presure.h
 *
 *  Created on: 18 avr. 2022
 *      Author: bastien
 */

#ifndef INC_ABP_PRESURE_H_
#define INC_ABP_PRESURE_H_
#include "i2c.h"

typedef enum {
	ABP_ERROR_OK = 0,               // Everything executed normally
	ABP_ERROR_TIMEOUT
} ABPError_e;

typedef struct ABP_presure ABP_presure;
struct ABP_presure{
	I2C_TypeDef *_i2cPort; //The generic connection to user's chosen I2C hardware
	float _presureMax;
	float _presureMin;
};

void ABP_Init(ABP_presure *dev, I2C_TypeDef *i2cPort, float pMax, float pMin);
ABPError_e ABP_readPresureNum(ABP_presure *dev, uint16_t *data);
ABPError_e ABP_readPresureFloat(ABP_presure *dev, float *data);
#endif /* INC_ABP_PRESURE_H_ */
