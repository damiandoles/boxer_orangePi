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
#include <stdbool.h>
#include "uart.h"

int main(void)
{
	Uart_Init();

	while (true)
	{
		Uart_RxHandler();
	}

	return EXIT_SUCCESS;
}
