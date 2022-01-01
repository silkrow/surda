/******************************************************************************
* File:             surda.c
*
* Author:           Kyle
* Created:          12/20/21
*
* Description:      
* surda stands for "Schedule ur day ahead". It's a cli app that helps with 
* making schedules and plans. This file is the main file of the app.
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "surda.h"

/******************************************************************************
* Function:         int main
* Arguments:		int argc, char* argv[]
* Return:           EXIT_FAILURE or EXIT_SUCCESS
* Error:            none
* 
* Description:      The main function of the program. 
*****************************************************************************/
int main(int argc, char* argv[]){

	FILE* flogo;
	char *slogo;

	if (NULL == (flogo = fopen (LOGOFILE, "r"))){
		fprintf(stderr, "error: logo.txt file DNE!\n");
		exit(EXIT_FAILURE);
	}
	
	fseek(flogo, 0, SEEK_END);
	int fileLen = ftell(flogo);
	slogo = (char*) malloc(sizeof(char)*fileLen);
	fseek(flogo, 0, SEEK_SET);
	fread(slogo, fileLen, sizeof(char), flogo);
	fclose(flogo);
	
	// Not sure why the last char in slogo will turn out
	// to be unexpected character.
	slogo[fileLen - 1] = 0; 

	printf("%s\n", slogo);
	
	free(slogo);
	lsh_loop();


	return EXIT_SUCCESS;
}
