/*
 * com.c
 *
 *  Created on: Oct 9, 2019
 *      Author: bastien
 */

#include "com_old.h"





void rcClearBuffer(volatile receptionController *rc)
{
	for(unsigned int i = 0; i<MAX_BYTERECEP ; i++)
	{
		rc->buffRecep[i] = 0;
	}

	rc->ptrByteToWrite =  0;
	rc->ptrByteToRead =  0;
	rc->bytePending = 0;
	rc->cmdPending = 0;
	#ifdef COM_USE_RTS
	USART_READY
	#endif
}

/*
 * Init buffer reception.
 */
void rcInit(volatile receptionController *rc,char endL)
{
	rc->endCmd = endL;
	rcClearBuffer(rc);
}




/*
 * Function copy first command received in bufferCmd.
 * Make sure bufferCmd have free memory for command, max is defined by MAX_BYTERECEP.
 * Return 1 if command available, otherwise return 0;
 */
unsigned char rcGetCommand(volatile receptionController *rc, char *bufferCmd)
{
	if(rc->cmdPending)
	{
		do
		{
			*bufferCmd = rcGetFirstChar(rc);
			bufferCmd++;
		}while(*(bufferCmd-1) != rc->endCmd);
		*(bufferCmd-1) = 0; //string terminator
		rc->cmdPending--;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
void toUpper(char *str)
{
	while(*str)
	{
		if( ((*str) >= 'a') && ((*str) <= 'z') )
			(*str) -= 32;

		str++;
	}
}*/

/***********************************************************************************************************/
/************************************************TRANSMITION CONTROLER****************************************/
/***********************************************************************************************************/

void tcClearBuffer(volatile transmitterController *tc)
{
	for(unsigned int i = 0; i<MAX_BYTEEMIT ; i++)
	{
		tc->buffEmit[i] = 0;
	}
	tc->byteToSend = 0;
	tc->byteToAdd = 0;
	tc->bytePending = 0;
}

void tcInit(volatile transmitterController *tc, USART_TypeDef *us)
{
	//tc->pfCmdSend = pf;
	tc->myUsart = us;
	tcClearBuffer(tc);
}


/*
 * Send string.
 * Note: buffer must be terminated by \0.
 */
void tcSendStr(volatile transmitterController *tc, char *str)
{
	while(*str)
	{
		tcSend(tc, *str);
		str++;
	}
}


/*
 * Send buffer.
 */
void tcSendBuffer(volatile transmitterController *tc, char *str,unsigned int size)
{
	for(unsigned int i = 0; i<size ; i++)
	{
		tcSend(tc, str[i]);
	}
}



