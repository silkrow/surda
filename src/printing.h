/******************************************************************************
* File:             printing.h
*
* Author:           Kyle  
* Created:          12/27/21 

* Description:      The contents related with formatted outputs of the shell.
*****************************************************************************/

#include <stdio.h>

#define BLANK "                        " 
#define SPLIT "|"
#define SOLINE "------------------------"
#define LINE  "- - - - - - - - - - - - "
#define HYPH "-"
#define 

/***********************************PRED**************************************/
char** set_empty_table();



/***********************************IMPL**************************************/

/******************************************************************************
* Function:         char** set_empty_table
* Arguments:		none
* Return:           A pointer to the array of chars containing the table.
* Error:            none

* Description:      This is a set up function.
*****************************************************************************/
char** set_empty_table(){
	char** table;
	if (NULL == (table = malloc(sizeof(table)))) return NULL;
	if (NULL == (*table = malloc(sizeof(char))))



}

