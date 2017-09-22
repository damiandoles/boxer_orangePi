/*
 * settings.c
 *
 *  Created on: Sep 21, 2017
 *      Author: Damian Dolewski
 */
#include "settings.h"
#include <stdlib.h>

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

void GetDevSettings(dev_settings_t *settings)
{
	(void)settings;
	actualSettings.irrSettings.startTime = (char*)calloc(SHORT_TIME_LEN, sizeof(char*));
	actualSettings.networkSettings.staticIpAddr = (char*)calloc(IP_FORMAT_LEN,	sizeof(char*));
	actualSettings.networkSettings.mask = (char*)calloc(IP_FORMAT_LEN,	sizeof(char*));
	actualSettings.networkSettings.gate = (char*)calloc(IP_FORMAT_LEN,	sizeof(char*));
}

void SetDevSettings(dev_settings_t *settings)
{
	(void)settings;
}
