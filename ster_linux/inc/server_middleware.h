/*
 * server_middleware.h
 *
 *  Created on: Sep 20, 2017
 *      Author: dolewdam
 */

#ifndef INC_SERVER_MIDDLEWARE_H_
#define INC_SERVER_MIDDLEWARE_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "civetweb.h"

#define DOCUMENT_ROOT "/home/dolewdam/Git_Repos/Prywatne/boxer_orangepi_www/website/source"//"."
#ifdef NO_SSL
#ifdef USE_IPV6
#define PORT "[::]:8888,8884"
#else
#define PORT "8080"
#endif
#else
#ifdef USE_IPV6
#define PORT "[::]:8888r,[::]:8843s,8884"
#else
#define PORT "8888r,8843s,8884"
#endif
#endif
#define EXAMPLE_URI "/example"
#define EXIT_URI "/exit"

int exitNow;

#endif /* INC_SERVER_MIDDLEWARE_H_ */
