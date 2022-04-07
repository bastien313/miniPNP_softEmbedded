/*
 * motor.c
 *
 *  Created on: Sep 17, 2019
 *      Author: bastien
 */

#include "motor.h"
#include "gpio.h"
#include "tim.h"
#include "ioext.h"
#include "math.h"

// 170khz = (170Mhz/500) /2 (see TIM PSC).


/*
 * Update stepWidth with the new speed requested.
 */
void PAPcomputeTIMvalue(volatile PAPController *pap)
{
	pap->stepWidth = (unsigned int)(F_STEP_MAX / pap->speed);
	pap->onePulseTIM->CCR1 = pap->stepWidth;
	pap->onePulseTIM->ARR = pap->stepWidth << 1;
}


void PAPinit(volatile PAPController *pap,TIM_TypeDef* tim, drvChan ch, unsigned char (*pf)(void))
{
	pap->stepPos = 0;
	pap->stepNeeded = 0;
	pap->speed = MIN_SPEED;
	pap->direction = 1;

	pap->channel = ch;
	pap->onePulseTIM = tim;

	pap->pfHome = pf;

	setStepDiv(ch, DIV16);

	setDecayMode(ch,fast);

	driverResetOff(ch);
	driverDisable(ch);
	driverDir(pap->channel,pap->direction);
}




/*
 * Call this all ms for manage acceleration control.
 */
void PAPaccelControl(volatile PAPController *pap)
{

	if(pap->stepNeeded < pap->stepAccel)
	{
		//Deceleration
		if(pap->speed > MIN_SPEED)
		{
			pap->speed -= (pap->accel/1000.0);

			if(pap->speed < MIN_SPEED)
				pap->speed = MIN_SPEED;
			PAPcomputeTIMvalue(pap);
		}
	}
	else
	{
		//Acceleration
		if(pap->speed < pap->speedTarget)
		{
			pap->speed += (pap->accel/1000.0);

			if(pap->speed > pap->speedTarget)
				pap->speed = pap->speedTarget;
			PAPcomputeTIMvalue(pap);
		}
	}

}


/*
 * Update stepAccel param.
 */
void PAPaccelRampCalc(volatile PAPController *pap)
{
	float tRamp = (pap->speedTarget - MIN_SPEED)/pap->accel;
	pap->stepAccel = (MIN_SPEED * tRamp) + (0.5f*pap->accel*tRamp*tRamp);
}


/*
 * Set direction of PAP driver.
 * dir must be -1 or 1.
 */
void PAPsetDir(volatile PAPController *pap, int dir)
{
	pap->direction = dir;
	driverDir(pap->channel,pap->direction);
}

/*
 * Function start a move of axis.
 * Function block the program while precedent move is ongoing.
 * stepBymm is the distance in mm. (must be negative)
 */
void PAPmoveRequest(volatile PAPController *pap, float distance, float multSpeed)
{
	pap->speedTarget = multSpeed * pap->maxSpeedTarget;
	pap->accel = multSpeed * pap->maxAccel;
	pap->stop = 0;

	while(PAPisMoving(pap));

	if(distance >= 0.0f)
		PAPsetDir(pap,1);
	else
		PAPsetDir(pap,-1);

	pap->stepNeeded = (unsigned int)(pap->stepBymm * fabsf(distance));
	PAPaccelRampCalc(pap);
	if(pap->stepAccel > (pap->stepNeeded/2))
		pap->stepAccel = pap->stepNeeded/2;

	pap->speed = MIN_SPEED;
	PAPcomputeTIMvalue(pap);
	pap->onePulseTIM->CNT = 0;
	pap->onePulseTIM->EGR |= TIM_EGR_UG; // force Update event and take account of PSC.
	pap->onePulseTIM->CR1 |= TIM_CR1_CEN;
	//PAPstepManage(pap); // launch step burst.
}




