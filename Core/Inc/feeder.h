/*
 * feeder.h
 *
 *  Created on: Jun 4, 2020
 *      Author: bastien
 */

#ifndef INC_FEEDER_H_
#define INC_FEEDER_H_

#include "i2c.h"

#define MAX_FEEDER 100

#define FEEDER_CTRL     0x00
#define FEEDER_STATUS   0x01
#define FEEDER_SIZENAME 0x02
#define FEEDER_POSX 	0x03
#define FEEDER_POSY 	0x04
#define FEEDER_POSZ 	0x05
#define FEEDER_STPSIZE 	0x06
#define FEEDER_CMPBS 	0x07
#define FEEDER_NAME 	0x08

#define LED_BIT			0x01
#define STEP_BIT		0x02
#define FLASH_BIT		0x04

#define BUSY_BIT		0x01


#define I2C_FEEDER I2C3


void feederScan(void);
void feederLedOn(unsigned char addrF);
void feederLedOff(unsigned char addrF);
unsigned char feederGetStatus(unsigned char addrF);
unsigned char feederIsMoving(unsigned char addrF);
void feederGetName(unsigned char addrF, char *buffer);
void feederSetName(unsigned char addrF, char *buffer);
void feederGetPos(unsigned char addrF, vector3d *pos);
void feederSetPos(unsigned char addrF, vector3d pos);
void feederSetStepSize(unsigned char addrF, unsigned char val);
unsigned char feederGetStepSize(unsigned char addrF);
void feederSetCMPB(unsigned char addrF, unsigned char val);
unsigned char feederGetCMPB(unsigned char addrF);
void feederSendStep(unsigned char addrF);
void feederFlashSave(unsigned char addrF);

extern unsigned char feederlist[127]; // list of all feeder address.
extern unsigned char amountFeeder;


#endif /* INC_FEEDER_H_ */
