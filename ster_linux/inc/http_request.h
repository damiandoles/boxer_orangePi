/*
 * http_request.h
 *
 *  Created on: Sep 6, 2017
 *      Author: doles
 */

#ifndef INC_HTTP_REQUEST_H_
#define INC_HTTP_REQUEST_H_

//#include <http_middleware.h>
//#include <server.h>
//
//HTTPServer srv;

#include "civetweb.h"

void HttpReq_RegisterUserHandlers(struct mg_context *ctx);

#endif /* INC_HTTP_REQUEST_H_ */
