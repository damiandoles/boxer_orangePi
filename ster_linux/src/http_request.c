/*
 * http_request.c
 *
 *  Created on: Sep 6, 2017
 *      Author: doles
 */
#include "http_request.h"
#include <stdio.h>
#include <string.h>

static void SaveLamp(HTTPReqMessage *req, HTTPResMessage *res);
static void SaveTempFan(HTTPReqMessage *req, HTTPResMessage *res);
static void SaveCalibPh(HTTPReqMessage *req, HTTPResMessage *res);
static void SaveIrr(HTTPReqMessage *req, HTTPResMessage *res);
static void SaveNetwork(HTTPReqMessage *req, HTTPResMessage *res);
static void Reset(HTTPReqMessage *req, HTTPResMessage *res);
static void FactoryDef(HTTPReqMessage *req, HTTPResMessage *res);

static const char PostResp[] = "HTTP/1.1 200 OK\r\n" \
		"Connection: close\r\n" \
		"Content-Type: application/x-www-form-urlencoded; " \
		"charset=UTF-8\r\n\r\n";

static const char HeadResp[] = "HTTP/1.1 200 OK\r\n" \
		"Connection: close\r\n" \
		"Content-Type: text/plain; " \
		"charset=UTF-8\r\n\r\n";

#define POST_RESP_LEN strlen(PostResp)
#define HEAD_RESP_LEN strlen(HeadResp)

void HttpMidd_RegisterUserHandlers(void)
{
	AddRouteHandler(HTTP_POST, "/SaveLamp",   	SaveLamp);
	AddRouteHandler(HTTP_POST, "/SaveTempFan",  SaveTempFan);
	AddRouteHandler(HTTP_POST, "/SaveCalibPh",  SaveCalibPh);
	AddRouteHandler(HTTP_POST, "/SaveIrr",   	SaveIrr);
	AddRouteHandler(HTTP_POST, "/SaveNetwork",  SaveNetwork);
	AddRouteHandler(HTTP_HEAD, "/Reset",   		Reset);
	AddRouteHandler(HTTP_HEAD, "/FactoryDef",   FactoryDef);
}

static void SaveLamp(HTTPReqMessage *req, HTTPResMessage *res)
{
	char * data;
	size_t dataSize = strlen((const char*)req->Body);
	if (dataSize > 0)
	{
		data = (char*)calloc(dataSize, sizeof(char));
		strcpy(data, (const char *)req->Body);
		printf("SaveLamp POST request: %s\r\n", data);
		memcpy(res->_buf, PostResp, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;
	}
}

static void SaveTempFan(HTTPReqMessage *req, HTTPResMessage *res)
{
	char * data;
	size_t dataSize = strlen((const char*)req->Body);
	if (dataSize > 0)
	{
		data = (char*)calloc(dataSize, sizeof(char));
		strcpy(data, (const char *)req->Body);
		printf("SaveTempFan POST request: %s\r\n", data);
		memcpy(res->_buf, PostResp, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;
	}
}

static void SaveCalibPh(HTTPReqMessage *req, HTTPResMessage *res)
{
	char * data;
	size_t dataSize = strlen((const char*)req->Body);
	if (dataSize > 0)
	{
		data = (char*)calloc(dataSize, sizeof(char));
		strcpy(data, (const char *)req->Body);
		printf("SaveCalibPh POST request: %s\r\n", data);
		memcpy(res->_buf, PostResp, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;
	}
}

static void SaveIrr(HTTPReqMessage *req, HTTPResMessage *res)
{
	char * data;
	size_t dataSize = strlen((const char*)req->Body);
	if (dataSize > 0)
	{
		data = (char*)calloc(dataSize, sizeof(char));
		strcpy(data, (const char *)req->Body);
		printf("SaveIrr POST request: %s\r\n", data);
		memcpy(res->_buf, PostResp, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;
	}
}

static void SaveNetwork(HTTPReqMessage *req, HTTPResMessage *res)
{
	char * data;
	size_t dataSize = strlen((const char*)req->Body);
	if (dataSize > 0)
	{
		data = (char*)calloc(dataSize, sizeof(char));
		strcpy(data, (const char *)req->Body);
		printf("SaveNetwork POST request: %s\r\n", data);
		memcpy(res->_buf, PostResp, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;
	}
}

static void Reset(HTTPReqMessage *req, HTTPResMessage *res)
{
	printf("Reset HEAD request\r\n");
	memcpy(res->_buf, HeadResp, HEAD_RESP_LEN);
	res->_index = HEAD_RESP_LEN;
}

static void FactoryDef(HTTPReqMessage *req, HTTPResMessage *res)
{
	printf("FactoryDef HEAD request\r\n");
	memcpy(res->_buf, HeadResp, HEAD_RESP_LEN);
	res->_index = HEAD_RESP_LEN;
}
