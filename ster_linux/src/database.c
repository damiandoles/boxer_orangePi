/*
 * database.c
 *
 *  Created on: 23 Jun 2017
 *      Author: Damian Dolewski
 */

#include "database.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATEMENT_LEN	256

static int SelectBasicMeas_Callback(void *NotUsed, int argc, char **argv, char **azColName);
static int SelectPhMeas_Callback(void *NotUsed, int argc, char **argv, char **colName);

static char sBasicMeas [6][32] 	= {{0},{0}};
static char sPhMeas [2][8] 		= {{0},{0}};

void DataBase_TestInsert(void)
{
#ifdef DEBUG_DB_INSERT_STATEMENT
	basic_meas_t testBasicMeas;
	strcpy(testBasicMeas.humidity, 		"48");
	strcpy(testBasicMeas.lux, 			"16678");
	strcpy(testBasicMeas.temp_up, 		"29.5");
	strcpy(testBasicMeas.temp_middle, 	"28.4");
	strcpy(testBasicMeas.temp_down, 	"27.7");
	strcpy(testBasicMeas.soil_moist, 	"DRY");

	DataBase_InsertBasicMeas(&testBasicMeas);

	ph_meas_t testPhMeas;

	strcpy(testPhMeas.ph_soil, 	"6.78");
	strcpy(testPhMeas.ph_water, "7.11");

	DataBase_InsertPhMeas(&testPhMeas);
#endif
}

