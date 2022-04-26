#include "VCNL3040.h"

const uint8_t VCNL3040_ADDR = 0xC0; //7-bit unshifted I2C address of VCNL4040

//Used to select between upper and lower byte of command register
#define LOWER 1
#define UPPER 0

//VCNL4040 Command Codes
//#define VCNL4040_ALS_CONF 0x00
//#define VCNL4040_ALS_THDH 0x01
//#define VCNL4040_ALS_THDL 0x02
#define VCNL3040_PS_CONF1 0x03 //Lower
#define VCNL3040_PS_CONF2 0x03 //Upper
#define VCNL3040_PS_CONF3 0x04 //Lower
#define VCNL3040_PS_MS 0x04 //Upper
#define VCNL3040_PS_CANC 0x05
#define VCNL3040_PS_THDL 0x06
#define VCNL3040_PS_THDH 0x07
#define VCNL3040_PS_DATA 0x08
//#define VCNL4040_ALS_DATA 0x09
//#define VCNL4040_WHITE_DATA 0x0A
#define VCNL3040_INT_FLAG 0x0B //Upper
#define VCNL3040_ID 0x0C


//Check comm with sensor and set it to default init settings
VCNL3040Error_e VCNL3040_Init(VCNL3040 *dev, I2C_TypeDef *i2cPort)
{
  dev->_i2cPort = i2cPort; //Grab which port the user wants us to use

  //if (VCNL3040_isConnected(dev) == 0)
	//  return 0; //I2C comm failure

  uint16_t id = 0;
  VCNL3040Error_e err = VCNL3040_getID(dev, &id);

  if (err != VCNL3040_ERROR_OK)
	  return err;
  if (id != 0x0186)
	  return VCNL3040_ERROR_WRONG_ID; //Check default ID value

  //Configure the various parts of the sensor
  VCNL3040_setLEDCurrent(dev, 200); //Max IR LED current
  VCNL3040_setIRDutyCycle(dev, 40); //Set to highest duty cycle
  VCNL3040_setProxIntegrationTime(dev, 2); //Set to max integratio
  VCNL3040_setProxResolution(dev,16); //Set to 16-bit output
  VCNL3040_enableSmartPersistance(dev); //Turn on smart presistance
  VCNL3040_powerOnProximity(dev); //Turn on prox sensing
  return VCNL3040_ERROR_OK;
}

//Test to see if the device is responding
unsigned int VCNL3040_isConnected(VCNL3040 *dev)
{
	uint16_t id;
	return (VCNL3040_getID(dev, &id) == VCNL3040_ERROR_OK) ? 1 : 0;
}


//Set the duty cycle of the IR LED. The higher the duty
//ratio, the faster the response time achieved with higher power
//consumption. For example, PS_Duty = 1/320, peak IRED current = 100 mA,
//averaged current consumption is 100 mA/320 = 0.3125 mA.
VCNL3040Error_e VCNL3040_setIRDutyCycle(VCNL3040 *dev, uint16_t dutyValue)
{
  if(dutyValue > 320 - 1) dutyValue = VCNL3040_PS_DUTY_320;
  else if(dutyValue > 160 - 1) dutyValue = VCNL3040_PS_DUTY_160;
  else if(dutyValue > 80 - 1) dutyValue = VCNL3040_PS_DUTY_80;
  else dutyValue = VCNL3040_PS_DUTY_40;
  
  return VCNL3040_bitMask(dev, VCNL3040_PS_CONF1, LOWER, VCNL3040_PS_DUTY_MASK, dutyValue);
}

//Set the Prox interrupt persistance value
//The PS persistence function (PS_PERS, 1, 2, 3, 4) helps to avoid
//false trigger of the PS INT. It defines the amount of
//consecutive hits needed in order for a PS interrupt event to be triggered.
VCNL3040Error_e VCNL3040_setProxInterruptPersistance(VCNL3040 *dev, uint8_t persValue)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF1, LOWER, VCNL3040_PS_PERS_MASK, persValue);
}

