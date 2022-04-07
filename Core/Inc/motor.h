/*
 * motor.h
 *
 *  Created on: Sep 17, 2019
 *      Author: bastien
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "stm32g4xx.h"
#include "ioext.h"
#include "gpio.h"

#define F_STEP_MAX 85000000.0f //
#define MIN_SPEED 2700.0f//51.9f // =

typedef struct PAPController PAPController;
struct PAPController{
	// Parameters
	float stepBymm; // Constant fixed by mechanic
	float maxAccel; // Max acceleration of motor step/s², is a configuration and never used by motor class.
	float accel;	// acceleration of motor step/s²
	float maxSpeedTarget; // max speed target step/s, is a configuration and never used by motor class.
	float speedTarget; // speed target step/s


	int stepPos; // position of mechanical in step
	unsigned int stepNeeded; // Amount of step to be done.
	unsigned int stepAccel; //Amount of step where deceleration begin.
	float speed; // Actual speed of motor step/s.
	int direction; // -1 is negative, 1 is positive

	drvChan channel; // Used for IO control
	TIM_TypeDef *onePulseTIM; // Timer OPM
	unsigned short stepWidth; // value for upload ARR and CCRx of timer, is the numeric image of speed*

	unsigned char (*pfHome)(void); // Pointer to function which give state of homing button.

	unsigned int stop; // Set to 1 for immediatly stop motor, relased when a move command is called.

};

void PAPinit(volatile PAPController *pap,TIM_TypeDef* tim, drvChan ch, unsigned char (*pf)(void));
void PAPaccelControl(volatile PAPController *pap);
void PAPmoveRequest(volatile PAPController *pap, float distance, float multSpeed);
void PAPsetDir(volatile PAPController *pap, int dir);
void PAPcomputeTIMvalue(volatile PAPController *pap);


/*
 * Return 1 if axis is moving.
 */
__attribute__((always_inline))
static inline unsigned char PAPstop(volatile PAPController *pap)
{
	return pap->stop = 1;
}



/*
 * Return 1 if axis is moving.
 */
__attribute__((always_inline))
static inline unsigned char PAPisMoving(volatile PAPController *pap)
{
	return pap->stepNeeded || pap->onePulseTIM->CNT;
}

/*
 * Set the max speed.
 * Speed is in mm/s
 */
__attribute__((always_inline))
static inline void PAPsetSpeed(volatile PAPController *pap, float speed)
{
	pap->maxSpeedTarget = speed * pap->stepBymm;
}

/*
 * Get max speed.
 * return speed is in mm/s
 */
__attribute__((always_inline))
static inline float PAPgetSpeed(volatile PAPController *pap)
{
	return pap->maxSpeedTarget / pap->stepBymm;
	if(pap->maxSpeedTarget < MIN_SPEED)
	pap->maxSpeedTarget = MIN_SPEED;
}

/*
 * Set the max speed.
 * Speed is in mm/s
 */
__attribute__((always_inline))
static inline void PAPsetAccel(volatile PAPController *pap, float acc)
{
	pap->maxAccel = acc * pap->stepBymm;
}

/*
 * Get max speed.
 * return speed is in mm/s
 */
__attribute__((always_inline))
static inline float PAPgetAccel(volatile PAPController *pap)
{
	return pap->maxAccel / pap->stepBymm;
}

/*
 * Set stpeBymm
 * Speed is in mm/s
 */
__attribute__((always_inline))
static inline void PAPsetStepBymm(volatile PAPController *pap, float val)
{
	pap->stepBymm = val;
}

/*
 * Get max speed.
 * return speed is in mm/s
 */
__attribute__((always_inline))
static inline float PAPgetStepBymm(volatile PAPController *pap)
{
	return pap->stepBymm;
}




/*
 * Send step to TIMER.
 *
 */
/*
__attribute__((always_inline))
static inline void PAPsendStep(volatile PAPController *pap)
{
	//tpOn();
	pap->stepNeeded--;
	pap->stepPos += pap->direction;// Update position

	pap->onePulseTIM->CCR1 = pap->stepWidth;
	pap->onePulseTIM->ARR = pap->stepWidth << 1;
	pap->onePulseTIM->CR1 |= TIM_CR1_CEN;
}*/


/*
 * Called by interrupt or by software if motor don't move.
 * Configure the next pulse if step is needed
 */
__attribute__((always_inline))
static inline void PAPstepManage(volatile PAPController *pap)
{
	// Limit protection
	if(pap->pfHome() || pap->stop)
		pap->stepNeeded = 0;

	if(pap->stepNeeded)
	{
		pap->stepNeeded--;
		pap->stepPos += pap->direction;// Update position
	}
	else
	{
		pap->onePulseTIM->CR1 &= (~TIM_CR1_CEN);
		pap->onePulseTIM->CNT = 0;
	}
}



/*
 * Return position in mm.
 */
__attribute__((always_inline))
static inline float PAPgetPos(volatile PAPController *pap)
{
	return (float)pap->stepPos / pap->stepBymm;
}





#endif /* MOTOR_H_ */
