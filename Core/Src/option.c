/*
 * option.c
 *
 *  Created on: Oct 31, 2019
 *      Author: bastien
 */

#include "stm32g4xx.h"
#include "option.h"
#include "tim.h"
#include <string.h>


unsigned int *adrUserPage 	  = (unsigned int*) (FLASH_BASE + (USER_PAGE * FLASH_PAGE_SIZE));

/*
 * Write data pointed by pData(32-bit width).
 * page: page of flash 0 to 127 for 256k of flash.
 * size : 1 to 512.
 */
void flashWriteQ31(unsigned int page, unsigned int *pData, unsigned int size)
{
	FLASH_EraseInitTypeDef ERInit;
	uint32_t add = FLASH_BASE + (page * FLASH_PAGE_SIZE);
	uint32_t err;

	ERInit.TypeErase = FLASH_TYPEERASE_PAGES;
	ERInit.Page = page ;
	ERInit.Banks = FLASH_BANK_1;
	ERInit.NbPages = 1;

	HAL_FLASH_Unlock();

	HAL_FLASHEx_Erase(&ERInit,&err);

	//uint64_t *valWrite = (uint64_t*) pData;
	for(unsigned short i = 0; i<size; i+=2)
	{

		uint64_t valWrite = (uint64_t) pData[i] + (((uint64_t)pData[i+1])<<32);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,add,valWrite);
		add +=8;
	}

	HAL_FLASH_Lock();
}

/*
 * Put unsigned int into float  without casting.
 */
__attribute__((always_inline))
static inline float uIntToFloatNC(unsigned int val)
{
	return *((float*)&val);
}

/*
 * Put unsigned int into char without casting.
 */
__attribute__((always_inline))
static inline char uIntToCharNC(unsigned int val)
{
	return *((char*)&val);
}


/*
 * Put float into unsigned int without casting.
 */
__attribute__((always_inline))
static inline unsigned int floatToUintNC(float val)
{
	return *((unsigned int*)&val);
}

/*
 * Put char into unsigned int without casting.
 */
__attribute__((always_inline))
static inline unsigned int charToUintNC(char val)
{
	return *((unsigned int*)&val);
}




/*
 * Compute chkSum of buffer.
 */
unsigned int chkSumCalc(unsigned int *buff,unsigned int sizeBuff)
{
	unsigned int chkSum = 0;
	for(unsigned int i = 0; i < sizeBuff; i++)
	{
		chkSum += buff[i];
	}
	return chkSum;
}



/*
 * Write the content of opt in flash.
 */
void  optWriteFlash(volatile hardControl *hc)
{
	unsigned int tabOut[TOTAL_SIZE + 1];

	tabOut[MOTOR_X_POS + MOTOR_SPEED_OFF] = floatToUintNC(hc->motorPap[X].maxSpeedTarget);
	tabOut[MOTOR_Y_POS + MOTOR_SPEED_OFF] = floatToUintNC(hc->motorPap[Y].maxSpeedTarget);
	tabOut[MOTOR_Z_POS + MOTOR_SPEED_OFF] = floatToUintNC(hc->motorPap[Z].maxSpeedTarget);
	tabOut[MOTOR_T_POS + MOTOR_SPEED_OFF] = floatToUintNC(hc->motorPap[T].maxSpeedTarget);

	tabOut[MOTOR_X_POS + MOTOR_ACCEL_OFF] = floatToUintNC(hc->motorPap[X].maxAccel);
	tabOut[MOTOR_Y_POS + MOTOR_ACCEL_OFF] = floatToUintNC(hc->motorPap[Y].maxAccel);
	tabOut[MOTOR_Z_POS + MOTOR_ACCEL_OFF] = floatToUintNC(hc->motorPap[Z].maxAccel);
	tabOut[MOTOR_T_POS + MOTOR_ACCEL_OFF] = floatToUintNC(hc->motorPap[T].maxAccel);

	tabOut[MOTOR_X_POS + MOTOR_STEPMM_OFF] = floatToUintNC(hc->motorPap[X].stepBymm);
	tabOut[MOTOR_Y_POS + MOTOR_STEPMM_OFF] = floatToUintNC(hc->motorPap[Y].stepBymm);
	tabOut[MOTOR_Z_POS + MOTOR_STEPMM_OFF] = floatToUintNC(hc->motorPap[Z].stepBymm);
	tabOut[MOTOR_T_POS + MOTOR_STEPMM_OFF] = floatToUintNC(hc->motorPap[T].stepBymm);

	tabOut[SCAN_POS + X_OFF] = floatToUintNC(hc->scanPos.x);
	tabOut[SCAN_POS + Y_OFF] = floatToUintNC(hc->scanPos.y);
	tabOut[SCAN_POS + Z_OFF] = floatToUintNC(hc->scanPos.z);

	tabOut[BOARD_POS + X_OFF] = floatToUintNC(hc->boardPos.x);
	tabOut[BOARD_POS + Y_OFF] = floatToUintNC(hc->boardPos.y);
	tabOut[BOARD_POS + Z_OFF] = floatToUintNC(hc->boardPos.z);

	tabOut[HEAD_POS + X_OFF] = 0;
	tabOut[HEAD_POS + Y_OFF] = 0;
	tabOut[HEAD_POS + Z_OFF] = floatToUintNC(hc->zHeadOffset);

	tabOut[TOTAL_SIZE] = chkSumCalc(tabOut,TOTAL_SIZE);


	if(memcmp(tabOut,adrUserPage,TOTAL_SIZE+1)) // Write if we have change for save flash.
	{
		ledOn();
		delay_ms(100);
		ledOff();
		flashWriteQ31(USER_PAGE,tabOut,TOTAL_SIZE+1);
	}

}



