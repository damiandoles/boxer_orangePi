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
#include <string.h>

#include "server_middleware.h"
#include "http_request.h"
#include "database.h"
#include "settings.h"

#define MAX_RESP_BODY_LEN 			512

#define MAX_POST_FORM_PARAMS		16
#define MAX_POST_PORM_PARAMS_LEN	8

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

static char ** Http_ExtractPostForm(char *str, int paramsNum);

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
			"%d\r\n%d\r\n%d\r\n%d\r\n%s\r\n0\r\n0\r\n", //+ counters
			actualSettings.lightSettings.timeOn,
			actualSettings.lightSettings.timeOff,
			actualSettings.lightSettings.initState,
			actualSettings.lightSettings.startMode,
			actualSettings.lightSettings.turnOnOffTime);

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

	size_t lenAll = strlen(m_basic.humidity) 	+
					strlen(m_basic.lux) 		+
					strlen(m_basic.temp_up) 	+
					strlen(m_basic.temp_middle) +
					strlen(m_basic.temp_down) 	+
					strlen(m_ph.ph_water) 		+
					strlen(m_ph.ph_soil) 		+
					strlen(m_basic.soil_moist);

	if (!lenAll)
	{
		snprintf(
				respBody,
				MAX_RESP_BODY_LEN,
				"- - -\r\n- - -\r\n- - -\r\n- - -\r\n- - -\r\n- - -\r\n- - -\r\n- - -");
	}
	else
	{
		snprintf(
				respBody,
				MAX_RESP_BODY_LEN,
				"%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s",
				m_basic.humidity,
				m_basic.lux,
				m_basic.temp_up,
				m_basic.temp_middle,
				m_basic.temp_down,
				m_ph.ph_water,
				m_ph.ph_soil,
				m_basic.soil_moist);
	}

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
			actualSettings.tempFanSettings.mode,
			actualSettings.tempFanSettings.pullFan,
			actualSettings.tempFanSettings.pushFan,
			actualSettings.tempFanSettings.tempMax);

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
			actualSettings.networkSettings.dhcpEnabled,
			actualSettings.networkSettings.staticIpAddr,
			actualSettings.networkSettings.mask,
			actualSettings.networkSettings.gate);

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
			actualSettings.irrSettings.mode,
			actualSettings.irrSettings.waterAmount,
			actualSettings.irrSettings.freq,
			actualSettings.irrSettings.startTime);

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
	char contentBuff[128] = {0};
	int len = mg_read(conn, contentBuff, sizeof(contentBuff)/sizeof(*(contentBuff)));
	if (len > 0)
	{
		char ** values = Http_ExtractPostForm(contentBuff, 5);

		actualSettings.lightSettings.timeOn 	= atoi(values[0]);
		actualSettings.lightSettings.timeOff 	= atoi(values[1]);
		actualSettings.lightSettings.initState 	= atoi(values[2]);
		actualSettings.lightSettings.startMode 	= atoi(values[3]);
		strcpy(actualSettings.lightSettings.turnOnOffTime, values[4]);

		Settings_SetLamp();

		mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
				"Connection: close\r\n" \
				"Content-Type: application/x-www-form-urlencoded\r\n\r\n");
	}

	return 1;
}

int SaveTempFan(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	char contentBuff[128] = {0};
	int len = mg_read(conn, contentBuff, sizeof(contentBuff)/sizeof(*(contentBuff)));
	if (len > 0)
	{
		char ** values = Http_ExtractPostForm(contentBuff, 4);

		actualSettings.tempFanSettings.mode 	= atoi(values[0]);
		actualSettings.tempFanSettings.tempMax 	= strtod(values[1], NULL);
		actualSettings.tempFanSettings.pushFan 	= atoi(values[2]);
		actualSettings.tempFanSettings.pullFan 	= atoi(values[3]);

		Settings_SetTempFan();

		mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
				"Connection: close\r\n" \
				"Content-Type: application/x-www-form-urlencoded\r\n\r\n");
	}

	return 1;
}

