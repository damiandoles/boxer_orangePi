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
#include "defines.h"

void ServerHttp_Init(void);

#endif /* INC_SERVER_MIDDLEWARE_H_ */