// Sets the integration time for the proximity sensor //
VCNL3040Error_e VCNL3040_setProxIntegrationTime(VCNL3040 *dev, uint8_t timeValue)
{
  if(timeValue > 8 - 1) timeValue = VCNL3040_PS_IT_8T;
  else if(timeValue > 4 - 1) timeValue = VCNL3040_PS_IT_4T;
  else if(timeValue > 3 - 1) timeValue = VCNL3040_PS_IT_3T;
  else if(timeValue > 2 - 1) timeValue = VCNL3040_PS_IT_2T;
  else timeValue = VCNL3040_PS_IT_1T;

  return VCNL3040_bitMask(dev, VCNL3040_PS_CONF1, LOWER, VCNL3040_PS_IT_MASK, timeValue);
}

// Power on the prox sensing portion of the device //
VCNL3040Error_e VCNL3040_powerOnProximity(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF1, LOWER, VCNL3040_PS_SD_MASK, VCNL3040_PS_SD_POWER_ON);
}

// Power off the prox sensing portion of the device //
VCNL3040Error_e VCNL3040_powerOffProximity(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF1, LOWER, VCNL3040_PS_SD_MASK, VCNL3040_PS_SD_POWER_OFF);
}

// Sets the proximity resolution //
VCNL3040Error_e VCNL3040_setProxResolution(VCNL3040 *dev, uint8_t resolutionValue)
{
	if(resolutionValue > 16 - 1) resolutionValue = VCNL3040_PS_HD_16_BIT;
	else resolutionValue = VCNL3040_PS_HD_12_BIT;
	
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF2, UPPER, VCNL3040_PS_HD_MASK, resolutionValue);
}

//Sets the proximity interrupt type
VCNL3040Error_e VCNL3040_setProxInterruptType(VCNL3040 *dev, uint8_t interruptValue)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF2, UPPER, VCNL3040_PS_INT_MASK, interruptValue);
}

//Enable smart persistance
//To accelerate the PS response time, smart
//persistence prevents the misjudgment of proximity sensing
//but also keeps a fast response time.
VCNL3040Error_e VCNL3040_enableSmartPersistance(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF3, LOWER, VCNL3040_PS_SMART_PERS_MASK, VCNL3040_PS_SMART_PERS_ENABLE);
}
VCNL3040Error_e VCNL3040_disableSmartPersistance(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF3, LOWER, VCNL3040_PS_SMART_PERS_MASK, VCNL3040_PS_SMART_PERS_DISABLE);
}

//Enable active force mode
//An extreme power saving way to use PS is to apply PS active force mode.
//Anytime host would like to request one proximity measurement,
//enable the active force mode. This
//triggers a single PS measurement, which can be read from the PS result registers.
//VCNL4040 stays in standby mode constantly.
VCNL3040Error_e VCNL3040_enableActiveForceMode(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF3, LOWER, VCNL3040_PS_FOR_MASK, VCNL3040_PS_FOR_ENABLE);
}
VCNL3040Error_e VCNL3040_disableActiveForceMode(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF3, LOWER, VCNL3040_PS_FOR_MASK, VCNL3040_PS_FOR_DISABLE);
}

//Set trigger bit so sensor takes a force mode measurement and returns to standby
VCNL3040Error_e VCNL3040_takeSingleProxMeasurement(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_CONF3, LOWER, VCNL3040_PS_FOR_TRIG_MASK, VCNL3040_PS_FOR_TRIG_ONETIME);
}



//Enable the proximity detection logic output mode
//When this mode is selected, the INT pin is pulled low when an object is
//close to the sensor (value is above high
//threshold) and is reset to high when the object moves away (value is
//below low threshold). Register: PS_THDH / PS_THDL
//define where these threshold levels are set.
VCNL3040Error_e VCNL3040_enableProxLogicMode(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_MS, UPPER, VCNL3040_PS_MS_MASK, VCNL3040_PS_MS_ENABLE);
}
VCNL3040Error_e VCNL3040_disableProxLogicMode(VCNL3040 *dev)
{
	return VCNL3040_bitMask(dev, VCNL3040_PS_MS, UPPER, VCNL3040_PS_MS_MASK, VCNL3040_PS_MS_DISABLE);
}

