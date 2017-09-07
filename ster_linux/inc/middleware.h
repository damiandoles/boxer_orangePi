#ifndef __MICRO_HTTP_MIDDLEWARE_H__
#define __MICRO_HTTP_MIDDLEWARE_H__

#include "server.h"

/* Route */
#ifndef MAX_HTTP_ROUTES
#define MAX_HTTP_ROUTES  20
#endif

//#define STATIC_FILE_FOLDER "/home/doles/Documents/Git_repos/boxer_orangepi_www/website/source"
#define STATIC_FILE_FOLDER "/home/dolewdam/Git_Repos/Prywatne/boxer_orangepi_www/website/source"

/* Data type of server application function */
typedef HTTPREQ_CALLBACK SAF;

int AddRouteHandler(HTTPMethod, char *, SAF);
void Dispatch(HTTPReqMessage *, HTTPResMessage *);
uint8_t _ReadStaticFiles(HTTPReqMessage *req, HTTPResMessage *res);

#endif
