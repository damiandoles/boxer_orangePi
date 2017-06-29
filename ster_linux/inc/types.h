/*
 * types.h
 *
 *  Created on: 29 Jun 2017
 *      Author: Damian Dolewski
 */

#ifndef INC_TYPES_H_
#define INC_TYPES_H_

typedef struct Measurements
{
	int humidity;
	int lux;
	float temp_up;
	float temp_middle;
	float temp_down;
	float ph_water;
	float ph_soil;
	char * soil_moist;
} measurement_t;

#endif /* INC_TYPES_H_ */
