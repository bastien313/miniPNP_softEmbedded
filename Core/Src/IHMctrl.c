/*
 * IHMctrl.c
 *
 *  Created on: May 28, 2020
 *      Author: bastien
 */

#include <string.h>
#include <stdio.h>
#include "com.h"
#include "hardControl.h"
#include "feeder.h"
#include "IHMctrl.h"
#include "VCNL3040.h"
#include "ABP_presure.h"



volatile receptionController rcPc;
char bufferRc[250];

volatile transmitterController tcPc;
char bufferTc[250];
volatile hardControl hc;

VCNL3040 scanSensor;
ABP_presure presureSensor;

machineState appState;
homingState appHomingState;
unsigned int watchdogMode; //If enable watchdog mode stop all movement if external com doesn't feed the watchdog.
unsigned int pipe1StatusMode; //If enable send status data over pipe 1


float moveRequestArr[4]; //Temporary move data request, passed by external com to rest of program.
unsigned int axisEenableRequest; //Temporary axis enable data, passed by external com to rest of program.
//unsigned int stopRequest;// Set this variable to 1 to stop all movement on going.

scanRequestParam scanRequest;


unsigned int watchDogTime;
unsigned int statusTime;
#define WATCHDOG_TIMEOUT 400 // 400ms
#define STATUS_SEND_DELAY 400 // 400ms
extern volatile unsigned int userTick;

/*
 * Check if axis is enable in axisEenableRequest global var.
 */
unsigned int axisIsEna(drvChan axis)
{
	return (axisEenableRequest & (0x01 << axis));
}
/*
void Scan(void)
{
	unsigned int amountscan = 0;
	unsigned char scanLIst[127];
	for(unsigned int i = 1; i<127; i++)
	{
		unsigned char val;
		scanLIst[i] = 0;
		if(I2CRead(I2C2, i << 1, REGISTER_ID, 1, &val))
		{
			scanLIst[amountscan] = i << 1;
			amountscan++;
		}
	}
	amountscan= 18;
}*/

void IHMctrlInit(void)
{
	rcInit(&rcPc, '\r');
	tcInit(&tcPc, USART1, bufferTc, 250); //(volatile transmitterController *tc, USART_TypeDef *us, char *buffAdr, unsigned int siz)
	tcDMAconfigure(&tcPc,DMA1, 0);
	hcInit(&hc);
	VCNL3040_Init(&scanSensor, I2C2);
	ABP_Init(&presureSensor, I2C2, 15.0, -15.0);

	appState = waiting;
	moveRequestArr[0] = 0.0f;
	moveRequestArr[1] = 0.0f;
	moveRequestArr[2] = 0.0f;
	moveRequestArr[3] = 0.0f;
	axisEenableRequest = 0;
	//stopRequest = 0;
	statusTime = userTick;
	watchDogTime = userTick;
	watchdogMode = 0;
	pipe1StatusMode = 1;

}
/*
void exctractParam(char *strIn, char delimiter, char *strOut)
{
	while(*strIn != delimiter && *strIn != 0)
	{
		*strOut = *strIn;
		strOut++;
		strIn++;
	}
}*/


/*
 * Get value of each axis and store it in val.
 */
unsigned int getFloatAxisVal(char *cmd, float *val)
{
	unsigned int axisEnable = 0; // set bit if axis is enable .
	char *ptrFind = strpbrk(cmd,"X");
	if(ptrFind)
	{
		sscanf(ptrFind+1,"%f",&val[X]);
		axisEnable |= 0x01;
	}


	ptrFind = strpbrk(cmd,"Y");
	if(ptrFind)
	{
		sscanf(ptrFind+1,"%f",&val[Y]);
		axisEnable |= 0x02;
	}

	ptrFind = strpbrk(cmd,"Z");
	if(ptrFind)
	{
		sscanf(ptrFind+1,"%f",&val[Z]);
		axisEnable |= 0x04;
	}

	ptrFind = strpbrk(cmd,"C");
	if(ptrFind)
	{
		sscanf(ptrFind+1,"%f",&val[T]);
		axisEnable |= 0x08;
	}
	return axisEnable;
}


/*
 * Send position of hardware to PC.
 */
