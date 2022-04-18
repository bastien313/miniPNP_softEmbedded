#ifndef VCNL3040_H
#define VCNL3040_H


#include <stdint.h>
#include "i2c.h"


/* OK */
static const uint8_t VCNL3040_PS_DUTY_MASK = (uint8_t)~((1 << 7) | (1 << 6));
static const uint8_t VCNL3040_PS_DUTY_40 = 0;
static const uint8_t VCNL3040_PS_DUTY_80 = (1 << 6);
static const uint8_t VCNL3040_PS_DUTY_160 = (1 << 7);
static const uint8_t VCNL3040_PS_DUTY_320 = (1 << 7) | (1 << 6);

/* OK */
static const uint8_t VCNL3040_PS_PERS_MASK = (uint8_t)~((1 << 5) | (1 << 4));
static const uint8_t VCNL3040_PS_PERS_1 = 0;
static const uint8_t VCNL3040_PS_PERS_2 = (1 << 4);
static const uint8_t VCNL3040_PS_PERS_3 = (1 << 5);
static const uint8_t VCNL3040_PS_PERS_4 = (1 << 5) | (1 << 4);

/* OK */
static const uint8_t VCNL3040_PS_IT_MASK = (uint8_t)~((1 << 3) | (1 << 2) | (1 << 1));
static const uint8_t VCNL3040_PS_IT_1T = 0;
static const uint8_t VCNL3040_PS_IT_15T = (1 << 1);
static const uint8_t VCNL3040_PS_IT_2T = (1 << 2);
static const uint8_t VCNL3040_PS_IT_25T = (1 << 2) | (1 << 1);
static const uint8_t VCNL3040_PS_IT_3T = (1 << 3);
static const uint8_t VCNL3040_PS_IT_35T = (1 << 3) | (1 << 1);
static const uint8_t VCNL3040_PS_IT_4T = (1 << 3) | (1 << 2);
static const uint8_t VCNL3040_PS_IT_8T = (1 << 3) | (1 << 2) | (1 << 1);

/* OK */
static const uint8_t VCNL3040_PS_SD_MASK = (uint8_t)~((1 << 0));
static const uint8_t VCNL3040_PS_SD_POWER_ON = 0;
static const uint8_t VCNL3040_PS_SD_POWER_OFF = (1 << 0);

/* OK */
static const uint8_t VCNL3040_PS_HD_MASK = (uint8_t)~((1 << 3));
static const uint8_t VCNL3040_PS_HD_12_BIT = 0;
static const uint8_t VCNL3040_PS_HD_16_BIT = (1 << 3);

/* OK */
static const uint8_t VCNL3040_PS_INT_MASK = (uint8_t)~((1 << 1) | (1 << 0));
static const uint8_t VCNL3040_PS_INT_DISABLE = 0;
static const uint8_t VCNL3040_PS_INT_CLOSE = (1 << 0);
static const uint8_t VCNL3040_PS_INT_AWAY = (1 << 1);
static const uint8_t VCNL3040_PS_INT_BOTH = (1 << 1) | (1 << 0);

/* OK */
static const uint8_t VCNL3040_PS_SMART_PERS_MASK = (uint8_t)~((1 << 4));
static const uint8_t VCNL3040_PS_SMART_PERS_DISABLE = 0;
static const uint8_t VCNL3040_PS_SMART_PERS_ENABLE = (1 << 1);
/* OK */
static const uint8_t VCNL3040_PS_FOR_MASK = (uint8_t)~((1 << 3));
static const uint8_t VCNL3040_PS_FOR_DISABLE = 0;
static const uint8_t VCNL3040_PS_FOR_ENABLE = (1 << 3);
/* OK */
static const uint8_t VCNL3040_PS_FOR_TRIG_MASK = (uint8_t)~((1 << 2));
static const uint8_t VCNL3040_PS_FOR_TRIG_ONETIME = (1 << 2);
static const uint8_t VCNL3040_PS_FOR_TRIG_NOPS = 0;

/*
static const uint8_t VCNL4040_WHITE_EN_MASK = (uint8_t)~((1 << 7));
static const uint8_t VCNL4040_WHITE_ENABLE = 0;
static const uint8_t VCNL4040_WHITE_DISABLE = (1 << 7);
*/

/* OK */
static const uint8_t VCNL3040_PS_MS_MASK = (uint8_t)~((1 << 6));
static const uint8_t VCNL3040_PS_MS_DISABLE = 0;
static const uint8_t VCNL3040_PS_MS_ENABLE = (1 << 6);

/* OK */
static const uint8_t VCNL3040_LED_I_MASK = (uint8_t)~((1 << 2) | (1 << 1) | (1 << 0));
static const uint8_t VCNL3040_LED_50MA = 0;
static const uint8_t VCNL3040_LED_75MA = (1 << 0);
static const uint8_t VCNL3040_LED_100MA = (1 << 1);
static const uint8_t VCNL3040_LED_120MA = (1 << 1) | (1 << 0);
static const uint8_t VCNL3040_LED_140MA = (1 << 2);
static const uint8_t VCNL3040_LED_160MA = (1 << 2) | (1 << 0);
static const uint8_t VCNL3040_LED_180MA = (1 << 2) | (1 << 1);
static const uint8_t VCNL3040_LED_200MA = (1 << 2) | (1 << 1) | (1 << 0);

