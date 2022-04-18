/*
 * com.h
 *
 *  Created on: Oct 9, 2019
 *      Author: bastien
 */

#ifndef COM_H_
#define COM_H_
#include "usart.h"
#include "gpio.h"

#define MAX_BYTERECEP 250
#define MAX_BYTEEMIT  250
#define MAX_SIZE_FULL_BUFFER 230 // MAX_BYTERECEP-20 for prevent buffer overflow

//#define COM_USE_RTS
#define USART_FULL GPIOA->ODR |= GPIO_ODR_ODR_6;//LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_6);
#define USART_READY GPIOA->ODR &= (~GPIO_ODR_ODR_6);//LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_6);

typedef struct receptionController receptionController;
struct receptionController{
	volatile char buffRecep[MAX_BYTERECEP]; // reception buffer
	volatile unsigned int ptrByteToWrite;
	volatile unsigned int ptrByteToRead;
	//volatile char bufferCmdOut[MAX_BYTERECEP]; // buffer temporarily used for transmit command to user application.
	volatile unsigned int bytePending; // amount of byte received not used.
	volatile unsigned int cmdPending; // amount of command received not used.
	volatile char endCmd; // char containt endLine command. (ex '\r')
};



void rcClearBuffer(volatile receptionController *rc);
void rcInit(volatile receptionController *rc,char endL);
//void rcAddCharToCmd(volatile receptionController *rc, char data);
unsigned char rcGetCommand(volatile receptionController *rc, char *bufferCmd);
//volatile char* rcGetInputCmd(volatile receptionController *rc, unsigned int delay_ms);


/*
 * Return 1 if buffer is full;
 */

__attribute__((always_inline))
static inline unsigned char rcIsFull(volatile receptionController *rc)
{
	if(rc->bytePending >= MAX_SIZE_FULL_BUFFER)
	{
		return 1;
	}
	else
		return 0;
}


/*
 * Return 1 if buffer is void;
 */
__attribute__((always_inline))
static inline unsigned char rcIsVoid(volatile receptionController *rc)
{
	if(rc->bytePending)
		return 0;
	else
		return 1;
}

/*
 * add char to buffer user rtception.
 * Function must be called by usart reception IT.
 * Not for user.
 */
__attribute__((always_inline))
static inline void rcAddCharToCmd(volatile receptionController *rc, char data)
{
	rc->buffRecep[rc->ptrByteToWrite] = data;
	rc->bytePending++;
#ifdef COM_USE_RTS
	if(rcIsFull(rc))
		USART_FULL
#endif

	if(rc->ptrByteToWrite == MAX_BYTERECEP-1)
		rc->ptrByteToWrite = 0;
	else
		rc->ptrByteToWrite++;

	if(data == rc->endCmd)
		rc->cmdPending++;
}

/*
 * Return the first char received.
 * Make sure char is received before call this.
 * User must NEVER use this function, it's called only by rcGetCommand().
 */
__attribute__((always_inline))
static inline unsigned char rcGetFirstChar(volatile receptionController *rc)
{
	char dataOut = rc->endCmd;
	if(!rcIsVoid(rc))
	{

		USART1->CR1 &= (~USART_CR1_RXNEIE);

		dataOut = rc->buffRecep[rc->ptrByteToRead];
		rc->bytePending--;

		USART1->CR1 |= USART_CR1_RXNEIE;
		if(rc->ptrByteToRead == MAX_BYTERECEP-1)
			rc->ptrByteToRead = 0;
		else
			rc->ptrByteToRead++;
#ifdef COM_USE_RTS
		if(!rcIsFull(rc))
			USART_READY
#endif
	}
	return dataOut;
}


/****************************************************************************************/
/*****************************TRASMITION CONTROLLER**************************************/
/****************************************************************************************/

typedef struct transmitterController transmitterController;
struct transmitterController{
	volatile char *buffEmit;
	unsigned int size;
	volatile unsigned int byteToSend;
	volatile unsigned int byteToAdd;
	volatile unsigned int bytePending;
	USART_TypeDef *myUsart;
	DMA_TypeDef *DMAx;
	uint32_t DMAChannel;
};


void tcClearBuffer(volatile transmitterController *tc);
void tcInit(volatile transmitterController *tc, USART_TypeDef *us, volatile char *buffAdr, unsigned int siz);
void tcSendStr(volatile transmitterController *tc, char *str);
void tcIntSendStr(volatile transmitterController *tc, char *str);
void tcSendBuffer(volatile transmitterController *tc, char *str,unsigned int size);
void tcDMAconfigure(volatile transmitterController *tc, DMA_TypeDef *DMAid, uint32_t Channelid);
void tcDMASend(volatile transmitterController *tc, unsigned int size);
void tcDMASendStr(volatile transmitterController *tc, char *str);


/*
 * Return 1 if buffer is full;
 */
__attribute__((always_inline))
static inline unsigned char tcIsFull(volatile transmitterController *tc)
{
	if(tc->bytePending >= tc->size)
	{
		return 1;
	}
	else
		return 0;
}


/*
 * Return 1 if buffer is void;
 */
__attribute__((always_inline))
inline unsigned char tcIsVoid(volatile transmitterController *tc)
{
	if(tc->bytePending)
		return 0;
	else
		return 1;
}

/*
 * Sent one char by uart.
 */
__attribute__((always_inline))
static inline void tcIntSend(volatile transmitterController *tc, char car)
{
	while(tcIsFull(tc));
	tc->buffEmit[tc->byteToAdd] = car;
	tc->byteToAdd++;
	tc->bytePending++;
	if(tc->byteToAdd == tc->size) tc->byteToAdd = 0; //prottction contre l'overflow
	tc->myUsart->CR1 |= USART_CR1_TXEIE; // enable interrupt.
}

/*
 * Function called by interruption when char send is complete.
 */
__attribute__((always_inline))
static inline void tcInteruptManagement(volatile transmitterController *tc)
{
	if(!tcIsVoid(tc))  // si il reste a transmetre
	{
		//tc->pfCmdSend(*(tc->ptrFifoOut));
		tc->myUsart->TDR = tc->buffEmit[tc->byteToSend];
		tc->byteToSend++;
		if(tc->byteToSend == tc->size) tc->byteToSend = 0; //prottction contre l'overflow
		tc->bytePending--;
	}
	else
	{
		tc->myUsart->CR1 &= (~USART_CR1_TXEIE); // disable interrupt.
	}
}

/*
 * Return 0 while transmission is onGoing.
 * Buffer void.
 */
__attribute__((always_inline))
static inline unsigned char tcTransmissionComplete(volatile transmitterController *tc)
{
	if((tcIsVoid(tc)) && (tc->myUsart->ISR & USART_ISR_TC))
		return 1;
	else
		return 0;
}








//void toUpper(char *str);


#endif /* COM_H_ */