//Set the IR LED sink current to one of 8 settings
VCNL3040Error_e VCNL3040_setLEDCurrent(VCNL3040 *dev, uint8_t currentValue)
{
	if(currentValue > 200 - 1) currentValue = VCNL3040_LED_200MA;
	else if(currentValue > 180 - 1) currentValue = VCNL3040_LED_180MA;
	else if(currentValue > 160 - 1) currentValue = VCNL3040_LED_160MA;
	else if(currentValue > 140 - 1) currentValue = VCNL3040_LED_140MA;
	else if(currentValue > 120 - 1) currentValue = VCNL3040_LED_120MA;
	else if(currentValue > 100 - 1) currentValue = VCNL3040_LED_100MA;
	else if(currentValue > 75 - 1) currentValue = VCNL3040_LED_75MA;
	else currentValue = VCNL3040_LED_50MA;

	return VCNL3040_bitMask(dev, VCNL3040_PS_MS, UPPER, VCNL3040_LED_I_MASK, currentValue);
}

//Set the proximity sensing cancelation value - helps reduce cross talk
//with ambient light
VCNL3040Error_e VCNL3040_setProxCancellation(VCNL3040 *dev, uint16_t cancelValue)
{
	return VCNL3040_writeCommand(dev, VCNL3040_PS_CANC, cancelValue);
}



//Value that Proximity Sensing must go above to trigger an interrupt
VCNL3040Error_e VCNL3040_setProxHighThreshold(VCNL3040 *dev, uint16_t threshold)
{
	return VCNL3040_writeCommand(dev, VCNL3040_PS_THDH, threshold);
}

//Value that Proximity Sensing must go below to trigger an interrupt
VCNL3040Error_e VCNL3040_setProxLowThreshold(VCNL3040 *dev, uint16_t threshold)
{
	return VCNL3040_writeCommand(dev, VCNL3040_PS_THDL, threshold);
}

//Read the Proximity value
VCNL3040Error_e VCNL3040_getProximity(VCNL3040 *dev, uint16_t *prox)
{
	return VCNL3040_readCommand(dev, VCNL3040_PS_DATA, prox);
}



//Read the sensors ID
VCNL3040Error_e VCNL3040_getID(VCNL3040 *dev, uint16_t *id)
{
  return VCNL3040_readCommand(dev, VCNL3040_ID, id);
}

/*
//Returns true if the prox value rises above the upper threshold
unsigned int VCNL3040_isClose()
{
  uint8_t interruptFlags = VCNL3040_readCommandUpper(dev, VCNL3040_INT_FLAG);
  return (interruptFlags & VCNL3040_INT_FLAG_CLOSE);
}

//Returns true if the prox value drops below the lower threshold
unsigned int VCNL3040_isAway()
{
  uint8_t interruptFlags = VCNL3040_readCommandUpper(dev, VCNL3040_INT_FLAG);
  return (interruptFlags & VCNL3040_INT_FLAG_AWAY);
}*/

/*
//Returns true if the prox value rises above the upper threshold
unsigned int VCNL3040_isLight()
{
  uint8_t interruptFlags = VCNL3040_readCommandUpper(dev, VCNL4040_INT_FLAG);
  return (interruptFlags & VCNL4040_INT_FLAG_ALS_HIGH);
}

//Returns true if the ALS value drops below the lower threshold
unsigned int VCNL3040_isDark()
{
  uint8_t interruptFlags = VCNL3040_readCommandUpper(dev, VCNL4040_INT_FLAG);
  return (interruptFlags & VCNL4040_INT_FLAG_ALS_LOW);
}
*/

//Reads two consecutive bytes from a given 'command code' location
//return 1 if succes.
VCNL3040Error_e VCNL3040_readCommand(VCNL3040 *dev, uint8_t commandCode, uint16_t *readData)
{
	uint8_t rData[2];
	unsigned int succes = I2CRead(dev->_i2cPort, VCNL3040_ADDR, commandCode, 2, rData);
	*readData = (((uint16_t)rData[1]) << 8) | rData[0];

	return succes ? VCNL3040_ERROR_OK : VCNL3040_ERROR_TIMEOUT;
}

