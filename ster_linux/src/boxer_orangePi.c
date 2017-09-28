/*
 ============================================================================
 Name        : boxer_orangePi.c
 Author      : Damian Dolewski
 Version     :
 Copyright   : All rights reserved.
 Description : Orange Pi Zero Boxer main application (motherboard)
 ============================================================================
 */

#include "uart.h"
#include "database.h"
#include "server_middleware.h"
#include "settings.h"

int main(void)
{
//	Uart_Init();
	DataBase_Init();
	DataBase_TestInsert();
	Settings_GetActual();
	ServerHttp_Init();

	pthread_join(uartRxThreadHandler, NULL);
	pthread_join(uartTxThreadHandler, NULL);
	pthread_mutex_destroy(&uartDB_mutex);

	return EXIT_SUCCESS;
}