/* OK */
//static const uint8_t VCNL4040_INT_FLAG_ALS_LOW = (1 << 5);
//static const uint8_t VCNL4040_INT_FLAG_ALS_HIGH = (1 << 4);
static const uint8_t VCNL3040_INT_FLAG_CLOSE = (1 << 1);
static const uint8_t VCNL3040_INT_FLAG_AWAY = (1 << 0);

typedef enum {
    VCNL3040_ERROR_OK = 0,               // Everything executed normally
	VCNL3040_ERROR_TIMEOUT,
	VCNL3040_ERROR_WRONG_ID

} VCNL3040Error_e;

typedef struct VCNL3040 VCNL3040;
struct VCNL3040{
	I2C_TypeDef *_i2cPort; //The generic connection to user's chosen I2C hardware
};

VCNL3040Error_e VCNL3040_Init(VCNL3040 *dev, I2C_TypeDef *i2cPort);
unsigned int VCNL3040_isConnected(VCNL3040 *dev); //True if sensor responded to I2C query

VCNL3040Error_e VCNL3040_setIRDutyCycle(VCNL3040 *dev, uint16_t dutyValue);

VCNL3040Error_e VCNL3040_setProxInterruptPersistance(VCNL3040 *dev, uint8_t persValue);
//void setAmbientInterruptPersistance(uint8_t persValue);

VCNL3040Error_e VCNL3040_setProxIntegrationTime(VCNL3040 *dev, uint8_t timeValue); //Sets the integration time for the proximity sensor
//void setAmbientIntegrationTime(uint16_t timeValue);

VCNL3040Error_e VCNL3040_powerOnProximity(VCNL3040 *dev); //Power on the prox sensing portion of the device
VCNL3040Error_e VCNL3040_powerOffProximity(VCNL3040 *dev); //Power off the prox sensing portion of the device

//void powerOnAmbient(void); //Power on the ALS sensing portion of the device
//void powerOffAmbient(void); //Power off the ALS sensing portion of the device

VCNL3040Error_e VCNL3040_setProxResolution(VCNL3040 *dev, uint8_t resolutionValue);//Sets the proximity resolution to 12 or 16 bit

//void enableAmbientInterrupts(void);
//void disableAmbientInterrupts(void);

VCNL3040Error_e VCNL3040_enableSmartPersistance(VCNL3040 *dev);
VCNL3040Error_e VCNL3040_disableSmartPersistance(VCNL3040 *dev);

VCNL3040Error_e VCNL3040_enableActiveForceMode(VCNL3040 *dev);
VCNL3040Error_e VCNL3040_disableActiveForceMode(VCNL3040 *dev);
VCNL3040Error_e VCNL3040_takeSingleProxMeasurement(VCNL3040 *dev);

//void enableWhiteChannel(void);
//void disableWhiteChannel(void);

VCNL3040Error_e VCNL3040_enableProxLogicMode(VCNL3040 *dev);
VCNL3040Error_e VCNL3040_disableProxLogicMode(VCNL3040 *dev);

VCNL3040Error_e VCNL3040_setLEDCurrent(VCNL3040 *dev, uint8_t currentValue);

VCNL3040Error_e VCNL3040_setProxCancellation(VCNL3040 *dev, uint16_t cancelValue);
//void setALSHighThreshold(uint16_t threshold);
//void setALSLowThreshold(uint16_t threshold);
VCNL3040Error_e VCNL3040_setProxHighThreshold(VCNL3040 *dev, uint16_t threshold);
VCNL3040Error_e VCNL3040_setProxLowThreshold(VCNL3040 *dev, uint16_t threshold);

VCNL3040Error_e VCNL3040_getProximity(VCNL3040 *dev, uint16_t *prox);
//uint16_t getAmbient();
//uint16_t getWhite();
VCNL3040Error_e VCNL3040_getID(VCNL3040 *dev, uint16_t *id);

VCNL3040Error_e VCNL3040_setProxInterruptType(VCNL3040 *dev, uint8_t interruptValue); //Enable four prox interrupt types
//VCNL3040Error_e isClose(VCNL3040 *dev); //Interrupt flag: True if prox value greater than high threshold
//VCNL3040Error_e isAway(VCNL3040 *dev); //Interrupt flag: True if prox value lower than low threshold
//boolean isLight(); //Interrupt flag: True if ALS value higher than high threshold
//boolean isDark(); //Interrupt flag: True if ALS value lower than low threshold

VCNL3040Error_e VCNL3040_readCommand(VCNL3040 *dev, uint8_t commandCode, uint16_t *readData);
VCNL3040Error_e VCNL3040_readCommandLower(VCNL3040 *dev, uint8_t commandCode, uint8_t *commandValue);
VCNL3040Error_e VCNL3040_readCommandUpper(VCNL3040 *dev, uint8_t commandCode, uint8_t *commandValue);

VCNL3040Error_e VCNL3040_writeCommand(VCNL3040 *dev, uint8_t commandCode, uint16_t value);
VCNL3040Error_e VCNL3040_writeCommandLower(VCNL3040 *dev, uint8_t commandCode, uint8_t newValue);
VCNL3040Error_e VCNL3040_writeCommandUpper(VCNL3040 *dev, uint8_t commandCode, uint8_t newValue);

VCNL3040Error_e VCNL3040_bitMask(VCNL3040 *dev, uint8_t commandAddress, unsigned int commandHeight, uint8_t mask, uint8_t thing);

#endif