//Write two bytes to a given command code location (8 bits)
//return VCNL3040_ERROR_OK if succes.
VCNL3040Error_e VCNL3040_writeCommand(VCNL3040 *dev, uint8_t commandCode, uint16_t value)
{
	unsigned char send[2];
	send[0] = value & 0xFF;
	send[1] = value >> 8;
	return I2CWrite(dev->_i2cPort, VCNL3040_ADDR, commandCode, 2, send) ? VCNL3040_ERROR_OK : VCNL3040_ERROR_TIMEOUT;
}

//Given a command code (address) write to the lower byte without affecting the upper byte
VCNL3040Error_e VCNL3040_writeCommandLower(VCNL3040 *dev, uint8_t commandCode, uint8_t newValue)
{
  uint16_t commandValue;
  VCNL3040Error_e err = VCNL3040_readCommand(dev, commandCode, &commandValue);
  if(err != VCNL3040_ERROR_OK)
	  return err;
  commandValue &= 0xFF00; //Remove lower 8 bits
  commandValue |= (uint16_t)newValue; //Mask in
  return VCNL3040_writeCommand(dev, commandCode, commandValue);
}

//Given a command code (address) write to the upper byte without affecting the lower byte
VCNL3040Error_e VCNL3040_writeCommandUpper(VCNL3040 *dev, uint8_t commandCode, uint8_t newValue)
{
  uint16_t commandValue;
  VCNL3040Error_e err = VCNL3040_readCommand(dev, commandCode, &commandValue);
  if(err != VCNL3040_ERROR_OK)
	  return err;
  commandValue &= 0x00FF; //Remove upper 8 bits
  commandValue |= (uint16_t)newValue << 8; //Mask in
  return (VCNL3040_writeCommand(dev, commandCode, commandValue));
}

//Given a command code (address) read the lower byte
VCNL3040Error_e VCNL3040_readCommandLower(VCNL3040 *dev, uint8_t commandCode, uint8_t *commandValue)
{
	uint16_t rData;
	VCNL3040Error_e err = VCNL3040_readCommand(dev, commandCode, &rData);
	*commandValue = (uint8_t)(rData & 0xFF);
	return err;
}

//Given a command code (address) read the upper byte
VCNL3040Error_e VCNL3040_readCommandUpper(VCNL3040 *dev, uint8_t commandCode, uint8_t *commandValue)
{
	uint16_t rData;
	VCNL3040Error_e err = VCNL3040_readCommand(dev, commandCode, &rData);
	*commandValue = (uint8_t)(rData >> 8);
	return err;
}

//Given a register, read it, mask it, and then set the thing
//commandHeight is used to select between the upper or lower byte of command register
//Example:
//Write dutyValue into PS_CONF1, lower byte, using the Duty_Mask
//bitMask(VCNL4040_PS_CONF1, LOWER, VCNL4040_PS_DUTY_MASK, dutyValue);
VCNL3040Error_e VCNL3040_bitMask(VCNL3040 *dev, uint8_t commandAddress, unsigned int commandHeight, uint8_t mask, uint8_t thing)
{
  // Grab current register context
  uint8_t registerContents;
  VCNL3040Error_e err;

  if (commandHeight == LOWER) err = VCNL3040_readCommandLower(dev, commandAddress, &registerContents);
  else err = VCNL3040_readCommandUpper(dev, commandAddress, &registerContents);
  if(err != VCNL3040_ERROR_OK)
	  return err;

  // Zero-out the portions of the register we're interested in
  registerContents &= mask;

  // Mask in new thing
  registerContents |= thing;

  // Change contents
  if (commandHeight == LOWER) err = VCNL3040_writeCommandLower(dev, commandAddress, registerContents);
  else err = VCNL3040_writeCommandUpper(dev, commandAddress, registerContents);
  return err;
}