void DataBase_Init(void)
{
	pthread_mutex_lock(&uartDB_mutex);
	sqlite3 * database;
	char * zErrMsg = 0;
	char * sqlStatement = (char*)calloc(MAX_STATEMENT_LEN, sizeof(char));

	if (sqlite3_open("sqldb/boxer.db", &database) != SQLITE_OK)
	{
#ifdef DEBUG_SQL_DATABASE
		fprintf(stderr, "DataBase_Init[Error]: Can't open database: %s\n\r", sqlite3_errmsg(database));
#endif
		sqlite3_close(database);
		exit(EXIT_FAILURE);
	}
	else
	{
#ifdef DEBUG_SQL_DATABASE
		fprintf(stdout, "DataBase_Init[Success]: Opened database successfully\n\r");
#endif
		snprintf(sqlStatement,
				MAX_STATEMENT_LEN,
				"CREATE TABLE IF NOT EXISTS BASIC_MEAS("  	\
				"HUMIDITY		INTEGER," 					\
				"LUX			INTEGER," 					\
				"TEMP_UP        FLOAT," 					\
				"TEMP_MIDDLE    FLOAT," 					\
				"TEMP_DOWN      FLOAT," 					\
				"SOIL_MOIST     TEXT NOT NULL," 			\
				"TIMELOCAL      TEXT NOT NULL);");

		if (sqlite3_exec(database, sqlStatement, SelectBasicMeas_Callback, 0, &zErrMsg) != SQLITE_OK)
		{
#ifdef DEBUG_SQL_DATABASE
			fprintf(stderr, "DataBase_Init[Error]: SQL exec %s\n\r", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			exit(EXIT_FAILURE);
		}
#ifdef DEBUG_SQL_DATABASE
		else
		{
			fprintf(stdout, "DataBase_Init[Success]: Table BASIC_MEAS created successfully\n\r");
		}
#endif
		snprintf(sqlStatement,
				MAX_STATEMENT_LEN,
				"CREATE TABLE IF NOT EXISTS PH_MEAS(" \
				"PH_WATER	FLOAT," \
				"PH_SOIL	FLOAT," \
				"TIMELOCAL  TEXT NOT NULL);");

		if (sqlite3_exec(database, sqlStatement, SelectBasicMeas_Callback, 0, &zErrMsg) != SQLITE_OK)
		{
#ifdef DEBUG_SQL_DATABASE
			fprintf(stderr, "DataBase_Init[Error]: SQL exec %s\n\r", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			exit(EXIT_FAILURE);
		}
#ifdef DEBUG_SQL_DATABASE
		else
		{
			fprintf(stdout, "DataBase_Init[Success]: Table PH_MEAS created successfully\n\r");
		}
#endif

		sqlite3_close(database);
		free(sqlStatement);
	}

	pthread_mutex_unlock(&uartDB_mutex);
}

void DataBase_InsertBasicMeas(basic_meas_t * meas)
{
	pthread_mutex_lock(&uartDB_mutex);
	sqlite3 * database;
	char * zErrMsg = 0;
	char * sqlStatement = (char*)calloc(MAX_STATEMENT_LEN, sizeof(char));

	if (sqlite3_open("sqldb/boxer.db", &database) != SQLITE_OK)
	{
#ifdef DEBUG_SQL_DATABASE
		fprintf(stderr, "DataBase_InsertBasicMeas[Error]: Can't open database: %s\n\r", sqlite3_errmsg(database));
#endif
		sqlite3_close(database);
		exit(EXIT_FAILURE);
	}
	else
	{
#ifdef DEBUG_SQL_DATABASE
		fprintf(stdout, "DataBase_InsertBasicMeas[Success]: Opened database successfully\n\r");
#endif
		snprintf(sqlStatement,
				MAX_STATEMENT_LEN,
				"INSERT INTO BASIC_MEAS (HUMIDITY, LUX, TEMP_UP, TEMP_MIDDLE, TEMP_DOWN, SOIL_MOIST, TIMELOCAL) \
				VALUES (%s,%s,%s,%s,%s,'%s',datetime('now', 'localtime'));",
				meas->humidity,
				meas->lux,
				meas->temp_up,
				meas->temp_middle,
				meas->temp_down,
				meas->soil_moist);

		if (sqlite3_exec(database, sqlStatement, SelectBasicMeas_Callback, 0, &zErrMsg) != SQLITE_OK)
		{
#ifdef DEBUG_SQL_DATABASE
			fprintf(stderr, "DataBase_InsertBasicMeas[Error]: SQL exec %s\n\r", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			exit(EXIT_FAILURE);
		}
#ifdef DEBUG_SQL_DATABASE
		else
		{
			fprintf(stdout, "DataBase_InsertBasicMeas[Success]: Data inserted successfully to BASIC_MEAS\n\r");
		}
#endif

		sqlite3_close(database);
		free(sqlStatement);
	}

	pthread_mutex_unlock(&uartDB_mutex);
}

void DataBase_InsertPhMeas(ph_meas_t * meas)
{
	pthread_mutex_lock(&uartDB_mutex);
	sqlite3 * database;
	char * zErrMsg = 0;
	char * sqlStatement = (char*)calloc(MAX_STATEMENT_LEN, sizeof(char));

	if (sqlite3_open("sqldb/boxer.db", &database) != SQLITE_OK)
	{
#ifdef DEBUG_SQL_DATABASE
		fprintf(stderr, "DataBase_InsertPhMeas[Error]: Can't open database: %s\n\r", sqlite3_errmsg(database));
#endif
		sqlite3_close(database);
		exit(EXIT_FAILURE);
	}
	else
	{
#ifdef DEBUG_SQL_DATABASE
		fprintf(stdout, "DataBase_InsertPhMeas[Success]: Opened database successfully\n\r");
#endif
		if (meas->ph_soil[0] != '\0' && meas->ph_water[0] == '\0')
		{
			snprintf(sqlStatement,
					MAX_STATEMENT_LEN,
					"INSERT INTO PH_MEAS (PH_WATER, PH_SOIL, TIMELOCAL) \
					 VALUES (NULL,%s,datetime('now', 'localtime'));",
					meas->ph_soil);
		}
		else if (meas->ph_soil[0] == '\0' && meas->ph_water[0] != '\0')
		{
			snprintf(sqlStatement,
					MAX_STATEMENT_LEN,
					"INSERT INTO PH_MEAS (PH_WATER, PH_SOIL, TIMELOCAL) \
					 VALUES (%s,NULL,datetime('now', 'localtime'));",
					meas->ph_water);
		}
		else
		{
			snprintf(sqlStatement,
					MAX_STATEMENT_LEN,
					"INSERT INTO PH_MEAS (PH_WATER, PH_SOIL, TIMELOCAL) \
					 VALUES (%s,%s,datetime('now', 'localtime'));",
					meas->ph_water,
					meas->ph_soil);
		}

		if (sqlite3_exec(database, sqlStatement, SelectBasicMeas_Callback, 0, &zErrMsg) != SQLITE_OK)
		{
#ifdef DEBUG_SQL_DATABASE
			fprintf(stderr, "DataBase_InsertPhMeas[Error]: SQL exec %s\n\r", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			exit(EXIT_FAILURE);
		}
#ifdef DEBUG_SQL_DATABASE
		else
		{
			fprintf(stdout, "DataBase_InsertPhMeas[Success]: Data inserted successfully to PH_MEAS\n\r");
		}
#endif
		sqlite3_close(database);
		free(sqlStatement);
	}

	pthread_mutex_unlock(&uartDB_mutex);
}

void DataBase_SelectMeasData(basic_meas_t * basicMeas, ph_meas_t * phMeas)
{
	pthread_mutex_lock(&uartDB_mutex);
	sqlite3 * database;
	char * zErrMsg = 0;
	char * sqlStatement = (char*)calloc(MAX_STATEMENT_LEN, sizeof(char));

	if (sqlite3_open("sqldb/boxer.db", &database) != SQLITE_OK)
	{
#ifdef DEBUG_SQL_DATABASE
		fprintf(stderr, "DataBase_SelectData[Error]: Can't open database: %s\n\r", sqlite3_errmsg(database));
#endif
		sqlite3_close(database);
		exit(EXIT_FAILURE);
	}
	else
	{
#ifdef DEBUG_SQL_DATABASE
		fprintf(stdout, "DataBase_SelectData[Success]: Opened database successfully\n\r");
#endif

		const char * basicMeasStatement = "SELECT * FROM BASIC_MEAS ORDER BY TIMELOCAL DESC Limit 1";
		strcpy(sqlStatement, basicMeasStatement);

		if (sqlite3_exec(database, sqlStatement, SelectBasicMeas_Callback, 0, &zErrMsg) != SQLITE_OK)
		{
#ifdef DEBUG_SQL_DATABASE
			fprintf(stderr, "DataBase_SelectData[Error]: SQL exec %s\n\r", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			exit(EXIT_FAILURE);
		}
#ifdef DEBUG_SQL_DATABASE
		else
		{
			fprintf(stdout, "DataBase_SelectData[Success]: Data selected successfully from BASIC_MEAS\n\r");
		}
#endif

// HUMIDITY = 48 LUX = 16678 TEMP_UP = 29.5 TEMP_MIDDLE = 28.4 TEMP_DOWN = 27.7 SOIL_MOIST = DRY TIMELOCAL = 2017-09-07 21:52:25

		strcpy(basicMeas->humidity, 	sBasicMeas[0]);
		strcpy(basicMeas->lux, 			sBasicMeas[1]);
		strcpy(basicMeas->temp_up, 		sBasicMeas[2]);
		strcpy(basicMeas->temp_middle, 	sBasicMeas[3]);
		strcpy(basicMeas->temp_down, 	sBasicMeas[4]);
		strcpy(basicMeas->soil_moist, 	sBasicMeas[5]);

		memset(sqlStatement, 0, sizeof(MAX_STATEMENT_LEN));
		const char * pHMeasStatement = "SELECT * FROM PH_MEAS ORDER BY TIMELOCAL DESC Limit 1";
		strcpy(sqlStatement, pHMeasStatement);

		if (sqlite3_exec(database, sqlStatement, SelectPhMeas_Callback, 0, &zErrMsg) != SQLITE_OK)
		{
#ifdef DEBUG_SQL_DATABASE
			fprintf(stderr, "DataBase_SelectData[Error]: SQL exec %s\n\r", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			exit(EXIT_FAILURE);
		}
#ifdef DEBUG_SQL_DATABASE
		else
		{
			fprintf(stdout, "DataBase_SelectData[Success]: Data selected successfully from BASIC_MEAS\n\r");
		}
#endif
// PH_WATER = NULL	PH_SOIL = 4.89	TIMELOCAL = 2017-09-07 22:09:34

		strcpy(phMeas->ph_water,  sPhMeas[0]);
		strcpy(phMeas->ph_soil, sPhMeas[1]);

		sqlite3_close(database);
		free(sqlStatement);
	}

	pthread_mutex_unlock(&uartDB_mutex);
}

static int SelectBasicMeas_Callback(void *NotUsed, int argc, char **argv, char **colName)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\t", colName[i], argv[i] ?  : "NULL");
        strcpy(sBasicMeas[i], argv[i] ?  : "NULL");
    }

    printf("\r\n");
    return 0;
}

static int SelectPhMeas_Callback(void *NotUsed, int argc, char **argv, char **colName)
{
	memset(sPhMeas,0,16);
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\t", colName[i], argv[i] ?  : "NULL");
        strcpy(sPhMeas[i], argv[i] ?  : "NULL");
    }

    printf("\r\n");
    return 0;
}
