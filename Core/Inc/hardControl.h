/*
 * hardControl.h
 *
 *  Created on: May 28, 2020
 *      Author: bastien
 */

#ifndef INC_HARDCONTROL_H_
#define INC_HARDCONTROL_H_

#include "motor.h"


__attribute__((always_inline))
static inline void v3SetPos(vector3d *v3, float x, float y, float z)
{
	v3->x = x;
	v3->y = y;
	v3->z = z;
}



typedef enum coordMode coordMode;
enum coordMode{
	relative = 0,
	absolute = 1,
};

typedef struct hardControl hardControl;
struct hardControl{
	float highSpeed; // mm/s
	coordMode coordMode;
	volatile PAPController motorPap[4];
	float zHeadOffset;
	vector3d scanPos;
	vector3d boardPos;
};

void hcInit(volatile hardControl *hc);
void hcSetZHeadOffset(volatile hardControl *hc, float off);
float hcGetZHeadOffset(volatile hardControl *hc);
void hcFlashSave(volatile hardControl *hc);
void setDigitalOutput(unsigned int digPin);
void resetDigitalOutput(unsigned int digPin);
void hcSetBoardPos(volatile hardControl *hc, float *pos);
void hcGetBoardPos(volatile hardControl *hc, float *pos);
void hcSetScanPos(volatile hardControl *hc, float *pos);
void hcGetScanPos(volatile hardControl *hc, float *pos);
void hcSetLinearSpeed(volatile hardControl *hc, float speed);
void hcSetRotationSpeed(volatile hardControl *hc, float speed);
unsigned char hcIsMoving(volatile hardControl *hc);
void hcGetPos(volatile hardControl *hc, float *pos);
void hcSetAccel(volatile hardControl *hc, float *accelTab);
void hcGetAccel(volatile hardControl *hc, float *accelTab);
void hcSetSpeed(volatile hardControl *hc, float *speedTab);
void hcGetSpeed(volatile hardControl *hc, float *speedTab);
void hcSetStepmm(volatile hardControl *hc, float *stepTab);
void hcGetStepmm(volatile hardControl *hc, float *stepTab);
void hcHome(volatile hardControl *hc, unsigned char *homeEna);
void hcMove(volatile hardControl *hc,float *moveTab, unsigned int axisEnable);
void hcStop(volatile hardControl *hc, unsigned int motorMask);


__attribute__((always_inline))
static inline void hcSetCoordMode(volatile hardControl *hc, coordMode cm)
{
	hc->coordMode = cm;
}


/*
__attribute__((always_inline))
static inline void  hcStepManage(volatile hardControl *hc)
{
	for(unsigned int i = 0 ; i< 4 ; i++)
		PAPstepManage(&(hc->motorPap[i]));
}
*/
__attribute__((always_inline))
static inline void  hcAccelControl(volatile hardControl *hc)
{
	for(unsigned int i = 0 ; i< 4 ; i++)
	{
		if(PAPisMoving(&(hc->motorPap[i])))
			PAPaccelControl(&(hc->motorPap[i]));
	}
}

void setDigitalOutput(unsigned int digPin);

#endif /* INC_HARDCONTROL_H_ */
