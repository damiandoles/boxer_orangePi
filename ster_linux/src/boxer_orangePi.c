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
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#include <onion/onion.h>
#include <onion/handler.h>
#include <onion/log.h>

#include <onion/exportlocal.h>
#include <onion/static.h>
#include <onion/shortcuts.h>

#include "uart.h"
#include "database.h"

#define TEST_DB_INSERT_STATEMENT

onion * o = NULL;

void free_onion(int unused){
	ONION_INFO("Closing connections");
	onion_free(o);
	exit(0);
}

int main(void)
{
	Uart_Init();
	DataBase_Init();

#ifdef TEST_DB_INSERT_STATEMENT
	basic_meas_t testBasicMeas;
	strcpy(testBasicMeas.humidity, 		"48");
	strcpy(testBasicMeas.lux, 			"16678");
	strcpy(testBasicMeas.temp_up, 		"29.5");
	strcpy(testBasicMeas.temp_middle, 	"28.4");
	strcpy(testBasicMeas.temp_down, 	"27.7");
	strcpy(testBasicMeas.soil_moist, 	"DRY");

	DataBase_InsertBasicMeas(&testBasicMeas);

	ph_meas_t testPhMeas;

	strcpy(testPhMeas.ph_soil, 	"6.78");
	strcpy(testPhMeas.ph_water, "7.11");

	DataBase_InsertPhMeas(&testPhMeas);
#endif

	char *port="8080";
	const char *dirname="/home/dolewdam/Git_Repos/Prywatne/boxer_orangepi_www/website/source";

	onion_handler *dir=onion_handler_export_local_new(dirname);
	onion_handler_add(dir, onion_handler_static("<h1>404 - File not found.</h1>", 404) );

	o=onion_new(O_THREADED|O_DETACH_LISTEN|O_SYSTEMD);
	onion_set_root_handler(o, dir);


	onion_set_port(o, port);

	signal(SIGINT, free_onion);
	int error=onion_listen(o);
	if (error){
		perror("Cant create the server");
	}
	ONION_INFO("Detached. Printing a message every 120 seconds.");
	int i=0;

	while (true)
	{
		Uart_RxHandler();
		i++;
		ONION_INFO("Still alive. Count %d.",i);
		sleep(120);
	}

	onion_free(o);
	return EXIT_SUCCESS;
}
