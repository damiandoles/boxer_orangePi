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

#define IP_FORMAT_LEN	15
#define SHORT_TIME_LEN	5

typedef uint8_t 		hours_num_t;
typedef uint16_t 		ml_t;
typedef uint8_t 		percent_t;
typedef uint8_t			days_t;

typedef enum LightInitState
{
	LIGHT_STATE_TURN_OFF,
	LIGHT_STATE_TURN_ON
}light_state_t;

typedef enum TempFanMode
{
	TEMP_FAN_MANUAL_MODE,
	TEMP_FAN_AUTO_MODE
}temp_fan_mode_t;

typedef enum IrrMode
{
	IRR_MANUAL_MODE,
	IRR_AUTO_SOIL_MOISTURE_MODE,
	IRR_AUTO_TIMER_MODE
}irr_mode_t;

typedef enum DhcpMode
{
	DHCP_OFF,
	DHCP_ON
}dhcp_mode_t;

typedef enum PhCalib
{
	CALIB_PH_NONE_PROBE,
	CALIB_PH_WATER_PROBE,
	CALIB_PH_SOIL_PROBE
}calib_probe_t;

typedef struct LightSettings
{
	hours_num_t timeOn;
	hours_num_t timeOff;
	light_state_t state;
	char * turnOnOffTime;
}light_sett_t;

typedef struct TempFanSettings
{
	temp_fan_mode_t mode;
	percent_t pullFan;
	percent_t pushFan;
	float tempMax;
}temp_fan_sett_t;

typedef struct IrrigationSettings
{
	irr_mode_t mode;
	ml_t waterAmount;
	days_t freq;
	char * startTime;
}irr_sett_t;

typedef struct NetworkSettings
{
	dhcp_mode_t dhcpEnabled;
	char * staticIpAddr;
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

extern const dev_settings_t defaultSettings;
dev_settings_t actualSettings;

void GetDevSettings(dev_settings_t *settings);
void SetDevSettings(dev_settings_t *settings);

#endif /* INC_SETTINGS_H_ */
