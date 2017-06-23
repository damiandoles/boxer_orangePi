/*
 * database.c
 *
 *  Created on: 23 Jun 2017
 *      Author: dolewdam
 */

#include "database.h"
#include <stdio.h>
#include <stdlib.h>

int DataBase_Init(void)
{
	sqlite3 * db;

	if (sqlite3_open("boxer.db", &db))
	{
	      fprintf(stderr, "Can't open database: %s\n\r", sqlite3_errmsg(db));
	      return -1;
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n\r");
	}

	sqlite3_close(db);
	return 0;
}

