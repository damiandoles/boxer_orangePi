/*
 * uart.h
 *
 *  Created on: 26 May 2017
 *      Author: Damian Dolewski
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <pthread.h>

pthread_t uartRxThreadHandler, uartTxThreadHandler;

void Uart_Init(void);
void Uart_RxHandler(void);

#endif /* INC_UART_H_ */
