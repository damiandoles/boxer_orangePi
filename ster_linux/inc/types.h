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
	char humidity[8];
	char lux[8];
	char temp_up[8];
	char temp_middle[8];
	char temp_down[8];
	char soil_moist[8];
} basic_meas_t;

typedef struct PHMeasurements
{
	char ph_water[8];
	char ph_soil[8];
} ph_meas_t;

#endif /* INC_TYPES_H_ */
