/*
 * com.c
 *
 *  Created on: Oct 9, 2019
 *      Author: bastien
 */

#include "com.h"
#include <string.h>
#include "stm32g4xx_ll_dma.h"



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
		*bufferCmd = 0; //string terminator
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
	for(unsigned int i = 0; i<tc->size ; i++)
	{
		tc->buffEmit[i] = 0;
	}
	tc->byteToSend = 0;
	tc->byteToAdd = 0;
	tc->bytePending = 0;
}

void tcInit(volatile transmitterController *tc, USART_TypeDef *us, volatile char *buffAdr, unsigned int siz)
{
	//tc->pfCmdSend = pf;
	tc->buffEmit = buffAdr;
	tc->size = siz;
	tc->myUsart = us;
	tcClearBuffer(tc);
}

void tcDMAconfigure(volatile transmitterController *tc, DMA_TypeDef *DMAid, uint32_t Channelid)
{
	tc->DMAx = DMAid;
	tc->DMAChannel = Channelid;
	LL_USART_EnableDMAReq_TX(tc->myUsart);

}



void tcDMASend(volatile transmitterController *tc, unsigned int size)
{

	//while(!LL_USART_IsActiveFlag_TC(tc->myUsart));
	//tc->DMAx->IFCR = (LL_DMA_ISR_TCIF1 << ((tc->DMAChannel-1) * 4));
	while(!LL_USART_IsActiveFlag_TC(tc->myUsart));
	LL_DMA_ClearFlag_TC1(tc->DMAx);

	LL_DMA_DisableChannel(tc->DMAx, tc->DMAChannel);
	//LL_DMA_DisableStream(tc->DMAx, tc->DMAChannel);
	LL_DMA_SetMemoryAddress(tc->DMAx, tc->DMAChannel, (unsigned int)tc->buffEmit);
	LL_DMA_SetPeriphAddress(tc->DMAx, tc->DMAChannel, (unsigned int)(&tc->myUsart->TDR));
	LL_DMA_SetDataLength(tc->DMAx, tc->DMAChannel, size);
	//LL_USART_ClearFlag_TC(tc->myUsart);
	//LL_DMA_EnableStream(tc->DMAx, tc->DMAChannel);
	LL_DMA_EnableChannel(tc->DMAx, tc->DMAChannel);
	//LL_DMA_ClearFlag_TC3(tc->DMAx);
	//tc->DMAx->IFCR = (LL_DMA_ISR_TCIF1 << ((tc->DMAChannel-1) * 4));

}

void tcDMASendStr(volatile transmitterController *tc, char *str)
{
	while(!LL_USART_IsActiveFlag_TC(tc->myUsart));
	for(unsigned int i = 0; str[i]; i++)
	{
		tc->buffEmit[i] = str[i];
	}
	tcDMASend(tc,strlen(str));
}

/*
 * Send string.
 * Note: buffer must be terminated by \0.
 */
void tcIntSendStr(volatile transmitterController *tc, char *str)
{
	while(*str)
	{
		tcIntSend(tc, *str);
		str++;
	}
}


/*
 * Send buffer.
 *//*
void tcIntSendBuffer(volatile transmitterController *tc, char *str,unsigned int size)
{
	for(unsigned int i = 0; i<size ; i++)
	{
		tcSend(tc, str[i]);
	}
}*/



