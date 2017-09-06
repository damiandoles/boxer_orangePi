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
//#include <onion/onion.h>
//#include <onion/handler.h>
//#include <onion/log.h>
//
//#include <onion/exportlocal.h>
//#include <onion/static.h>
//#include <onion/shortcuts.h>

#include "uart.h"
#include "database.h"

//onion * o = NULL;
void * uartRxThread(void * param);
void * uartTxThread(void * param);

//void free_onion(int unused){
//	ONION_INFO("Closing connections");
//	onion_free(o);
//	exit(0);
//}
//onion_connection_status post_data(void *_, onion_request *req, onion_response *res){
//	if (onion_request_get_flags(req)&OR_HEAD){
//		onion_response_write_headers(res);
//		return OCS_PROCESSED;
//	}
//	const char *user_data=onion_request_get_post(req,"/SaveLamp");
//	onion_response_printf(res, "The user wrote: %s", user_data);
//
//	printf("TEEEEST\r\n");
//	return OCS_PROCESSED;
//}

/* The HTTP server of this process. */
HTTPServer srv;

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
int main(void)
{
	Uart_Init();
	DataBase_Init();
	DataBase_TestInsert();

//	const char * port = "8080";
//	const char * dirname = "/home/dolewdam/Git_Repos/Prywatne/boxer_orangepi_www/website/source";
//
//	onion_handler *dir = onion_handler_export_local_new(dirname);
//	onion_handler_add(dir, onion_handler_static("<h1>404 - File not found.</h1>", 404) );
//
//	o=onion_new(O_THREADED|O_DETACH_LISTEN|O_SYSTEMD);
//	onion_set_root_handler(o, dir);
//
//	onion_handler * test = onion_handler_new((void*)post_data, NULL, NULL);
//	onion_url * url = onion_url_new();
//	onion_root_url(o);
//
//	onion_url_add_handler(url, "*", test);
//
//	onion_set_port(o, port);
//
//	signal(SIGINT, free_onion);
//	int error = onion_listen(o);
//	if (error)
//	{
//		perror("Cant create the server");
//	}
//
//	pthread_t uartRxThreadHandler, uartTxThreadHandler;
//	errno = pthread_create(&uartRxThreadHandler, NULL, uartRxThread, NULL);
//	errno = pthread_create(&uartTxThreadHandler, NULL, uartTxThread, NULL);
//
//	pthread_join(uartRxThreadHandler, NULL);
//	pthread_join(uartTxThreadHandler, NULL);
//
//	onion_free(o);

	/* Register the routes. */
	AddRoute(HTTP_GET, "/index.html", IndexHTML);
	AddRoute(HTTP_GET, "/boxer.db",   DB);
	AddRoute(HTTP_GET, "/css/style.css", CSS);
	AddRoute(HTTP_GET, "/css/menu.css", CSS);
	AddRoute(HTTP_GET, "/scripts/scripts.js", JS);
	AddRoute(HTTP_GET, "/scripts/menu.js", JS);
	AddRoute(HTTP_GET, "/scripts/sql.js", JS);
	AddRoute(HTTP_GET, "/scripts/jquery-3.2.1.min.js", JS);

	AddRoute(HTTP_POST, "/SaveData",   SaveData);
	/* Initial the HTTP server and make it listening on MHS_PORT. */
	HTTPServerInit(&srv, 8080);
	/* Run the HTTP server forever. */
	/* Run the dispatch callback if there is a new request */
	HTTPServerRunLoop(&srv, Dispatch);
	HTTPServerClose(&srv);

	return EXIT_SUCCESS;
}

//void * uartRxThread(void * param)
//{
//	while (true)
//	{
//		Uart_RxHandler();
//		sleep(2);
//		printf("UART RX THREAD\r\n");
//	}
//
//	pthread_exit(0);
//}
//
//void * uartTxThread(void * param)
//{
//	while (true)
//	{
//		sleep(2);
//		printf("UART TX THREAD\r\n");
//	}
//
//	pthread_exit(0);
//}
