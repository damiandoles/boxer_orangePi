/*
 * settings.h
 *
 *  Created on: Sep 21, 2017
 *      Author: Damian Dolewski
 */

#ifndef INC_SETTINGS_H_
#define INC_SETTINGS_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct LightSettings
{
	uint8_t timeOn;
	uint8_t timeOff;
	bool initState;
	char * turnOnOffTime;
}light_sett_t;

typedef struct TempFanSettings
{
	uint8_t mode;
	uint8_t pullFan;
	uint8_t pushFan;
	float tempMax;
}temp_fan_sett_t;

typedef struct IrrigationSettings
{
	uint8_t mode;
	uint16_t waterAmount;
	uint8_t freq;
	char * startTime;
}irr_sett_t;

typedef struct NetworkSettings
{
	bool dhcpEnabled;
	char * ipAddr;
	char * mask;
	char * gate;
}network_sett_t;

typedef struct DeviceSettings
{
	light_sett_t lightSettings;
	temp_fan_sett_t tempFanSettings;
	irr_sett_t irrSettings;
	network_sett_t networkSettings;
}dev_settings_t;

const dev_settings_t defSettings =
{
	{
		.timeOn 		= 12,
		.timeOff 		= 12,
		.initState 		= false,
		.turnOnOffTime 	= "06:00"
	},
	{
		.mode 			= 1,
		.pullFan 		= 50,
		.pushFan 		= 50,
		.tempMax 		= 26.0
	},
	{
		.mode 			= 2,
		.waterAmount 	= 500,
		.freq 			= 3,
		.startTime 		= "00:00"
	},
	{
		.dhcpEnabled 	= true,
		.ipAddr 		= "---.---.---.---",
		.mask 			= "---.---.---.---",
		.gate 			= "---.---.---.---"
	}
};

#endif /* INC_SETTINGS_H_ */
