/*
 * defines.h
 *
 *  Created on: Sep 29, 2017
 *      Author: dolewdam
 */

#ifndef INC_DEFINES_H_
#define INC_DEFINES_H_

// USER DEFINES

#define DOCUMENT_ROOT 		"/home/dolewdam/Git_Repos/Prywatne/boxer_orangepi_www/website/source"
#define SERIAL_PORT_NAME	"/dev/ttyACM0"

#define NO_SSL

#ifdef NO_SSL
#define SERVER_PORT 		"8080"
#else
#define SERVER_PORT 		"8080,9090s"
#endif

#define MAIN_URI 			"/index.html"

#endif /* INC_DEFINES_H_ */
