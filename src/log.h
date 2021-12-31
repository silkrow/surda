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
#include <time.h>

#define LOGPATH "surdalog/log"

extern struct tm* st_week;
extern struct tm* now;
/***********************************PRED**************************************/
typedef struct Log{
	/* plan[day][time]. */
	//char*** plan;
	int open;
	char* name;
	FILE* logf;
	int t; //  Indicating which type this log is.
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
int touch_log(Log* log);
int set_log(char** args, Log* log);
int time_convert(char* s, int day);
int jump_line(FILE* file, int line);


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
	int start, end, duration;
	int day;

	if (!touch_log(log)) return 1;

	switch (count_args(args)){
		/* time1 time2*/
		case 3:
			day = now->tm_wday;
			if (time_convert(args[1], day) == -1 || time_convert(args[2], day) == -1 ||
					(start = time_convert(args[1], day)) >= (end = time_convert(args[2], day))){
				printf("surda: Invalid syntax for add!\n"
					   "       Use add time1 time2 [day]\n"
					   "       where time should be in format of 24h, with preceding 0 for hours < 10.\n"
					   "       day should be a number of 0-6, 0 for Sunday.\n"
					   "       example:     add 08:00 24:00 4\n"
					   "       Type \"help\" for help.\n");
			}
			else{
				duration = end - start;
				log->logf = fopen(log->name, "r+");
				fseek(log->logf, 0, SEEK_SET);// Preset the pointer in the file.
				
				/* Now set the pointer in the file to the start line. */
				if (!jump_line(log->logf, start)){
					printf("surda: Adding failed, something's wrong with your log file!!\n");
					fclose(log->logf);
					return 1;
				}
				else{
					for (int i = 0; i < duration; ++i){
						if (fgetc(log->logf) != '0'){
							printf("There're already plans in the period!\n");
							fclose(log->logf);
							return 1;
						}
						jump_line(log->logf, 1);
					}
					
					fputs("here i am!!!", log->logf);

					/* Now it's checked to be fine. */
					printf("Please now enter the plan script, hit enter only when finished!\n");				
					fclose(log->logf);
					return 2;
				}
			}
			return 1;

		/* time1 time2 day*/
		case 4:
			return 1;

		default:
			printf("surda: Invalid syntax for add!\n"
				   "       Use add time1 time2 [day]\n"
				   "       where time should be in format of 24h, with preceding 0 for hours < 10.\n"
				   "       day should be a number of 0-6, 0 for Sunday.\n"
				   "       example:     add 08:00 24:00 4\n"
				   "       Type \"help\" for help.\n");
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
* Function:         int touch_log
* Arguments:		Log* log
* Return:           1 if success, 0 if the file doesn't exist.
* Error:            none

* Description:      Try to open the log with the name of file, and match it with
* the log pointer.
*****************************************************************************/
int touch_log(Log* log){
	if (NULL == (log->logf = fopen(log->name, "r"))){
		if (log->t == 0)
			printf("You don't have any schedule for week " 
				"%d-%02d-%02d yet, type \'set\' to get one.\n",
				st_week->tm_year + 1900, st_week->tm_mon + 1, st_week->tm_mday);
		else if(log->t == 1);
		return 0;
	} else fclose(log->logf);

	return 1;
}

/******************************************************************************
* Function:         int set_log
* Arguments:		char** args, Log* log
* Return:           1
* Error:            none

* Description:      Initialize a log file.
*****************************************************************************/
int set_log(char** args, Log* log){
	if (NULL == (log->logf = fopen(log->name, "w"))){
		printf("Failed to initialize a table of schedule!\n");
		return 1;
	}
	else{
		for (int i = 0; i < 1512; ++i)
			fputs("0\n", log->logf);
	}
	fclose(log->logf);

	return 1;
}

/******************************************************************************
* Function:         int time_convert
* Arguments:		char* s, int day
* Return:           The line number of that time stamp, -1 if invalid.
* Error:            none

* Description:      
*****************************************************************************/
int time_convert(char* s, int day){
	int n = 0;

	if (*s > '2' || *s < '0' || *(s + 1) < '0' || *(s + 1) > '9' || *(s + 2) != ':' ||
			*(s + 3) > '5' || *(s + 3) < '0' || (*(s + 4) != '0' && *(s + 4) != '5')) 
		return -1;
	else{
		n += (*s - '0')*10;
		n += (*(s + 1) - '0');
		if (n > 24 || n < 6) return -1;
		n = n*60;
		n += (*(s + 3) - '0')*10;
		n += (*(s + 4) - '0');

		if (n > 24*60) return -1;
	}
	n = (n - 6*60)/5;
	n = n + day*18*60/5;

	return n;
}

/******************************************************************************
* Function:         int jump_line
* Arguments:		FILE* file, int line
* Return:           1 if success, 0 if failed.
* Error:            none

* Description:      This function set the pointer in FILE* file to the next 
* 'line' line by stepping through lines with fgets().
*****************************************************************************/
int jump_line(FILE* file, int line){
	char s[1024];
	for (int i = 0; i < line; ++i)
		if (!fgets(s, 1024, file)) return 0;

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