/*
 * Function called when data is invalid.
 * Write default option on flash.
 */
void optWriteDefalut(volatile hardControl *hc)
{
	hc->motorPap[X].maxSpeedTarget = 160000.0f;
	hc->motorPap[Y].maxSpeedTarget = 160000.0f;
	hc->motorPap[Z].maxSpeedTarget = 160000.0f;
	hc->motorPap[T].maxSpeedTarget = 160000.0f;

	hc->motorPap[X].maxAccel = 30000.0f;
	hc->motorPap[Y].maxAccel = 30000.0f;
	hc->motorPap[Z].maxAccel = 30000.0f;
	hc->motorPap[T].maxAccel = 30000.0f;

	hc->motorPap[X].stepBymm = 3200.0f;
	hc->motorPap[Y].stepBymm = 3200.0f;
	hc->motorPap[Z].stepBymm = 3200.0f;
	hc->motorPap[T].stepBymm = 3200.0f;

	hc->scanPos.x = 10.2f;
	hc->scanPos.y = 5.2f;
	hc->scanPos.z = 18.2f;

	hc->boardPos.x = 22.2f;
	hc->boardPos.y = 15.2f;
	hc->boardPos.z = 25.2f;

	hc->zHeadOffset = 7.4f;

	optWriteFlash(hc);
}


/*
 * Read data in flash and put this in option.
 * If chkSum is not valid, we write default data in flash and into option.
 */
unsigned char optReadFlash(volatile hardControl *hc)
{
	if(chkSumCalc(adrUserPage,TOTAL_SIZE) == adrUserPage[TOTAL_SIZE])
	{
		hc->motorPap[X].maxSpeedTarget = uIntToFloatNC(adrUserPage[MOTOR_X_POS + MOTOR_SPEED_OFF]);
		hc->motorPap[Y].maxSpeedTarget = uIntToFloatNC(adrUserPage[MOTOR_Y_POS + MOTOR_SPEED_OFF]);
		hc->motorPap[Z].maxSpeedTarget = uIntToFloatNC(adrUserPage[MOTOR_Z_POS + MOTOR_SPEED_OFF]);
		hc->motorPap[T].maxSpeedTarget = uIntToFloatNC(adrUserPage[MOTOR_T_POS + MOTOR_SPEED_OFF]);

		hc->motorPap[X].maxAccel = uIntToFloatNC(adrUserPage[MOTOR_X_POS + MOTOR_ACCEL_OFF]);
		hc->motorPap[Y].maxAccel = uIntToFloatNC(adrUserPage[MOTOR_Y_POS + MOTOR_ACCEL_OFF]);
		hc->motorPap[Z].maxAccel = uIntToFloatNC(adrUserPage[MOTOR_Z_POS + MOTOR_ACCEL_OFF]);
		hc->motorPap[T].maxAccel = uIntToFloatNC(adrUserPage[MOTOR_T_POS + MOTOR_ACCEL_OFF]);

		hc->motorPap[X].stepBymm = uIntToFloatNC(adrUserPage[MOTOR_X_POS + MOTOR_STEPMM_OFF]);
		hc->motorPap[Y].stepBymm = uIntToFloatNC(adrUserPage[MOTOR_Y_POS + MOTOR_STEPMM_OFF]);
		hc->motorPap[Z].stepBymm = uIntToFloatNC(adrUserPage[MOTOR_Z_POS + MOTOR_STEPMM_OFF]);
		hc->motorPap[T].stepBymm = uIntToFloatNC(adrUserPage[MOTOR_T_POS + MOTOR_STEPMM_OFF]);

		hc->scanPos.x = uIntToFloatNC(adrUserPage[SCAN_POS + X_OFF]);
		hc->scanPos.y = uIntToFloatNC(adrUserPage[SCAN_POS + Y_OFF]);
		hc->scanPos.z = uIntToFloatNC(adrUserPage[SCAN_POS + Z_OFF]);

		hc->boardPos.x = uIntToFloatNC(adrUserPage[BOARD_POS + X_OFF]);
		hc->boardPos.y = uIntToFloatNC(adrUserPage[BOARD_POS + Y_OFF]);
		hc->boardPos.z = uIntToFloatNC(adrUserPage[BOARD_POS + Z_OFF]);

		hc->zHeadOffset = uIntToFloatNC(adrUserPage[HEAD_POS + Z_OFF]);

		return 1;

	}
	else
	{
		optWriteDefalut(hc);
		return 0;
	}
}