int SaveCalibPh(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	char contentBuff[128] = {0};
	int len = mg_read(conn, contentBuff, sizeof(contentBuff)/sizeof(*(contentBuff)));
	if (len > 0)
	{
		char * splitStr = strtok(contentBuff, "=");
		splitStr = strtok(NULL, "=");

		calib_probe_t probeType = CALIB_PH_NONE_PROBE;
		probeType = (calib_probe_t)splitStr;

		switch (probeType)
		{
		case CALIB_PH_WATER_PROBE:
			break;

		case CALIB_PH_SOIL_PROBE:
			break;

		default:
			break;
		}

		mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
				"Connection: close\r\n" \
				"Content-Type: application/x-www-form-urlencoded\r\n\r\n");
	}

	//TODO Send calibration command to ph module
	return 1;
}

int SaveIrr(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	char contentBuff[128] = {0};
	int len = mg_read(conn, contentBuff, sizeof(contentBuff)/sizeof(*(contentBuff)));
	if (len > 0)
	{
		char ** values = Http_ExtractPostForm(contentBuff, 4);

		actualSettings.irrSettings.mode 		= atoi(values[0]);
		actualSettings.irrSettings.waterAmount 	= atoi(values[1]);
		actualSettings.irrSettings.freq 		= atoi(values[2]);
		strcpy(actualSettings.irrSettings.startTime, values[3]);

		Settings_SetIrr();

		mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
				"Connection: close\r\n" \
				"Content-Type: application/x-www-form-urlencoded\r\n\r\n");
	}

	return 1;
}

int SaveNetwork(struct mg_connection *conn, void *cbdata)
{
	(void)cbdata;
	char contentBuff[128] = {0};
	int len = mg_read(conn, contentBuff, sizeof(contentBuff)/sizeof(*(contentBuff)));
	if (len > 0)
	{
		char ** values = Http_ExtractPostForm(contentBuff, 4);

		actualSettings.networkSettings.dhcpEnabled = atoi(values[0]);

		if (actualSettings.networkSettings.dhcpEnabled == DHCP_OFF)
		{
			strcpy(actualSettings.networkSettings.staticIpAddr, values[1]);
			strcpy(actualSettings.networkSettings.mask, values[2]);
			strcpy(actualSettings.networkSettings.gate, values[3]);
		}
		else
		{
			memset(actualSettings.networkSettings.staticIpAddr, 0, IP_FORMAT_LEN);
			memset(actualSettings.networkSettings.mask, 0, IP_FORMAT_LEN);
			memset(actualSettings.networkSettings.gate, 0, IP_FORMAT_LEN);
		}

		Settings_SetNetwork();

		mg_printf(conn, "HTTP/1.1 200 OK\r\n" \
				"Connection: close\r\n" \
				"Content-Type: application/x-www-form-urlencoded\r\n\r\n");
	}

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

	memcpy(&actualSettings, &defaultSettings, sizeof(dev_settings_t));
	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\n"
	          "Content-Type: text/plain; charset=UTF-8\r\n"
	          "Connection: close\r\n\r\n");

	Settings_SetDefault();
	return 1;
}

static char ** Http_ExtractPostForm(char *str, int paramsNum)
{
    char **ret = (char **)malloc(MAX_POST_FORM_PARAMS * sizeof(char *)); //rows
    int row;

    for (row = 0; row < 16; row++)
    {
        ret[row] = (char *)malloc(MAX_POST_PORM_PARAMS_LEN * sizeof(char)); //columns
    }

	char * splitStr = 0;
	int i = 0;
	splitStr = strtok (str, "&");
	strcpy(ret[i], splitStr);

	while (splitStr != NULL)
	{
		if (i < 16 - 1)
		{
			i++;
		}
		else
		{
			break;
		}

		splitStr = strtok (NULL, "&");
		if (splitStr != 0x0)
		{
			strcpy(ret[i], splitStr);
		}
		else
		{
			break;
		}
	}

	for (i = 0; i < paramsNum; i++)
	{
		splitStr = strtok(ret[i], "=");
		if (splitStr != NULL)
		{
			splitStr = strtok(NULL, "=");
			strcpy(ret[i], splitStr);
		}

	}

	return &(*ret);
}
