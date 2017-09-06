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

void DataBase_TestInsert(void)
{
#ifdef TEST_DB_INSERT_STATEMENT
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
	sqlite3 * database;
	char * zErrMsg = 0;
	char * sqlStatement = (char*)calloc(MAX_STATEMENT_LEN, sizeof(char));

	/* Open SQL database */
	if (sqlite3_open("sqldb/boxer.db", &database) != SQLITE_OK)
	{
		fprintf(stderr, "DataBase_Init[Error]: Can't open database: %s\n\r", sqlite3_errmsg(database));
	}
	else
	{
		fprintf(stdout, "DataBase_Init[Success]: Opened database successfully\n\r");

		/* Create SQL statement */
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

		/* Execute SQL statement */
		if (sqlite3_exec(database, sqlStatement, callback, 0, &zErrMsg) != SQLITE_OK)
		{
			fprintf(stderr, "DataBase_Init[Error]: SQL exec %s\n\r", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			fprintf(stdout, "DataBase_Init[Success]: Table BASIC_MEAS created successfully\n\r");
		}

		/* Create SQL statement */
		snprintf(sqlStatement,
				MAX_STATEMENT_LEN,
				"CREATE TABLE IF NOT EXISTS PH_MEAS(" \
				"PH_WATER	FLOAT," \
				"PH_SOIL	FLOAT," \
				"TIMELOCAL  TEXT NOT NULL);");

		/* Execute SQL statement */
		if (sqlite3_exec(database, sqlStatement, callback, 0, &zErrMsg) != SQLITE_OK)
		{
			fprintf(stderr, "DataBase_Init[Error]: SQL exec %s\n\r", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			fprintf(stdout, "DataBase_Init[Success]: Table PH_MEAS created successfully\n\r");
		}

		sqlite3_close(database);
		free(sqlStatement);
	}
}

void DataBase_InsertBasicMeas(basic_meas_t * meas)
{
	sqlite3 * database;
	char * zErrMsg = 0;
	char * sqlStatement = (char*)calloc(MAX_STATEMENT_LEN, sizeof(char));

	/* Open SQL database */
	if (sqlite3_open("sqldb/boxer.db", &database) != SQLITE_OK)
	{
		fprintf(stderr, "DataBase_InsertBasicMeas[Error]: Can't open database: %s\n\r", sqlite3_errmsg(database));
	}
	else
	{
		fprintf(stdout, "DataBase_InsertBasicMeas[Success]: Opened database successfully\n\r");

		/* Create SQL statement */
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

		/* Execute SQL statement */
		if (sqlite3_exec(database, sqlStatement, callback, 0, &zErrMsg) != SQLITE_OK)
		{
			fprintf(stderr, "DataBase_InsertBasicMeas[Error]: SQL exec %s\n\r", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			fprintf(stdout, "DataBase_InsertBasicMeas[Success]: Data inserted successfully to BASIC_MEAS\n\r");
		}

		sqlite3_close(database);
		free(sqlStatement);
	}
}

void DataBase_InsertPhMeas(ph_meas_t * meas)
{
	sqlite3 * database;
	char * zErrMsg = 0;
	char * sqlStatement = (char*)calloc(MAX_STATEMENT_LEN, sizeof(char));

	/* Open SQL database */
	if (sqlite3_open("sqldb/boxer.db", &database) != SQLITE_OK)
	{
		fprintf(stderr, "DataBase_InsertPhMeas[Error]: Can't open database: %s\n\r", sqlite3_errmsg(database));
	}
	else
	{
		fprintf(stdout, "DataBase_InsertPhMeas[Success]: Opened database successfully\n\r");

		if (meas->ph_soil[0] != '\0' && meas->ph_water[0] == '\0')
		{
			/* Create SQL statement */
			snprintf(sqlStatement,
					MAX_STATEMENT_LEN,
					"INSERT INTO PH_MEAS (PH_WATER, PH_SOIL, TIMELOCAL) \
					 VALUES (NULL,%s,datetime('now', 'localtime'));",
					meas->ph_soil);
		}
		else if (meas->ph_soil[0] == '\0' && meas->ph_water[0] != '\0')
		{
			/* Create SQL statement */
			snprintf(sqlStatement,
					MAX_STATEMENT_LEN,
					"INSERT INTO PH_MEAS (PH_WATER, PH_SOIL, TIMELOCAL) \
					 VALUES (%s,NULL,datetime('now', 'localtime'));",
					meas->ph_water);
		}
		else
		{
			/* Create SQL statement */
			snprintf(sqlStatement,
					MAX_STATEMENT_LEN,
					"INSERT INTO PH_MEAS (PH_WATER, PH_SOIL, TIMELOCAL) \
					 VALUES (%s,%s,datetime('now', 'localtime'));",
					meas->ph_water,
					meas->ph_soil);
		}

		/* Execute SQL statement */
		if (sqlite3_exec(database, sqlStatement, callback, 0, &zErrMsg) != SQLITE_OK)
		{
			fprintf(stderr, "DataBase_InsertPhMeas[Error]: SQL exec %s\n\r", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			fprintf(stdout, "DataBase_InsertPhMeas[Success]: Data inserted successfully to PH_MEAS\n\r");
		}

		sqlite3_close(database);
		free(sqlStatement);
	}
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	for (int i = 0; i < argc; i++)
	{
		fprintf(stdout,"%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	fprintf(stdout,"\n");
	return 0;
}
