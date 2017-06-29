/*
 ============================================================================
 Name        : boxer_orangePi.c
 Author      : Damian Dolewski
 Version     :
 Copyright   : All rights reserved.
 Description : Orange Pi Zero Boxer main application (motherboard)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uart.h"
#include "database.h"

int main(void)
{
	Uart_Init();
	DataBase_Init();

	measurement_t testMeas;
	testMeas.humidity = 45;
	testMeas.lux = 16678;
	testMeas.ph_soil = 6.78;
	testMeas.ph_water = 7.11;
	testMeas.temp_up = 29.5;
	testMeas.temp_middle = 28.4;
	testMeas.temp_down = 27.7;
	testMeas.soil_moist = "DRY";

	DataBase_Insert(&testMeas);

	while (true)
	{
		Uart_RxHandler();
	}

	return EXIT_SUCCESS;
}
