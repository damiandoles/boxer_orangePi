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

#define TEST_DB_INSERT_STATEMENT

int main(void)
{
	Uart_Init();
	DataBase_Init();

#ifdef TEST_DB_INSERT_STATEMENT
	//only for tests
	basic_meas_t testBasicMeas;
	testBasicMeas.humidity 		= "45";
	testBasicMeas.lux 			= "16678";
	testBasicMeas.temp_up 		= "29.5";
	testBasicMeas.temp_middle 	= "28.4";
	testBasicMeas.temp_down 	= "27.7";
	testBasicMeas.soil_moist 	= "DRY";

	DataBase_InsertBasicMeas(&testBasicMeas);

	ph_meas_t testPhMeas;
	testPhMeas.ph_soil 	= "6.78";
	testPhMeas.ph_water = "7.11";

	DataBase_InsertPhMeas(&testPhMeas);
#endif

	while (true)
	{
		Uart_RxHandler();
	}

	return EXIT_SUCCESS;
}
