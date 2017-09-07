/*
 * http_request.c
 *
 *  Created on: Sep 6, 2017
 *      Author: doles
 */
#include "http_request.h"
#include "database.h"
#include <stdio.h>
#include <string.h>

static void GetLamp(HTTPReqMessage *req, HTTPResMessage *res);
static void GetMeas(HTTPReqMessage *req, HTTPResMessage *res);
static void GetTempFan(HTTPReqMessage *req, HTTPResMessage *res);
static void GetAdvanced(HTTPReqMessage *req, HTTPResMessage *res);
static void GetIrr(HTTPReqMessage *req, HTTPResMessage *res);

static void SaveLamp(HTTPReqMessage *req, HTTPResMessage *res);
static void SaveTempFan(HTTPReqMessage *req, HTTPResMessage *res);
static void SaveCalibPh(HTTPReqMessage *req, HTTPResMessage *res);
static void SaveIrr(HTTPReqMessage *req, HTTPResMessage *res);
static void SaveNetwork(HTTPReqMessage *req, HTTPResMessage *res);

static void Reset(HTTPReqMessage *req, HTTPResMessage *res);
static void FactoryDef(HTTPReqMessage *req, HTTPResMessage *res);

static const char PostResp_Code200[] = "HTTP/1.1 200 OK\r\n" \
		"Connection: close\r\n" \
		"Content-Type: application/x-www-form-urlencoded; " \
		"charset=UTF-8\r\n\r\n";

static const char PostResp_Code201[] = "HTTP/1.1 201 OK\r\n" \
		"Connection: close\r\n" \
		"Content-Type: text/plain; " \
		"charset=UTF-8\r\n\r\n";

static const char HeadResp[] = "HTTP/1.1 200 OK\r\n" \
		"Connection: close\r\n" \
		"Content-Type: text/plain; " \
		"charset=UTF-8\r\n\r\n";

#define POST_RESP_LEN 		strlen(PostResp_Code200)
#define HEAD_RESP_LEN 		strlen(HeadResp)
#define MAX_RESP_BODY_LEN 	128

void HttpReq_RegisterUserHandlers(void)
{
	AddRouteHandler(HTTP_POST, "/GetLamp",   	GetLamp);
	AddRouteHandler(HTTP_POST, "/GetMeas",   	GetMeas);
	AddRouteHandler(HTTP_POST, "/GetTempFan",   GetTempFan);
	AddRouteHandler(HTTP_POST, "/GetAdvanced",  GetAdvanced);
	AddRouteHandler(HTTP_POST, "/GetIrr",   	GetIrr);

	AddRouteHandler(HTTP_POST, "/SaveLamp",  	SaveLamp);
	AddRouteHandler(HTTP_POST, "/SaveTempFan",  SaveTempFan);
	AddRouteHandler(HTTP_POST, "/SaveCalibPh",  SaveCalibPh);
	AddRouteHandler(HTTP_POST, "/SaveIrr",   	SaveIrr);
	AddRouteHandler(HTTP_POST, "/SaveNetwork",  SaveNetwork);

	AddRouteHandler(HTTP_HEAD, "/Reset",   		Reset);
	AddRouteHandler(HTTP_HEAD, "/FactoryDef",   FactoryDef);
}

static void GetLamp(HTTPReqMessage *req, HTTPResMessage *res)
{
	printf("GetLamp POST request\r\n");
	memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
	res->_index = POST_RESP_LEN;
}

static void GetMeas(HTTPReqMessage *req, HTTPResMessage *res)
{
	basic_meas_t m_basic;
	ph_meas_t m_ph;

	DataBase_SelectMeasData(&m_basic, &m_ph);

	printf("GetMeas POST request\r\n");
	memcpy(res->_buf, PostResp_Code201, POST_RESP_LEN);

	char * respBody = (char*)calloc(MAX_RESP_BODY_LEN, sizeof(char));
	snprintf(respBody,
			MAX_RESP_BODY_LEN,
			"humidity=%s 		\
			&lux=%s 			\
			&temp_up=%s 		\
			&temp_middle=%s 	\
			&temp_down=%s 		\
			&soil_moist=%s"
			,
			m_basic.humidity,
			m_basic.lux,
			m_basic.temp_up,
			m_basic.temp_middle,
			m_basic.temp_down,
			m_basic.soil_moist);

	size_t respBodyLen = strlen(respBody);
	res->_index = POST_RESP_LEN + respBodyLen;
}

static void GetTempFan(HTTPReqMessage *req, HTTPResMessage *res)
{
	printf("GetTempFan POST request\r\n");
	memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
	res->_index = POST_RESP_LEN;
}

static void GetAdvanced(HTTPReqMessage *req, HTTPResMessage *res)
{
	printf("GetAdvanced POST request\r\n");
	memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
	res->_index = POST_RESP_LEN;
}

static void GetIrr(HTTPReqMessage *req, HTTPResMessage *res)
{
	printf("GetIrr POST request\r\n");
	memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
	res->_index = POST_RESP_LEN;
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
		memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;

		free(data);
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
		memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;

		free(data);
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
		memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;

		free(data);
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
		memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;

		free(data);
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
		memcpy(res->_buf, PostResp_Code200, POST_RESP_LEN);
		res->_index = POST_RESP_LEN;

		free(data);
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
