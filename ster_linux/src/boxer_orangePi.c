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
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#include "server.h"
#include "middleware.h"
#include "uart.h"
#include "database.h"

void * uartRxThread(void * param);
void * uartTxThread(void * param);
void * httpThread(void * param);

HTTPServer srv;
pthread_t uartRxThreadHandler, uartTxThreadHandler, httpThreadHandler;

#ifdef _PARSE_SIGNAL_
#include <signal.h>

#ifdef _PARSE_SIGNAL_INT_
void SigRoutine_INT(int unused) {
	HTTPServerClose(&srv);
}
#endif
#endif

void IndexHTML(HTTPReqMessage *req, HTTPResMessage *res) {
	int fileFound = _ReadStaticFiles(req, res);
	if (fileFound != 1)
	{
		printf("index.html not found\r\n");
	}
	else
	{
		printf("index.html found in STATIC_FILE_FOLDER\r\n");
	}
}
void DB(HTTPReqMessage *req, HTTPResMessage *res) {
	int fileFound = _ReadStaticFiles(req, res);
	if (fileFound != 1)
	{
		printf("boxer.db not found\r\n");
	}
	else
	{
		printf("boxer.db found in STATIC_FILE_FOLDER\r\n");
	}
}
void CSS(HTTPReqMessage *req, HTTPResMessage *res) {
	int fileFound = _ReadStaticFiles(req, res);
	if (fileFound != 1)
	{
		printf("css not found\r\n");
	}
	else
	{
		printf("css found in STATIC_FILE_FOLDER\r\n");
	}
}

void JS(HTTPReqMessage *req, HTTPResMessage *res) {
	int fileFound = _ReadStaticFiles(req, res);
	if (fileFound != 1)
	{
		printf("js not found\r\n");
	}
	else
	{
		printf("js found in STATIC_FILE_FOLDER\r\n");
	}
}
void SaveData(HTTPReqMessage *req, HTTPResMessage *res) {

	char * data;
	size_t dataSize = strlen((const char*)req->Body);
	if (dataSize > 0)
	{
		data = (char*)calloc(dataSize, sizeof(char));
		strcpy(data, (const char *)req->Body);
		printf("SaveData POST request: %s\r\n", data);
	}
}

void TerminateApp(int status)
{
	pthread_join(uartRxThreadHandler, NULL);
//	pthread_join(uartTxThreadHandler, NULL);
	pthread_join(httpThreadHandler, NULL);
	exit(EXIT_SUCCESS);
}

int main(void)
{
	signal(SIGINT, TerminateApp);
	Uart_Init();
	DataBase_Init();
	DataBase_TestInsert();

	/* Register the routes. */
	AddRouteHandler(HTTP_GET, "/index.html", IndexHTML);
	AddRouteHandler(HTTP_GET, "/boxer.db",   DB);
	AddRouteHandler(HTTP_GET, "/css/style.css", CSS);
	AddRouteHandler(HTTP_GET, "/css/menu.css", CSS);
	AddRouteHandler(HTTP_GET, "/scripts/scripts.js", JS);
	AddRouteHandler(HTTP_GET, "/scripts/menu.js", JS);
	AddRouteHandler(HTTP_GET, "/scripts/sql.js", JS);
	AddRouteHandler(HTTP_GET, "/scripts/jquery-3.2.1.min.js", JS);
	AddRouteHandler(HTTP_POST, "/SaveData",   SaveData);

	/* Initial the HTTP server and make it listening on MHS_PORT. */
	HTTPServerInit(&srv, 8080);

	errno = pthread_create(&uartRxThreadHandler, NULL, uartRxThread, NULL);
//	errno = pthread_create(&uartTxThreadHandler, NULL, uartTxThread, NULL);
	errno = pthread_create(&httpThreadHandler, NULL, httpThread, NULL);

	pthread_join(uartRxThreadHandler, NULL);
//	pthread_join(uartTxThreadHandler, NULL);
	pthread_join(httpThreadHandler, NULL);

	return EXIT_SUCCESS;
}

void * uartRxThread(void * param)
{
	while (true)
	{
		Uart_RxHandler();
	}

	pthread_exit(NULL);
}

void * uartTxThread(void * param)
{
	while (true)
	{
		sleep(2);
		printf("UART TX THREAD\r\n");
	}

	pthread_exit(NULL);
}

void * httpThread(void * param)
{
	while (true)
	{
		HTTPServerRunLoop(&srv, Dispatch);
	}

	HTTPServerClose(&srv);
	pthread_exit(NULL);
}
