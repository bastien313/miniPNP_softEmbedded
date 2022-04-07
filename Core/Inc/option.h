/*
 * option.h
 *
 *  Created on: Oct 31, 2019
 *      Author: bastien
 */

#ifndef OPTION_H_
#define OPTION_H_

#include "hardControl.h"

//#define FLASH_PAGE_SIZE (2048)

#define USER_PAGE 63


#define MOTOR_SIZE 3
#define COORD_SIZE 3

#define MOTOR_SPEED_OFF 0
#define MOTOR_ACCEL_OFF 1
#define MOTOR_STEPMM_OFF 2

#define X_OFF 0
#define Y_OFF 1
#define Z_OFF 2

#define MOTOR_X_POS 0
#define MOTOR_Y_POS (MOTOR_X_POS + MOTOR_SIZE)
#define MOTOR_Z_POS (MOTOR_Y_POS + MOTOR_SIZE)
#define MOTOR_T_POS (MOTOR_Z_POS + MOTOR_SIZE)
#define SCAN_POS    (MOTOR_T_POS + MOTOR_SIZE)
#define BOARD_POS    (SCAN_POS + COORD_SIZE)
#define HEAD_POS    (BOARD_POS + COORD_SIZE)

#define TOTAL_SIZE (HEAD_POS + COORD_SIZE)



unsigned char optReadFlash(volatile hardControl *hc);
void  optWriteFlash(volatile hardControl *hc);
void optWriteDefalut(volatile hardControl *hc);



#endif /* OPTION_H_ */
