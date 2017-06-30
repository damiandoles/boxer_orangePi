/*
 * types.h
 *
 *  Created on: 29 Jun 2017
 *      Author: Damian Dolewski
 */

#ifndef INC_TYPES_H_
#define INC_TYPES_H_

/*
 * all measurements data are string data because
 * uart frame is string type and its directly put
 * to the sql statement (snprintf function)
*/
typedef struct BasicMeasurements
{
	char * humidity;
	char * lux;
	char * temp_up;
	char * temp_middle;
	char * temp_down;
	char * soil_moist;
} basic_meas_t;

typedef struct PHMeasurements
{
	char * ph_water;
	char * ph_soil;
} ph_meas_t;

#endif /* INC_TYPES_H_ */
