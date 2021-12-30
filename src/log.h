/******************************************************************************
* File:             log.h
*
* Author:           Kyle  
* Created:          12/22/21 

* Description:     	This file contains the table implementation of surda app.
* surda stores the schedule of a whole week in one log file in the .surdalog 
* directory. 
*
* log is a set of adding and deleting operations recorded after surda starts to
* run, and before exiting, it shall call the update_log function to update the 
* log file so that it contains only adding operations. table sturcture is used 
* to contain the infomation of table elements that about to be displayed
* (Locations, strings...). 
* 
* table is based on log.

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define LOGPATH "surdalog/"

/***********************************PRED**************************************/
typedef struct Log{
	/* plan[day][time]. */
	//char*** plan;
	FILE* logf;
} Log;

/* The Table structure should contain a file of txt form to display the table,
 * as well as seven linked lists that keeps track of the plans in each day of 
 * the week. Mod structure shall contain Table inside of it, when there is an 
 * attempt to load mod, the table of this week should be checked with the mod
 * for any conflicts. For the Mod structure, Table inside of it may not need
 * to contain a file of txt. */
typedef struct Table{
	/* Use name string to identify the name, for example "mod1", or "week1". */
	char* name; 
	/* A string to the address of the file to display. */
	/* Seven trees of plans constructed according to the log. */

} Table;

int count_args (char** args);
int add_plan (char** args, Log* log);
int add_plan_str(char** args, Log* log);

int del_plan (int start, int date, Log* log);
void show_table (Table* table, Log* log);
void log_update (Log* log);
void log_store (Table*, Table*);
int mod_load (Table*, Table*);

/***********************************IMPL**************************************/

/******************************************************************************
* Function:         int count_args
* Arguments:		char** args
* Return:           The number of valid arguments passed in.
* Error:            none

* Description:      
*****************************************************************************/
int count_args(char** args){
	int n = 0;

	/* Start testing from args[1], assuming args[0] is not interested. */
	while (args[++n] != NULL);

	return n;
}

/******************************************************************************
* Function:         int add_plan 
* Arguments:		char** args, Log* log
* Return:          	1
* Error:            none

* Description:      Reads in information and add plan to the log file.
*****************************************************************************/
int add_plan (char** args, Log* log){
	
	switch (count_args(args)){
		/* time1 time2*/
		case 2:
			return 1;

		/* time1 time2 day*/
		case 3:
			return 1;

		default:
			printf("surda: Invalid syntax for add!\n");
			printf("       Use add time1 time2 [day]\n"
		   			"       or  type \"help\" for help.\n");


			return 1;
	}
}

/******************************************************************************
* Function:         int add_plan_str
* Arguments:		char** args, Log* log
* Return:           -1 if the added plan is invalid, 1 if it's valid.
* Error:            none

* Description:      Simply copy the string input to the log file.
*****************************************************************************/
int add_plan_str(char** args, Log* log){
	

	return 1;	
}

/******************************************************************************
* Function:         int del_plan 
* Arguments:		int start, int date, Log* log
* Return:          	Whether the deleting is legal. 
* Error:            none

* Description:      Add a deleting record in the log file.
*****************************************************************************/
int del_plan (int start, int date, Log* log){
	
}

/******************************************************************************
* Function:         void show_table 
* Arguments:		Table* table, Log* log
* Return:           none
* Error:            none

* Description:      A function to display the table file in the shell.
*****************************************************************************/
void show_table (Table* table, Log* log){
	
}

/******************************************************************************
* Function:         void log_update
* Arguments:		Log* log 
* Return:           none
* Error:            none

* Description:      A function that rewrites the log file by calculating all 
* the records up to now, and leave it with only adding instructions.
*****************************************************************************/
void log_update(Log* log){
	
}

/******************************************************************************
* Function:         void log_store
* Arguments:		Table* mod, Table* table 
* Return:           none
* Error:            none

* Description:      A function that stores a table of plans as a module, which 
* can be used in the future conveniently.
*****************************************************************************/
void log_store(Table* mod, Table* table){
	
}

/******************************************************************************
* Function:         int mod_load
* Arguments:		Table* mod, Table* table
* Return:           Whether there're conflicts when loading the module.
* Error:            none

* Description:      Attempts to load a module of schedule from some pre-stored
* modules to the log file this week. 
*
* further feature:	Feed back the conflicting elements(or the time period).
*****************************************************************************/
int mod_load(Table* mod, Table* table){
	
}