void sendPos(void)
{
	float pos[4];
	char posStr[100];
	hcGetPos(&hc,pos);

	sprintf(posStr,"0X:%.4f Y:%.4f Z:%.4f C:%.4f\r",pos[X],pos[Y],pos[Z],pos[T]);
	tcDMASendStr(&tcPc,posStr);
}


void setZHeasRefCommad(char *cmd)
{
	float pos[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	getFloatAxisVal(cmd,pos);
	hcSetZHeadOffset(&hc,pos[Z]);
	tcDMASendStr(&tcPc,"0OK\r");
}

void setScanRefCommad(char *cmd)
{
	float pos[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	getFloatAxisVal(cmd,pos);
	hcSetScanPos(&hc,pos);
	tcDMASendStr(&tcPc,"0OK\r");
}

void setBoardRefCommad(char *cmd)
{
	float pos[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	getFloatAxisVal(cmd,pos);
	hcSetBoardPos(&hc,pos);
	tcDMASendStr(&tcPc,"0OK\r");
}

void setSpeedCommand(char *cmd)
{
	float speed[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	getFloatAxisVal(cmd,speed);
	hcSetSpeed(&hc, speed);
	tcDMASendStr(&tcPc,"0OK\r");
}

void setAccelCommand(char *cmd)
{
	float accel[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	getFloatAxisVal(cmd,accel);
	hcSetAccel(&hc, accel);
	tcDMASendStr(&tcPc,"0OK\r");
}
void setStepConfCommand(char *cmd)
{
	float stepC[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	getFloatAxisVal(cmd,stepC);
	hcSetStepmm(&hc, stepC);
	tcDMASendStr(&tcPc,"0OK\r");
}

void homeCommand(char *cmd)
{
	axisEenableRequest = 0;
	if(cmd[0] == 0)
	{
		axisEenableRequest = 0x0F;
	}
	else
	{
		if(strchr (cmd,'X'))
			axisEenableRequest |= 0x01;
		if(strchr (cmd,'Y'))
			axisEenableRequest |= 0x02;
		if(strchr (cmd,'Z'))
			axisEenableRequest |= 0x04;
		if(strchr (cmd,'C'))
			axisEenableRequest |= 0x08;
	}
	appHomingState = home_start;
	appState = homing;
	/*driverEnable(X);
	driverEnable(Y);
	driverEnable(Z);
	driverEnable(T);
	hcHome(&hc, homeEna);
	sendPos();
	driverDisable(X);
	driverDisable(Y);
	driverDisable(Z);
	driverDisable(T);*/
}

/*
 * Manage movement and  speed if requested.
 * If C move is present only circular speed is changed.
 */
void moveCommand(char *cmd)
{
	char *ptrFind = strpbrk(cmd,"F");
	if(ptrFind)
	{
		float speed;
		//exctractParam(ptrFind,' ',strParam);
		sscanf(ptrFind+1,"%f",&speed);
		if(strpbrk(cmd,"C"))
			hcSetRotationSpeed(&hc, speed);
		else
			hcSetLinearSpeed(&hc, speed);
	}

	axisEenableRequest = getFloatAxisVal(cmd,moveRequestArr);
	appState = moveRequest;
	/*driverEnable(X);
	driverEnable(Y);
	driverEnable(Z);
	driverEnable(T);

	// start movement.
	hcMove(&hc, moveTab);

	while(hcIsMoving(&hc));
	sendPos();
	driverDisable(X);
	driverDisable(Y);
	driverDisable(Z);
	driverDisable(T);*/
}


void readAccel(void)
{
	char strOut[100];
	float accel[4]= {0.0f, 0.0f, 0.0f, 0.0f};
	hcGetAccel(&hc, accel);
	sprintf(strOut, "0X%f Y%f Z%f C%f\r", accel[X], accel[Y], accel[Z], accel[T]);
	tcDMASendStr(&tcPc,strOut);
}

void readStepConf(void)
{
	char strOut[100];
	float stepC[4]= {0.0f, 0.0f, 0.0f, 0.0f};
	hcGetStepmm(&hc, stepC);
	sprintf(strOut, "0X%f Y%f Z%f C%f\r", stepC[X], stepC[Y], stepC[Z], stepC[T]);
	tcDMASendStr(&tcPc,strOut);
}

void readSpeed(void)
{
	char strOut[100];
	float speed[4]= {0.0f, 0.0f, 0.0f, 0.0f};
	hcGetSpeed(&hc, speed);
	sprintf(strOut, "0X%f Y%f Z%f C%f\r", speed[X], speed[Y], speed[Z], speed[T]);
	tcDMASendStr(&tcPc,strOut);
}

void readScanRef(void)
{
	char strOut[100];
	float sr[4]= {0.0f, 0.0f, 0.0f, 0.0f};
	hcGetScanPos(&hc, sr);
	sprintf(strOut, "0X%f Y%f Z%f\r", sr[X], sr[Y], sr[Z]);
	tcDMASendStr(&tcPc,strOut);
}

void readBoardRef(void)
{
	char strOut[100];
	float sr[4]= {0.0f, 0.0f, 0.0f, 0.0f};
	hcGetBoardPos(&hc, sr);
	sprintf(strOut, "0X%f Y%f Z%f\r", sr[X], sr[Y], sr[Z]);
	tcDMASendStr(&tcPc,strOut);
}

void readZHeadOff(void)
{
	char strOut[100];
	sprintf(strOut, "0Z%f\r", hcGetZHeadOffset(&hc));
	tcDMASendStr(&tcPc,strOut);
}

void sendFeederList(void)
{
	char strOut[256];
	feederScan();
	strOut[0] = '0';
	strOut[1] = 0;

	for(unsigned int i = 0; i<amountFeeder; i++)
	{
		char addrStr[20];
		sprintf(addrStr, "%d ", feederlist[i]);
		strcat(strOut,addrStr);
	}
	strOut[strlen(strOut)-1] = 0; // Escape last space
	strcat(strOut,"\r");
	tcDMASendStr(&tcPc,strOut);
}

void getFeederParam(char *cmd)
{
	char strOut[256];
	char name[40];
	unsigned int addr;
	vector3d pos;
	unsigned char stepS;
	unsigned char stepCMP;
	sscanf(cmd,"%u",&addr);

	feederGetName(addr, name);
	feederGetPos(addr, &pos);
	stepS = feederGetStepSize(addr);
	stepCMP = feederGetCMPB(addr);

	sprintf(strOut,"0N%s X%f Y%f Z%f S%d C%d\r", name, pos.x, pos.y, pos.z, stepS, stepCMP);
	tcDMASendStr(&tcPc,strOut);
}

void setFeederParam(char *cmd)
{
	char name[50];
	unsigned int addr;
	vector3d pos;
	unsigned int stepS;
	unsigned int stepCMP;
	sscanf(cmd,"%u N%s X%f Y%f Z%f S%u C%u", &addr, name, &(pos.x), &(pos.y), &(pos.z), &stepS, &stepCMP);

	feederSetName(addr, name);
	feederSetPos(addr, pos);
	feederSetStepSize(addr, stepS);
	feederSetCMPB(addr, stepCMP);

	tcDMASendStr(&tcPc,"0OK\r");
}

void flashSaveFeeder(char *cmd)
{
	unsigned int addr;
	sscanf(cmd,"%u",&addr);
	feederFlashSave(addr);
	tcDMASendStr(&tcPc,"0OK\r");
}

void feederMove(char *cmd)
{
	unsigned int addr;
	sscanf(cmd,"%u",&addr);
	feederSendStep(addr);
	tcDMASendStr(&tcPc,"0OK\r");
}

void readCommand(char *cmd)
{
	if( cmd[0] == '2' && cmd[1] == '0' && cmd[2] == '1')
	{
		readAccel();
	}
	else if( cmd[0] == '9' && cmd[1] == '2')
	{
		readStepConf();
	}
	else if( cmd[0] == '2' && cmd[1] == '0' && cmd[2] == '3')
	{
		readSpeed();
	}
	else if( cmd[0] == '3' && cmd[1] == '3' && cmd[2] == '0')
	{
		readScanRef();
	}
	else if( cmd[0] == '3' && cmd[1] == '3' && cmd[2] == '1')
	{
		readBoardRef();
	}
	else if( cmd[0] == '3' && cmd[1] == '3' && cmd[2] == '2')
	{
		readZHeadOff();
	}
	else if( cmd[0] == '1' && cmd[1] == '1' && cmd[2] == '4')
	{
		sendPos();
	}
	else if( cmd[0] == '4' && cmd[1] == '0' && cmd[2] == '0')
	{
		if(appState == waiting)// is busy?
			tcDMASendStr(&tcPc,"00\r");
		else
			tcDMASendStr(&tcPc,"01\r");
	}
	else if( cmd[0] == '5' && cmd[1] == '0' && cmd[2] == '0')
	{
		char distStr[20];
		uint16_t numericDistance = 0;
		VCNL3040_getProximity(&scanSensor,&numericDistance);
		sprintf(distStr,"0%d\r",numericDistance);
		tcDMASendStr(&tcPc,distStr);
	}
	else if( cmd[0] == '5' && cmd[1] == '0' && cmd[2] == '1')
	{
		char moveStr[40];
		float posTemp[4];
		sscanf(&cmd[4],"A%c N%u L%f F%f", &scanRequest.axis, &scanRequest.nbMesure, &scanRequest.lengt, &scanRequest.speed);
		scanRequest.stepMesurment = scanRequest.lengt / (float)scanRequest.nbMesure;
		scanRequest.nbMesure = 0;
		scanRequest.isEnable = 1;
		if(scanRequest.axis == 'X') 	  scanRequest.idAxis = 0;
		else if (scanRequest.axis == 'Y') scanRequest.idAxis = 1;
		else if (scanRequest.axis == 'Z') scanRequest.idAxis = 2;
		else if (scanRequest.axis == 'C') scanRequest.idAxis = 3;
		hcGetPos(&hc, posTemp);
		scanRequest.lastScanPos = posTemp[scanRequest.idAxis];
		hcSetCoordMode(&hc, relative);
		sprintf(moveStr, "F%f %c%f", scanRequest.speed, scanRequest.axis, scanRequest.lengt);
		moveCommand(moveStr);

	}
	else if( cmd[0] == '6' && cmd[1] == '0' && cmd[2] == '0')
	{
		char distStr[20];
		float presure = 0;
		ABP_readPresureFloat(&presureSensor, &presure);
		sprintf(distStr,"0%0.4f\r",presure);
		tcDMASendStr(&tcPc,distStr);
	}
	else
		tcDMASendStr(&tcPc,"0R Command unknown\r");
}

/*
 * If first char on command line is M.
 * Used for move configuration command.
 */
void confCommand(char *cmd)
{
	if( cmd[0] == '2' && cmd[1] == '0' && cmd[2] == '1' && cmd[3] == ' ')
	{
		setAccelCommand(&cmd[4]);
	}
	else if( cmd[0] == '2' && cmd[1] == '0' && cmd[2] == '3' && cmd[3] == ' ')
	{
		setSpeedCommand(&cmd[4]);
	}
	else if( cmd[0] == '3' && cmd[1] == '3' && cmd[2] == '0' && cmd[3] == ' ')
	{
		setScanRefCommad(&cmd[4]);
	}
	else if( cmd[0] == '3' && cmd[1] == '3' && cmd[2] == '1' && cmd[3] == ' ')
	{
		setBoardRefCommad(&cmd[4]);
	}
	else if( cmd[0] == '3' && cmd[1] == '3' && cmd[2] == '2' && cmd[3] == ' ')
	{
		setZHeasRefCommad(&cmd[4]);
	}
	else if( cmd[0] == '5' && cmd[1] == '0' && cmd[2] == '0')
	{
		hcFlashSave(&hc);
		tcDMASendStr(&tcPc,"0OK\r");
	}
	else if(cmd[0] == '6'&& cmd[1] == '4' && cmd[2] == ' ')
	{
		unsigned int digPin;
		sscanf(&cmd[3], "%u",&digPin);
		setDigitalOutput(digPin);
	}
	else if(cmd[0] == '6'&& cmd[1] == '5' && cmd[2] == ' ')
	{
		unsigned int digPin;
		sscanf(&cmd[3], "%u",&digPin);
		resetDigitalOutput(digPin);
	}
	else if(cmd[0] == '9'&& cmd[1] == '2' && cmd[2] == ' ')
	{
		setStepConfCommand(&cmd[3]);
	}
	else
		tcDMASendStr(&tcPc,"0M Command unknown\r");
}


/*
 * If first char on command line is G.
 * Used for move command
 */
void Gcommand(char *cmd)
{
	if( (cmd[0] == '0' || cmd[0] == '1') && (cmd[1] == ' '))
		moveCommand(&cmd[2]);

	else if(cmd[0] == '9'&& cmd[1] == '0' && cmd[2] == ' ')
	{
		hcSetCoordMode(&hc, absolute);
		tcDMASendStr(&tcPc,"0OK\r");
	}

	else if(cmd[0] == '9'&& cmd[1] == '1' && cmd[2] == ' ')
	{
		hcSetCoordMode(&hc, relative);
		tcDMASendStr(&tcPc,"0OK\r");
	}
	else if(cmd[0] == '2'&& cmd[1] == '8' && cmd[2] == ' ' && appState == waiting)
	{
		homeCommand(&cmd[3]);
	}
	else
		tcDMASendStr(&tcPc,"0G Command unknown\r");
}

void feederCommand(char *cmd)
{
	if(cmd[0] == 'A')
	{
		sendFeederList();
	}
	else if(cmd[0] == 'G'&& cmd[1] == ' ')
	{
		getFeederParam(&cmd[2]);
	}
	else if(cmd[0] == 'S'&& cmd[1] == ' ')
	{
		setFeederParam(&cmd[2]);
	}
	else if(cmd[0] == 'F'&& cmd[1] == ' ')
	{
		flashSaveFeeder(&cmd[2]);
	}
	else if(cmd[0] == 'M'&& cmd[1] == ' ')
	{
		feederMove(&cmd[2]);
	}
}

void specialCommandCommand(char *cmd)
{
	if(cmd[0] == '9' && cmd[1] == '9' && cmd[2] == ' ')
	{
		unsigned int motorMask= 0;
		if(cmd[3] == 0)
		{
			motorMask = 0x0F;
		}
		else
		{
			if(strchr (&cmd[3],'X'))
				motorMask |= 0x01;
			if(strchr (&cmd[3],'Y'))
				motorMask |= 0x02;
			if(strchr (&cmd[3],'Z'))
				motorMask |= 0x04;
			if(strchr (&cmd[3],'C'))
				motorMask |= 0x08;
		}
		hcStop(&hc, motorMask);
	}
	else if(cmd[0] == '0' && cmd[1] == '0')
	{
		driverDisable(X);
		driverDisable(Y);
		driverDisable(Z);
		driverDisable(T);
	}
	else if(cmd[0] == '1' && cmd[1] == '1' )
	{
		driverEnable(X);
		driverEnable(Y);
		driverEnable(Z);
		driverEnable(T);
	}
	else if(cmd[0] == '2' && cmd[1] == '0' )
	{
		watchdogMode = 0;
	}
	else if(cmd[0] == '2' && cmd[1] == '1' )
	{
		watchdogMode = 1;
	}
	else if(cmd[0] == '2' && cmd[1] == '2' )
	{
		watchDogTime = userTick;
	}
	else if(cmd[0] == '3' && cmd[1] == '0' )
	{
		pipe1StatusMode = 0;
	}
	else if(cmd[0] == '3' && cmd[1] == '1' )
	{
		pipe1StatusMode = 1;
	}
}

/*
 * Check if command is received and execute is.
 */
void externalCom(void)
{
	if(rcGetCommand(&rcPc,bufferRc))
	{
		if((bufferRc[0] == 'G'))
			Gcommand(&bufferRc[1]);
		else if(bufferRc[0] == 'M')
			confCommand(&bufferRc[1]);
		else if(bufferRc[0] == 'R')
			readCommand(&bufferRc[1]);
		else if(bufferRc[0] == 'F')
			feederCommand(&bufferRc[1]);
		else if(bufferRc[0] == 'S')
			specialCommandCommand(&bufferRc[1]);
		else
			tcDMASendStr(&tcPc,"0Command unknown\r");
	}
}

void scanLine(void)
{
	if(hcIsMoving(&hc) && scanRequest.nbMesure < SIZE_SCAN_DATA)
	{
		float posTemp[4];
		hcGetPos(&hc, posTemp);
		if ((posTemp[scanRequest.idAxis] - scanRequest.lastScanPos) > scanRequest.stepMesurment)
		{
			scanRequest.axisScanVal[scanRequest.nbMesure] = posTemp[scanRequest.idAxis];
			scanRequest.lastScanPos = posTemp[scanRequest.idAxis];
			VCNL3040_getProximity(&scanSensor,&scanRequest.scanVal[scanRequest.nbMesure]);
			scanRequest.nbMesure++;
		}
	}
	else // send data;
	{
		char lineOut[30];
		sprintf(lineOut,"0%u\r",scanRequest.nbMesure);
		tcDMASendStr(&tcPc,lineOut);
		for(unsigned int i=0; i<scanRequest.nbMesure; i++)
		{
			sprintf(lineOut,"0%f %u\r",scanRequest.axisScanVal[i], scanRequest.scanVal[i]);
			tcDMASendStr(&tcPc,lineOut);
		}
		scanRequest.isEnable = 0;
	}
}

void homeFunction(void)
{
	static float oldSpeed[4];
	static unsigned int timeTmp;

	if(appHomingState == home_start)
	{
		for(unsigned int i = 0; i<4; i++)
			oldSpeed[i] = PAPgetSpeed(&(hc.motorPap[i]));
		// Move to switch
		for(unsigned int i = 0 ; i < 2; i++)
		{
			if(axisEenableRequest & (0x01 << i))
			{
				PAPsetSpeed(&(hc.motorPap[i]),10.0f);
				PAPmoveRequest(&(hc.motorPap[i]),-200000,1.0f);
			}
		}
		if(axisEenableRequest & (0x01 << 3))
		{
			PAPsetSpeed(&(hc.motorPap[Z]),10.0f);
			PAPmoveRequest(&(hc.motorPap[Z]),200000,1.0f);
		}
		appHomingState = home_waitMoving;
	}
	else if(appHomingState == home_waitMoving)
	{
		if(!hcIsMoving(&hc))
			appHomingState = home_switchEscape;
	}
	else if(appHomingState == home_switchEscape)
	{
		if(axisIsEna(X) || axisIsEna(Y) || axisIsEna(Z))
		{
			for(unsigned int i = 0 ; i < 3; i++)
			{
				if(axisIsEna(i))
				{
					if(!hc.motorPap[i].pfHome())
					{
						axisEenableRequest &= ~((0x01<<i)&0x07);
						//homeEna[i] = 0;
						hc.motorPap[i].stepPos = 0;
					}
					else
					{
						if(i == Z)
							PAPmoveRequest(&(hc.motorPap[i]),-1,0.2f);
						else
							PAPmoveRequest(&(hc.motorPap[i]),1,0.2f);
					}
				}
			}
			timeTmp = userTick;
			appHomingState = home_delay;
		}
		else
		{
			appHomingState = home_speedReconfigure;
		}
	}
	else if(appHomingState == home_delay)
	{
		if( (userTick - timeTmp) > 1)
		{
			appHomingState = home_switchEscape;
		}
	}
	else if(appHomingState == home_speedReconfigure)
	{
		for(unsigned int i = 0 ; i < 4; i++)
		{
			PAPsetSpeed(&(hc.motorPap[i]),oldSpeed[i]);
		}
		appHomingState = home_end;
	}
}



void printStatus(void)
{
	char statBuffTmp[100];
	float pos[4];
	float presure;
	hcGetPos(&hc,pos);
	ABP_readPresureFloat(&presureSensor, &presure);
	sprintf(statBuffTmp,"1X:%.4f Y:%.4f Z:%.4f C:%.4f P:%.4f\r",pos[X],pos[Y],pos[Z],pos[T], presure);
	tcDMASendStr(&tcPc,statBuffTmp);
}

void appLoop(void)
{
	externalCom();
	if(appState == moveRequest)
	{
		hcMove(&hc, moveRequestArr, axisEenableRequest);
		watchDogTime = userTick;
		appState = moving;
	}
	else if(appState == moving)
	{
		if(!hcIsMoving(&hc))
		{
			appState = waiting;
		}
		if(watchdogMode && ((userTick - watchDogTime)>WATCHDOG_TIMEOUT))
		{
			hcStop(&hc,0x0F); // Stop all motors
		}
	}
	else if(appState == homing)
	{
		homeFunction();
		if(appHomingState == home_end)
			appState = waiting;
	}

	if(pipe1StatusMode && ((userTick-statusTime)>STATUS_SEND_DELAY))
	{
		printStatus();
		statusTime = userTick;
	}
	if(scanRequest.isEnable)
	{
		scanLine();
	}
}




