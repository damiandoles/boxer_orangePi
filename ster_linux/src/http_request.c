/*
 * http_reques.c
 *
 *  Created on: Sep 6, 2017
 *      Author: doles
 */
#include "http_request.h"
#include <stdio.h>
#include <string.h>

static void SaveData(HTTPReqMessage *req, HTTPResMessage *res);
static void JS(HTTPReqMessage *req, HTTPResMessage *res);
static void CSS(HTTPReqMessage *req, HTTPResMessage *res);
static void DB(HTTPReqMessage *req, HTTPResMessage *res);
static void IndexHTML(HTTPReqMessage *req, HTTPResMessage *res);

void Http_RegisterHandlers(void)
{
	AddRouteHandler(HTTP_GET, "/index.html", IndexHTML);
	AddRouteHandler(HTTP_GET, "/boxer.db",   DB);
	AddRouteHandler(HTTP_GET, "/css/style.css", CSS);
	AddRouteHandler(HTTP_GET, "/css/menu.css", CSS);
	AddRouteHandler(HTTP_GET, "/scripts/scripts.js", JS);
	AddRouteHandler(HTTP_GET, "/scripts/menu.js", JS);
	AddRouteHandler(HTTP_GET, "/scripts/sql.js", JS);
	AddRouteHandler(HTTP_GET, "/scripts/jquery-3.2.1.min.js", JS);
	AddRouteHandler(HTTP_POST, "/SaveData",   SaveData);
}

static void IndexHTML(HTTPReqMessage *req, HTTPResMessage *res)
{
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

static void DB(HTTPReqMessage *req, HTTPResMessage *res)
{
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

static void CSS(HTTPReqMessage *req, HTTPResMessage *res)
{
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

static void JS(HTTPReqMessage *req, HTTPResMessage *res)
{
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

static void SaveData(HTTPReqMessage *req, HTTPResMessage *res)
{

	char * data;
	size_t dataSize = strlen((const char*)req->Body);
	if (dataSize > 0)
	{
		data = (char*)calloc(dataSize, sizeof(char));
		strcpy(data, (const char *)req->Body);
		printf("SaveData POST request: %s\r\n", data);
	}
}
