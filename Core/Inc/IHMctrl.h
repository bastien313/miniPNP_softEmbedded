/*
 * IHMctrl.h
 *
 *  Created on: May 28, 2020
 *      Author: bastien
 */

#ifndef INC_IHMCTRL_H_
#define INC_IHMCTRL_H_

typedef enum machineState machineState;
enum machineState{
	waiting, moveRequest, moving, homing
};

typedef enum homingState homingState;
enum homingState{
	home_start, home_waitMoving, home_switchEscape, home_delay, home_speedReconfigure, home_end
};


#define SIZE_SCAN_DATA 1000
typedef struct scanRequestParam scanRequestParam;
struct scanRequestParam{
	char axis;
	unsigned int idAxis;
	float stepMesurment;
	unsigned int nbMesure;
	float lengt;
	float speed;
	float lastScanPos;
	unsigned int isEnable;
	unsigned short scanVal[1000];
	float axisScanVal[1000];
};


void IHMctrlInit(void);
void externalCom(void);
void appLoop(void);
#endif /* INC_IHMCTRL_H_ */
