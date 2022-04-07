/*
 * ioext.h
 *
 *  Created on: 8 oct. 2019
 *      Author: bastien
 */

#ifndef IOEXT_H_
#define IOEXT_H_

extern unsigned int actualState;

typedef enum maskIO maskIO;
enum maskIO{
	MOD0_X   = 0x00000001,
	MOD1_X   = 0x00000002,
	MOD2_X   = 0x00000004,
	EN_X     = 0x00000008,
	RST_X    = 0x00000010,
	MOD0_Y   = 0x00000020,
	MOD1_Y   = 0x00000040,
	MOD2_Y   = 0x00000080,
	EN_Y     = 0x00000100,
	RST_Y    = 0x00000200,
	MOD0_Z   = 0x00000400,
	MOD1_Z   = 0x00000800,
	MOD2_Z   = 0x00001000,
	EN_Z     = 0x00002000,
	RST_Z    = 0x00004000,
	MOD0_T   = 0x00008000,
	MOD1_T   = 0x00010000,
	MOD2_T   = 0x00020000,
	EN_T     = 0x00040000,
	RST_T    = 0x00080000,
	EV    	 = 0x00100000,
	EXT    	 = 0x00200000,

};

typedef enum drvChan drvChan;
enum drvChan{
	X = 0,
	Y = 1,
	Z = 2,
	T = 3,
};

typedef enum stepDiv stepDiv;
enum stepDiv{
	DIV1  = 0x00,
	DIV2  = 0x01,
	DIV4  = 0x02,
	DIV8  = 0x03,
	DIV16 = 0x04,
	DIV32 = 0x05,
};

typedef enum decayMode decayMode;
enum decayMode{
	slow  = 0x00,
	fast  = 0x01,
	mixed = 0x02,
};

void initExtIo(void);
void updateIo(unsigned int value);


__attribute__((always_inline))
static inline void resetIo(unsigned int mask)
{
	updateIo(actualState &= (~mask));
}

__attribute__((always_inline))
static inline void setIo(unsigned int mask)
{
	updateIo(actualState |= mask);
}

__attribute__((always_inline))
static inline void EVenable(void)
{
	setIo(EV);
}

__attribute__((always_inline))
static inline void EVdisable(void)
{
	resetIo(EV);
}


__attribute__((always_inline))
static inline void EXTenable(void)
{
	setIo(EXT);
}

__attribute__((always_inline))
static inline void EXTdisable(void)
{
	resetIo(EXT);
}


void driverEnable(drvChan drv);
void driverDisable(drvChan drv);
void driverResetOn(drvChan drv);
void driverResetOff(drvChan drv);
void setStepDiv(drvChan drv, stepDiv div);
void driverDir(drvChan drv, int dir);
void setDecayMode(drvChan drv,decayMode mode);

#endif /* IOEXT_H_ */
