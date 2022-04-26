/*
 * hardControl.c
 *
 *  Created on: May 28, 2020
 *      Author: bastien
 */

#include "math.h"
#include "hardControl.h"
#include "gpio.h"
#include "tim.h"
#include "option.h"

void hcInit(volatile hardControl *hc)
{
	hc->coordMode = absolute;
	PAPinit(&(hc->motorPap[X]), TIM2, X, homeXisOn,MIN_SPEED);
	PAPinit(&(hc->motorPap[Y]), TIM3, Y, homeYisOn,MIN_SPEED);
	PAPinit(&(hc->motorPap[Z]), TIM4, Z, homeZisOn,MIN_SPEED);
	PAPinit(&(hc->motorPap[T]), TIM5, T, homeTisOn,170.0f);
	optReadFlash(hc);
}





void hcSetZHeadOffset(volatile hardControl *hc, float off)
{
	hc->zHeadOffset = off;
}
float hcGetZHeadOffset(volatile hardControl *hc)
{
	return hc->zHeadOffset;
}

/*
 * Save parameter in flash/
 */
void hcFlashSave(volatile hardControl *hc)
{
	optWriteFlash(hc);
}


void setDigitalOutput(unsigned int digPin)
{
	switch(digPin)
	{
		case 0:
			pumpOn();
		break;

		case 1:
			EVenable();
		break;
		case 2:
			EXTenable();
		break;

		default:
		break;
	}
}


void resetDigitalOutput(unsigned int digPin)
{
	switch(digPin)
	{
		case 0:
			pumpOff();
		break;

		case 1:
			EVdisable();
		break;
		case 2:
			EXTdisable();
		break;

		default:
		break;
	}
}

void hcSetBoardPos(volatile hardControl *hc, float *pos)
{
	v3SetPos(&(hc->boardPos),pos[X], pos[Y], pos[Z]);
}

void hcGetBoardPos(volatile hardControl *hc, float *pos)
{
	pos[X] = hc->boardPos.x;
	pos[Y] = hc->boardPos.y;
	pos[Z] = hc->boardPos.z;
}

void hcSetScanPos(volatile hardControl *hc, float *pos)
{
	v3SetPos(&(hc->scanPos),pos[X], pos[Y], pos[Z]);
}

void hcGetScanPos(volatile hardControl *hc, float *pos)
{
	pos[X] = hc->scanPos.x;
	pos[Y] = hc->scanPos.y;
	pos[Z] = hc->scanPos.z;
}

void hcSetLinearSpeed(volatile hardControl *hc, float speed)
{
	PAPsetSpeed(&(hc->motorPap[X]),speed);
	PAPsetSpeed(&(hc->motorPap[Y]),speed);
	PAPsetSpeed(&(hc->motorPap[Z]),speed);
}

void hcSetRotationSpeed(volatile hardControl *hc, float speed)
{
	PAPsetSpeed(&(hc->motorPap[T]),speed);
}


/*
 * Return 1 if any motor is moving, optherwise return 0.
 */
unsigned char hcIsMoving(volatile hardControl *hc)
{

	for(unsigned int i = 0; i < 4 ; i++)
	{
		if(PAPisMoving(&(hc->motorPap[i])))
			return 1;
	}
	return 0;
}

/*
 * Write in pos the position of all axis.
 * pos must be allocated with size = 4
 */
void hcGetPos(volatile hardControl *hc, float *pos)
{
	for(unsigned int i = 0; i < 4 ; i++)
	{
		pos[i] = PAPgetPos(&(hc->motorPap[i]));
	}
}


/*
 * Set acceleration of channel if valui is not zero.
 */
void hcSetAccel(volatile hardControl *hc, float *accelTab)
{
	for(unsigned int i = 0 ; i < 4; i++)
	{
		if(fpclassify(accelTab[i]) != FP_ZERO)
			PAPsetAccel(&(hc->motorPap[i]),accelTab[i]);
	}
}


/*
 * Set acceleration of channel if valui is not zero.
 */
void hcGetAccel(volatile hardControl *hc, float *accelTab)
{
	for(unsigned int i = 0 ; i < 4; i++)
	{
		accelTab[i] = PAPgetAccel(&(hc->motorPap[i]));

	}
}
/*
 * Set speed of channel if valui is not zero.
 */
void hcSetSpeed(volatile hardControl *hc, float *speedTab)
{
	for(unsigned int i = 0 ; i < 4; i++)
	{
		if(fpclassify(speedTab[i]) != FP_ZERO)
			PAPsetSpeed(&(hc->motorPap[i]),speedTab[i]);
	}
}

void hcGetSpeed(volatile hardControl *hc, float *speedTab)
{
	for(unsigned int i = 0 ; i < 4; i++)
	{
		speedTab[i] = PAPgetSpeed(&(hc->motorPap[i]));
	}
}


/*
 * Set stepConf.
 */
void hcSetStepmm(volatile hardControl *hc, float *stepTab)
{
	for(unsigned int i = 0 ; i < 4; i++)
	{
		if(fpclassify(stepTab[i]) != FP_ZERO)
			PAPsetStepBymm(&(hc->motorPap[i]),stepTab[i]);
	}
}

void hcGetStepmm(volatile hardControl *hc, float *stepTab)
{
	for(unsigned int i = 0 ; i < 4; i++)
	{
		stepTab[i] = PAPgetStepBymm(&(hc->motorPap[i]));
	}
}

