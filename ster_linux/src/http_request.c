/*
 * http_request.c
 *
 *  Created on: Sep 6, 2017
 *      Author: doles
 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "server_middleware.h"
#include "http_request.h"
#include "database.h"
#include "settings.h"

int GetLamp(struct mg_connection *conn, void *cbdata);
int GetMeas(struct mg_connection *conn, void *cbdata);
int GetTempFan(struct mg_connection *conn, void *cbdata);
int GetAdvanced(struct mg_connection *conn, void *cbdata);
int GetIrr(struct mg_connection *conn, void *cbdata);

int SaveLamp(struct mg_connection *conn, void *cbdata);
int SaveTempFan(struct mg_connection *conn, void *cbdata);
int SaveCalibPh(struct mg_connection *conn, void *cbdata);
int SaveIrr(struct mg_connection *conn, void *cbdata);
int SaveNetwork(struct mg_connection *conn, void *cbdata);

int Reset(struct mg_connection *conn, void *cbdata);
int FactoryDef(struct mg_connection *conn, void *cbdata);

#define MAX_RESP_BODY_LEN 		512

void HttpReq_RegisterUserHandlers(struct mg_context *ctx)
{
	mg_set_request_handler(ctx, "/GetLamp", 	GetLamp, 	 NULL);
	mg_set_request_handler(ctx, "/GetMeas", 	GetMeas, 	 NULL);
	mg_set_request_handler(ctx, "/GetTempFan", 	GetTempFan,  NULL);
	mg_set_request_handler(ctx, "/GetAdvanced", GetAdvanced, NULL);
	mg_set_request_handler(ctx, "/GetIrr", 		GetIrr, 	 NULL);

	mg_set_request_handler(ctx, "/SaveLamp", 	SaveLamp,	 NULL);
	mg_set_request_handler(ctx, "/SaveTempFan", SaveTempFan, NULL);
	mg_set_request_handler(ctx, "/SaveCalibPh", SaveCalibPh, NULL);
	mg_set_request_handler(ctx, "/SaveIrr", 	SaveIrr, 	 NULL);
	mg_set_request_handler(ctx, "/SaveNetwork", SaveNetwork, NULL);

	mg_set_request_handler(ctx, "/Reset", 		Reset, 		 NULL);
	mg_set_request_handler(ctx, "/FactoryDef", 	FactoryDef,  NULL);
}

int GetLamp(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	printf("GetLamp POST request\r\n");

	char * respBody = (char*)calloc(MAX_RESP_BODY_LEN, sizeof(char));
	snprintf(
			respBody,
			MAX_RESP_BODY_LEN,
			"%d\r\n%d\r\n%d\r\n%s\r\n0\r\n0\r\n", //+ counters
			defSettings.lightSettings.timeOn,
			defSettings.lightSettings.timeOff,
			defSettings.lightSettings.initState,
			defSettings.lightSettings.turnOnOffTime);

	mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
			"Connection: close\r\n" \
			"Content-Type: text/plain; charset=UTF-8\r\n" \
			"Content-Lenght: %i\r\n\r\n %s",
			(int)strlen(respBody), respBody);
	free(respBody);
	return 1;
}

int GetMeas(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	printf("GetMeas POST request\r\n");
	basic_meas_t m_basic;
	ph_meas_t m_ph;

	DataBase_SelectMeasData(&m_basic, &m_ph);
	char * respBody = (char*)calloc(MAX_RESP_BODY_LEN, sizeof(char));
	snprintf(
			respBody,
			MAX_RESP_BODY_LEN,
			"%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
			m_basic.humidity,
			m_basic.lux,
			m_basic.temp_up,
			m_basic.temp_middle,
			m_basic.temp_down,
			m_ph.ph_water,
			m_ph.ph_soil,
			m_basic.soil_moist);

	mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
			"Connection: close\r\n" \
			"Content-Type: text/plain; charset=UTF-8\r\n" \
			"Content-Lenght: %i\r\n\r\n %s",
			(int)strlen(respBody), respBody);

	free(respBody);
	return 1;
}

int GetTempFan(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	printf("GetTempFan POST request\r\n");
	char * respBody = (char*)calloc(MAX_RESP_BODY_LEN, sizeof(char));
	snprintf(
			respBody,
			MAX_RESP_BODY_LEN,
			"%d\r\n%d\r\n%d\r\n%.1f\r\n",
			defSettings.tempFanSettings.mode,
			defSettings.tempFanSettings.pullFan,
			defSettings.tempFanSettings.pushFan,
			defSettings.tempFanSettings.tempMax);

	mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
			"Connection: close\r\n" \
			"Content-Type: text/plain; charset=UTF-8\r\n" \
			"Content-Lenght: %i\r\n\r\n %s",
			(int)strlen(respBody), respBody);
	free(respBody);
	return 1;
}

int GetAdvanced(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	printf("GetAdvanced POST request\r\n");
	char * respBody = (char*)calloc(MAX_RESP_BODY_LEN, sizeof(char));
	snprintf(
			respBody,
			MAX_RESP_BODY_LEN,
			"%d\r\n%s\r\n%s\r\n%s\r\n",
			defSettings.networkSettings.dhcpEnabled,
			defSettings.networkSettings.ipAddr,
			defSettings.networkSettings.mask,
			defSettings.networkSettings.gate);

	mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
			"Connection: close\r\n" \
			"Content-Type: text/plain; charset=UTF-8\r\n" \
			"Content-Lenght: %i\r\n\r\n %s",
			(int)strlen(respBody), respBody);
	free(respBody);
	return 1;
}

int GetIrr(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	printf("GetIrr POST request\r\n");
	char * respBody = (char*)calloc(MAX_RESP_BODY_LEN, sizeof(char));
	snprintf(
			respBody,
			MAX_RESP_BODY_LEN,
			"%d\r\n%d\r\n%d\r\n%s\r\n",
			defSettings.irrSettings.mode,
			defSettings.irrSettings.waterAmount,
			defSettings.irrSettings.freq,
			defSettings.irrSettings.startTime);

	mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
			"Connection: close\r\n" \
			"Content-Type: text/plain; charset=UTF-8\r\n" \
			"Content-Lenght: %i\r\n\r\n %s",
			(int)strlen(respBody), respBody);
	free(respBody);
	return 1;
}

int SaveLamp(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
//	char * data;
//	size_t dataSize = strlen((const char*)req->Body);
//	if (dataSize > 0)
//	{
//		data = (char*)calloc(dataSize, sizeof(char));
//		strcpy(data, (const char *)req->Body);
//		printf("SaveLamp POST request: %s\r\n", data);
//		memcpy(res->_buf, Code200_appxwww, CODE200_APP_WWW_LEN);
//		res->_index = CODE200_APP_WWW_LEN;
//
//		free(data);
//	}
	//TODO save lamp settings to the JSON file
	return 1;
}

int SaveTempFan(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
//	char * data;
//	size_t dataSize = strlen((const char*)req->Body);
//	if (dataSize > 0)
//	{
//		data = (char*)calloc(dataSize, sizeof(char));
//		strcpy(data, (const char *)req->Body);
//		printf("SaveTempFan POST request: %s\r\n", data);
//		memcpy(res->_buf, Code200_appxwww, CODE200_APP_WWW_LEN);
//		res->_index = CODE200_APP_WWW_LEN;
//
//		free(data);
//	}
	//TODO save temp/fan settings to the JSON file
	return 1;
}

int SaveCalibPh(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
//	char * data;
//	size_t dataSize = strlen((const char*)req->Body);
//	if (dataSize > 0)
//	{
//		data = (char*)calloc(dataSize, sizeof(char));
//		strcpy(data, (const char *)req->Body);
//		printf("SaveCalibPh POST request: %s\r\n", data);
//		memcpy(res->_buf, Code200_appxwww, CODE200_APP_WWW_LEN);
//		res->_index = CODE200_APP_WWW_LEN;
//
//		free(data);
//	}
	//TODO start calibration, calculate ph equation factors and save it to the JSON file
	return 1;
}

int SaveIrr(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
//	char * data;
//	size_t dataSize = strlen((const char*)req->Body);
//	if (dataSize > 0)
//	{
//		data = (char*)calloc(dataSize, sizeof(char));
//		strcpy(data, (const char *)req->Body);
//		printf("SaveIrr POST request: %s\r\n", data);
//		memcpy(res->_buf, Code200_appxwww, CODE200_APP_WWW_LEN);
//		res->_index = CODE200_APP_WWW_LEN;
//
//		free(data);
//	}
	//TODO save irrigation settings to the JSON file
	return 1;
}

int SaveNetwork(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
//	char * data;
//	size_t dataSize = strlen((const char*)req->Body);
//	if (dataSize > 0)
//	{
//		data = (char*)calloc(dataSize, sizeof(char));
//		strcpy(data, (const char *)req->Body);
//		printf("SaveNetwork POST request: %s\r\n", data);
//		memcpy(res->_buf, Code200_appxwww, CODE200_APP_WWW_LEN);
//		res->_index = CODE200_APP_WWW_LEN;
//
//		free(data);
//	}
	//TODO save network settings to the JSON file
	return 1;
}

int Reset(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	printf("Reset HEAD request\r\n");
	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\n"
	          "Content-Type: text/plain; charset=UTF-8\r\n"
	          "Connection: close\r\n\r\n");

	//TODO device reset (linux restart)
	return 1;
}

int FactoryDef(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	printf("FactoryDef HEAD request\r\n");
	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\n"
	          "Content-Type: text/plain; charset=UTF-8\r\n"
	          "Connection: close\r\n\r\n");

	//TODO restore factory settings (JSON file)
	return 1;
}
