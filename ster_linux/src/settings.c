/*
 * settings.c
 *
 *  Created on: Sep 21, 2017
 *      Author: Damian Dolewski
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "settings.h"
#include "cJSON.h"

#define JSON_SETTINGS_PATH	"settings/settings.json"

static char * readJsonFile(const char * path);
static void saveJsonFile(const char * path, const char * fileContent);

static const char * defaultJsonContent = "{\n    \"lamp_settings\": {\n        \"time_on\": 12,\n        \"time_off\": 12,\n        \"initial_state\": 0,\n        \"turn_on_off_time\": \"00:00\"\n    },\n    \"temp_fan_settings\": {\n        \"mode\": 1,\n        \"pullfan\": 50,\n        \"pushfan\": 50,\n        \"temp_max\": 25.0\n    },\n    \"irrigation_settings\": {\n        \"mode\": 2,\n        \"water_amount\": 500,\n        \"freqency\": 3,\n        \"start_time\": \"22:00\" \n    },\n    \"network_settings\": {\n        \"dhcp_mode\": 1,\n        \"ipaddr\": \"---.---.---.---\",\n        \"mask\": \"---.---.---.---\",\n        \"gate\": \"---.---.---.---\"\n    }\n}\n";

const dev_settings_t defaultSettings =
{
	{
		.timeOn 		= 12,
		.timeOff 		= 12,
		.state 			= LIGHT_STATE_TURN_OFF,
		.turnOnOffTime 	= "06:00"
	},
	{
		.mode 			= TEMP_FAN_AUTO_MODE,
		.pullFan 		= 50,
		.pushFan 		= 50,
		.tempMax 		= 25.0
	},
	{
		.mode 			= IRR_AUTO_TIMER_MODE,
		.waterAmount 	= 500,
		.freq 			= 3,
		.startTime 		= "00:00"
	},
	{
		.dhcpEnabled 	= DHCP_ON,
		.staticIpAddr 	= "---.---.---.---",
		.mask 			= "---.---.---.---",
		.gate 			= "---.---.---.---"
	}
};

void Settings_GetActual(void)
{
	actualSettings.irrSettings.startTime = (char*)calloc(SHORT_TIME_LEN, sizeof(char*));
	actualSettings.lightSettings.turnOnOffTime = (char*)calloc(SHORT_TIME_LEN, sizeof(char*));
	actualSettings.networkSettings.staticIpAddr = (char*)calloc(IP_FORMAT_LEN,	sizeof(char*));
	actualSettings.networkSettings.mask = (char*)calloc(IP_FORMAT_LEN,	sizeof(char*));
	actualSettings.networkSettings.gate = (char*)calloc(IP_FORMAT_LEN,	sizeof(char*));

	char * jsonStr = readJsonFile(JSON_SETTINGS_PATH);
	cJSON * root = cJSON_Parse(jsonStr);

//	LAMP CONFIG
	cJSON * lamp_settings 	 = cJSON_GetObjectItemCaseSensitive(root, "lamp_settings");
	cJSON * time_on 		 = cJSON_GetObjectItemCaseSensitive(lamp_settings, "time_on");
	cJSON * time_off 		 = cJSON_GetObjectItemCaseSensitive(lamp_settings, "time_off");
	cJSON * initial_state 	 = cJSON_GetObjectItemCaseSensitive(lamp_settings, "initial_state");
	cJSON * turn_on_off_time = cJSON_GetObjectItemCaseSensitive(lamp_settings, "turn_on_off_time");

	actualSettings.lightSettings.state 	 = (light_state_t)initial_state->valueint;
	actualSettings.lightSettings.timeOn  = (hours_num_t)time_on->valueint;
	actualSettings.lightSettings.timeOff = (hours_num_t)time_off->valueint;
	strcpy(actualSettings.lightSettings.turnOnOffTime, turn_on_off_time->valuestring);

//	TEMP/FAN CONFIG
	cJSON * temp_fan_settings = cJSON_GetObjectItemCaseSensitive(root, "temp_fan_settings");
	cJSON * temp_fan_mode 	  = cJSON_GetObjectItemCaseSensitive(temp_fan_settings, "mode");
	cJSON * pullfan 		  = cJSON_GetObjectItemCaseSensitive(temp_fan_settings, "pullfan");
	cJSON * pushfan 		  = cJSON_GetObjectItemCaseSensitive(temp_fan_settings, "pushfan");
	cJSON * temp_max 		  = cJSON_GetObjectItemCaseSensitive(temp_fan_settings, "temp_max");

	actualSettings.tempFanSettings.mode 	= (temp_fan_mode_t)temp_fan_mode->valueint;
	actualSettings.tempFanSettings.pullFan 	= (percent_t)pullfan->valueint;
	actualSettings.tempFanSettings.pushFan 	= (percent_t)pushfan->valueint;
	actualSettings.tempFanSettings.tempMax 	= (float)temp_max->valuedouble;

//	IRRIGATION CONFIG
	cJSON * irrigation_settings = cJSON_GetObjectItemCaseSensitive(root, "irrigation_settings");
	cJSON * irr_mode 			= cJSON_GetObjectItemCaseSensitive(irrigation_settings, "mode");
	cJSON * water_amount 		= cJSON_GetObjectItemCaseSensitive(irrigation_settings, "water_amount");
	cJSON * freqency 			= cJSON_GetObjectItemCaseSensitive(irrigation_settings, "freqency");
	cJSON * start_time 			= cJSON_GetObjectItemCaseSensitive(irrigation_settings, "start_time");

	actualSettings.irrSettings.mode 		= (irr_mode_t)irr_mode->valueint;
	actualSettings.irrSettings.freq 		= (days_t)freqency->valueint;
	actualSettings.irrSettings.waterAmount 	= (ml_t)water_amount->valueint;
	strcpy(actualSettings.irrSettings.startTime, start_time->valuestring);

//	NETWORK CONFIG
	cJSON * network_settings = cJSON_GetObjectItemCaseSensitive(root, "network_settings");
	cJSON * dhcp_mode 		 = cJSON_GetObjectItemCaseSensitive(network_settings, "dhcp_mode");
	cJSON * ipaddr 			 = cJSON_GetObjectItemCaseSensitive(network_settings, "ipaddr");
	cJSON * mask 			 = cJSON_GetObjectItemCaseSensitive(network_settings, "mask");
	cJSON * gate 			 = cJSON_GetObjectItemCaseSensitive(network_settings, "gate");

	actualSettings.networkSettings.dhcpEnabled = (dhcp_mode_t)dhcp_mode->valueint;
	strcpy(actualSettings.networkSettings.staticIpAddr, ipaddr->valuestring);
	strcpy(actualSettings.networkSettings.mask, mask->valuestring);
	strcpy(actualSettings.networkSettings.gate, gate->valuestring);

	cJSON_Delete(root);
	free(jsonStr);
}

void Settings_SetTempFan(void)
{
	char * jsonStr = readJsonFile(JSON_SETTINGS_PATH);
	cJSON * root = cJSON_Parse(jsonStr);

	cJSON * temp_fan_settings = cJSON_GetObjectItemCaseSensitive(root, "temp_fan_settings");
	cJSON * temp_fan_mode 	  = cJSON_GetObjectItemCaseSensitive(temp_fan_settings, "mode");
	cJSON * pullfan 		  = cJSON_GetObjectItemCaseSensitive(temp_fan_settings, "pullfan");
	cJSON * pushfan 		  = cJSON_GetObjectItemCaseSensitive(temp_fan_settings, "pushfan");
	cJSON * temp_max 		  = cJSON_GetObjectItemCaseSensitive(temp_fan_settings, "temp_max");

	cJSON_SetNumberValue(temp_fan_mode, actualSettings.tempFanSettings.mode);
	cJSON_SetNumberValue(pullfan, 		actualSettings.tempFanSettings.pullFan);
	cJSON_SetNumberValue(pushfan, 		actualSettings.tempFanSettings.pushFan);
	cJSON_SetNumberValue(temp_max, 		actualSettings.tempFanSettings.tempMax);

	char *newJsonContent = cJSON_Print(root);
	saveJsonFile(JSON_SETTINGS_PATH, newJsonContent);
	cJSON_Delete(root);
	free(jsonStr);
}

void Settings_SetLamp(void)
{
	char * jsonStr = readJsonFile(JSON_SETTINGS_PATH);
	cJSON * root = cJSON_Parse(jsonStr);

	cJSON * lamp_settings 	 = cJSON_GetObjectItemCaseSensitive(root, "lamp_settings");
	cJSON * time_on 		 = cJSON_GetObjectItemCaseSensitive(lamp_settings, "time_on");
	cJSON * time_off 		 = cJSON_GetObjectItemCaseSensitive(lamp_settings, "time_off");
	cJSON * initial_state 	 = cJSON_GetObjectItemCaseSensitive(lamp_settings, "initial_state");
	cJSON * turn_on_off_time = cJSON_GetObjectItemCaseSensitive(lamp_settings, "turn_on_off_time");

	cJSON_SetNumberValue(time_on, 			actualSettings.lightSettings.timeOn);
	cJSON_SetNumberValue(time_off, 			actualSettings.lightSettings.timeOff);
	cJSON_SetNumberValue(initial_state, 	actualSettings.lightSettings.state);
	strcpy(turn_on_off_time->valuestring, 	actualSettings.lightSettings.turnOnOffTime);

	char *newJsonContent = cJSON_Print(root);
	saveJsonFile(JSON_SETTINGS_PATH, newJsonContent);
	cJSON_Delete(root);
	free(jsonStr);
}

void Settings_SetIrr(void)
{
	char * jsonStr = readJsonFile(JSON_SETTINGS_PATH);
	cJSON * root = cJSON_Parse(jsonStr);

	cJSON * irrigation_settings = cJSON_GetObjectItemCaseSensitive(root, "irrigation_settings");
	cJSON * irr_mode 			= cJSON_GetObjectItemCaseSensitive(irrigation_settings, "mode");
	cJSON * water_amount 		= cJSON_GetObjectItemCaseSensitive(irrigation_settings, "water_amount");
	cJSON * freqency 			= cJSON_GetObjectItemCaseSensitive(irrigation_settings, "freqency");
	cJSON * start_time 			= cJSON_GetObjectItemCaseSensitive(irrigation_settings, "start_time");

	cJSON_SetNumberValue(irr_mode, 		actualSettings.irrSettings.mode);
	cJSON_SetNumberValue(water_amount, 	actualSettings.irrSettings.waterAmount);
	cJSON_SetNumberValue(freqency, 		actualSettings.irrSettings.freq);
	strcpy(start_time->valuestring, 	actualSettings.irrSettings.startTime);

	char *newJsonContent = cJSON_Print(root);
	saveJsonFile(JSON_SETTINGS_PATH, newJsonContent);
	cJSON_Delete(root);
	free(jsonStr);
}

void Settings_SetNetwork(void)
{
	char * jsonStr = readJsonFile(JSON_SETTINGS_PATH);
	cJSON * root = cJSON_Parse(jsonStr);

	cJSON * network_settings	= cJSON_GetObjectItemCaseSensitive(root, "network_settings");
	cJSON * dhcp_mode 			= cJSON_GetObjectItemCaseSensitive(network_settings, "dhcp_mode");
	cJSON * ipaddr 				= cJSON_GetObjectItemCaseSensitive(network_settings, "ipaddr");
	cJSON * mask 				= cJSON_GetObjectItemCaseSensitive(network_settings, "mask");
	cJSON * gate 				= cJSON_GetObjectItemCaseSensitive(network_settings, "gate");

	cJSON_SetNumberValue(dhcp_mode, actualSettings.networkSettings.dhcpEnabled);
	strcpy(ipaddr->valuestring, 	actualSettings.networkSettings.staticIpAddr);
	strcpy(mask->valuestring, 		actualSettings.networkSettings.mask);
	strcpy(gate->valuestring, 		actualSettings.networkSettings.gate);

	char *newJsonContent = cJSON_Print(root);
	saveJsonFile(JSON_SETTINGS_PATH, newJsonContent);
	cJSON_Delete(root);
	free(jsonStr);
}

void Settings_SetDefault(void)
{
	memcpy(&actualSettings, &defaultSettings, sizeof(dev_settings_t));
	saveJsonFile(JSON_SETTINGS_PATH, defaultJsonContent);
}

static void saveJsonFile(const char * path, const char * fileContent)
{
	FILE *fp;
	fp = fopen(path, "wb");
	fwrite(fileContent, sizeof(fileContent[0]), strlen(fileContent), fp);
	fclose(fp);
}

static char * readJsonFile(const char * path)
{
	FILE *f = NULL;
	long len = 0;
	char *data = NULL;

	f = fopen(path,"rb"); //binary mode
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);

	data = (char*)malloc(len + 1);

	fread(data, 1, len, f);
	data[len] = '\0';
	fclose(f);

	return data;
}
