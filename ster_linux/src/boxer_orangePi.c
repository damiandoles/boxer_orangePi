/*
 ============================================================================
 Name        : boxer_orangePi.c
 Author      : Damian Dolewski
 Version     :
 Copyright   : All rights reserved.
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

int main(void)
{
	Uart_Init();

	while(1)
	{
		Uart_RxHandler();
	}

	return EXIT_SUCCESS;
}
