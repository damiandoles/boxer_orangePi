/*
 ============================================================================
 Name        : boxer_orangePi.c
 Author      : Damian Dolewski
 Version     :
 Copyright   : All rights reserved.
 Description : Orange Pi Zero Boxer main application (motherboard)
 ============================================================================
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#include "server.h"
#include "middleware.h"
#include "uart.h"
#include "database.h"
#include "http_request.h"

void * uartRxThread(void * param);
void * uartTxThread(void * param);
void * httpThread(void * param);

pthread_t uartRxThreadHandler, uartTxThreadHandler, httpThreadHandler;

void TerminateApp(int status)
{
	pthread_join(uartRxThreadHandler, NULL);
	pthread_join(uartTxThreadHandler, NULL);
	pthread_join(httpThreadHandler, NULL);
	Http_ServerClose(&srv);
	exit(EXIT_SUCCESS);
}

int main(void)
{
	signal(SIGINT, TerminateApp);
//	Uart_Init();
	DataBase_Init();
	DataBase_TestInsert();

	HttpMidd_RegisterUserHandlers();
	Http_ServerInit(&srv, HTTP_PORT);

	errno = pthread_create(&uartRxThreadHandler, NULL, uartRxThread, NULL);
	errno = pthread_create(&uartTxThreadHandler, NULL, uartTxThread, NULL);
	errno = pthread_create(&httpThreadHandler, NULL, httpThread, NULL);

	pthread_join(uartRxThreadHandler, NULL);
	pthread_join(uartTxThreadHandler, NULL);
	pthread_join(httpThreadHandler, NULL);

	return EXIT_SUCCESS;
}

void * uartRxThread(void * param)
{
	while (true)
	{
		Uart_RxHandler();
	}

	pthread_exit(NULL);
}

void * uartTxThread(void * param)
{
	while (true)
	{
		sleep(2);
#ifdef DEBUG_UART_TX
		printf("UART TX THREAD\r\n");
#endif
	}

	pthread_exit(NULL);
}

void * httpThread(void * param)
{
	while (true)
	{
		Http_ServerRun(&srv, Dispatch);
	}

	Http_ServerClose(&srv);
	pthread_exit(NULL);
}
