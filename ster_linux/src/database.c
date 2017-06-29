/*
 * database.c
 *
 *  Created on: 23 Jun 2017
 *      Author: Damian Dolewski
 */

#include "database.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STATEMENT_LEN	256

static int callback(void *NotUsed, int argc, char **argv, char **azColName);

void DataBase_Init(void)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	char *sql;

	/* Open SQL database */
	if (sqlite3_open("boxer.db", &db) != SQLITE_OK)
	{
		  fprintf(stderr, "DataBase_Init[Error]: Can't open database: %s\n\r", sqlite3_errmsg(db));
	}
	else
	{
		fprintf(stdout, "DataBase_Init[Success]: Opened database successfully\n\r");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE IF NOT EXISTS MEASUREMENTS("  \
		 "HUMIDITY		INTEGER," \
		 "LUX			INTEGER," \
		 "TEMP_UP         FLOAT," \
		 "TEMP_MIDDLE     FLOAT," \
		 "TEMP_DOWN       FLOAT," \
		 "PH_WATER        FLOAT," \
		 "PH_SOIL         FLOAT," \
		 "SOIL_MOIST      TEXT NOT NULL," \
		 "TIMELOCAL       TEXT NOT NULL);";

	/* Execute SQL statement */
	if (sqlite3_exec(db, sql, callback, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "DataBase_Init[Error]: SQL exec %s\n\r", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "DataBase_Init[Success]: Table created successfully\n\r");
	}

	sqlite3_close(db);
}

void DataBase_Insert(measurement_t * meas)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	char * sqlStatement = (char*)calloc(MAX_STATEMENT_LEN, sizeof(char));

	/* Open SQL database */
	if (sqlite3_open("boxer.db", &db) != SQLITE_OK)
	{
		  fprintf(stderr, "DataBase_Insert[Error]: Can't open database: %s\n\r", sqlite3_errmsg(db));
	}
	else
	{
		fprintf(stdout, "DataBase_Insert[Success]: Opened database successfully\n\r");
	}

	/* Create SQL statement */
	snprintf(sqlStatement,
			MAX_STATEMENT_LEN,
			"INSERT INTO MEASUREMENTS (HUMIDITY, LUX, TEMP_UP, TEMP_MIDDLE, TEMP_DOWN, PH_WATER, PH_SOIL, SOIL_MOIST, TIMELOCAL) \
			VALUES (%d,%d,%f,%f,%f,%f,%f,'%s',datetime('now', 'localtime'));",
			meas->humidity,
			meas->lux,
			meas->temp_up,
			meas->temp_middle,
			meas->temp_down,
			meas->ph_water,
			meas->ph_soil,
			meas->soil_moist);

	/* Execute SQL statement */
	if (sqlite3_exec(db, sqlStatement, callback, 0, &zErrMsg) != SQLITE_OK)
	{
		fprintf(stderr, "DataBase_Init[Error]: SQL exec %s\n\r", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "DataBase_Init[Success]: Data inserted successfully to measurements\n\r");
	}

	free(sqlStatement);
	sqlite3_close(db);
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	for(int i = 0; i < argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}