/*
 * Make homing.
 * homeEna is a unsigned char[4].
 */
/*
void hcHome(volatile hardControl *hc, unsigned char *homeEna)
{
	float oldSpeed[4];

	// Move to switch
	for(unsigned int i = 0 ; i < 2; i++)
	{
		if(homeEna[i])
		{
			oldSpeed[i] = PAPgetSpeed(&(hc->motorPap[i]));
			PAPsetSpeed(&(hc->motorPap[i]),10.0f);
			PAPmoveRequest(&(hc->motorPap[i]),-200000,1.0f);
		}
	}

	if(homeEna[Z])
	{
		oldSpeed[Z] = PAPgetSpeed(&(hc->motorPap[Z]));
		PAPsetSpeed(&(hc->motorPap[Z]),10.0f);
		PAPmoveRequest(&(hc->motorPap[Z]),200000,1.0f);
	}

	while(hcIsMoving(hc));

	// Prepare switch escape.
	for(unsigned int i = 0 ; i < 2; i++)
		PAPsetDir(&(hc->motorPap[i]),1);

	PAPsetDir(&(hc->motorPap[Z]),-1);

	// Switch escape
	while(homeEna[X] || homeEna[Y] || homeEna[Z])
	{
		for(unsigned int i = 0 ; i < 3; i++)
		{
			if(homeEna[i])
			{
				if(!hc->motorPap[i].pfHome())
				{
					homeEna[i] = 0;
					hc->motorPap[i].stepPos = 0;
				}
				else
				{
					if(i == Z)
						PAPmoveRequest(&(hc->motorPap[i]),-1,0.2f);
					else
						PAPmoveRequest(&(hc->motorPap[i]),1,0.2f);
				}
			}
		}

		delay_us(1000);
	}

	for(unsigned int i = 0 ; i < 3; i++)
	{
		PAPsetSpeed(&(hc->motorPap[i]),oldSpeed[i]);
	}
}
*/
/*
 * Launch move to each motor.
 * moveTab must be a float[4] wich contain all movement.
 */
void hcMove(volatile hardControl *hc,float *moveTab, unsigned int axisEnable)
{
	float dist[4];
	float speedCoef[4];

	// Compute absolute distance to move
	if(hc->coordMode == absolute)
	{
		for(unsigned int i = 0 ; i < 4; i++)
		{
			if(axisEnable & (0x01 << i))
				dist[i] = fabsf(PAPgetPos(&(hc->motorPap[i])) - moveTab[i]);
			else
				dist[i] = 0.0f;
		}
	}
	else
	{
		for(unsigned int i = 0 ; i < 4; i++)
		{
			if (axisEnable & (0x01 << i))
				dist[i] = fabsf(moveTab[i]);
			else
				dist[i] = 0.0f;
		}

	}

	//X is the biggest distance.
	if( (dist[X] >= dist[Y]) && (dist[X] >= dist[Z]) && (fpclassify(dist[X]) != FP_ZERO) )
	{
		speedCoef[X] = 1.0f;
		speedCoef[Y] = dist[Y] / dist[X];
		speedCoef[Z] = dist[Z] / dist[X];
		speedCoef[T] = 1.0f;
	}
	//Y is the biggest distance.
	else if( (dist[Y] >= dist[X]) && (dist[Y] >= dist[Z]) && (fpclassify(dist[Y]) != FP_ZERO) )
	{
		speedCoef[X] = dist[X] / dist[Y];
		speedCoef[Y] = 1.0f;
		speedCoef[Z] = dist[Z] / dist[Y];
		speedCoef[T] = 1.0f;
	}
	//Z is the biggest distance.
	else if (dist[Z] != FP_ZERO)
	{
		speedCoef[X] = dist[X] / dist[Z];
		speedCoef[Y] = dist[Y] / dist[Z];
		speedCoef[Z] = 1.0f;
		speedCoef[T] = 1.0f;
	}
	else
	{
		speedCoef[X] = 1.0f;
		speedCoef[Y] = 1.0f;
		speedCoef[Z] = 1.0f;
		speedCoef[T] = 1.0f;
	}

	// Compute distance to move
	if(hc->coordMode == absolute)
	{
		for(unsigned int i = 0 ; i < 4; i++)
		{
			if (axisEnable & (0x01 << i))
				dist[i] = moveTab[i] - PAPgetPos(&(hc->motorPap[i]));
			else
				dist[i] = 0.0f;
		}

	}
	else
	{
		for(unsigned int i = 0 ; i < 4; i++)
		{
			if (axisEnable & (0x01 << i))
				dist[i] = moveTab[i];
			else
				dist[i] = 0.0f;
		}
	}

	for(unsigned int i = 0 ; i < 4; i++)
	{
		if( (fpclassify(dist[i]) != FP_ZERO) && (!PAPisMoving(&hc->motorPap[i])) )
			PAPmoveRequest(&(hc->motorPap[i]), dist[i], speedCoef[i]);
	}
}

void hcStop(volatile hardControl *hc, unsigned int motorMask)
{
	for(unsigned int i = 0 ; i < 4; i++)
	{
		if(motorMask & (0x01<<i))
			PAPstop(&hc->motorPap[i]);
	}
}


