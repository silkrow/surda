/*****************************************************************************
* File:             myshell.h
*
* Author:           Kyle  
* Created:          12/20/21 
*
* Description:      This file contains a slightly modified version of the shell
* built by Stephen Brennan which was originally called 'lsh'. The repo: https:/
* /github.com/brenns10/lsh I extended the implementation and added the 
* corresponding functions that supports surda program.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> // For wait(), waitpid().
#include <sys/wait.h>
#include "log.h"
#include "printing.h"
#include "time.h"

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a" 
#define HELPFILE "info/helper.txt"

/***********************************PRED**************************************/

void lsh_loop(void);
char *lsh_read_line (void);
char **lsh_split_line(char *line);
int lsh_launch(char **args);
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_execute(char **args);
static Log* log_0;
struct tm *now;
struct tm *st_week;

/***********************************IMPL**************************************/

/******************************************************************************
* Function:         void lsh_loop
* Arguments:		void	
* Return:           none
* Error:            none
*
* Description:      The main loop of the lsh shell, called by the main function
* and starts the shell. lsh_read_line gets the command line(in the form of char*),
* lsh_split_line then splits the line into a two dimensional char array(with the
* inner dimension containing each token string).
*****************************************************************************/
void lsh_loop(void){
	char *line;
	char **args;
	int status = 1;

	time_t t = time(NULL);
  	now = localtime(&t);
	st_week = malloc(sizeof(struct tm));

	/* Use st_week to contain the Sunday of beginning of this week. */
	*st_week = *now;
	st_week->tm_mday -= now->tm_wday;
	mktime(st_week);

	/* Preprocessing, set the log file to this week! */
	char log_file_name[30]; 
	char week_date[15];

	strftime(week_date, 14, "%F", st_week);
	strcpy(log_file_name, LOGPATH);
	strcat(log_file_name, week_date);
	
	if (NULL == (log_0 = malloc(sizeof(Log)))){
		printf("surda error: Failed in memory allocation.\n");
		return;
	}
	log_0->name = log_file_name;
	log_0->open = 0;
	log_0->t = 0;

	/* Before everything, remind the user if there's no log file for this week yet.*/
	touch_log(log_0);


	/* Main loop of the command line. */
	do{
		char line2[1024];
		printf("surda:> ");
		line = lsh_read_line();
		strcpy(line2, line);
		args = lsh_split_line(line);

		/* If status is 1, then keep on asking for new commands,
		 * if status is 2, read the text of plan. */
		switch (status){
			case 1:
				status = lsh_execute(args);
				break;
			case 2:
				status = add_plan_str(line2, log_0);
				break;
			default:
				break;
		}

		free(line);
		free(args);

	} while (status);

	if (!log_0->name) free(log_0->name);
	if (!log_0->logf) free(log_0->logf);
	free(log_0);
}

/******************************************************************************
* Function:         char *lsh_read_line
* Arguments:		void
* Return:           The char* pointing the line input to lsh.
* Error:            none

* Description:      The function that takes the whole command line for following
* parsing. 
*****************************************************************************/
char *lsh_read_line(void){
	int bufsize = LSH_RL_BUFSIZE;
	//int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);

	if (!buffer){
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	if (fgets(buffer, bufsize, stdin)){
		char *c;
		c = strchr(buffer, '\n');
		if (c) *c = 0;
		else while(getchar() != EOF && getchar() != '\n');
	}

	return buffer;
}

/******************************************************************************
* Function:         char **lsh_split_line
* Arguments:		char *line
* Return:           The address to the tokens array.
* Error:            none

* Description:      It handles the tokens and return them in an array(of str).
*****************************************************************************/
char **lsh_split_line(char *line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
  	char *token;

  	if (!tokens) {
   		fprintf(stderr, "lsh: allocation error\n");
    	exit(EXIT_FAILURE);
  	}

  	token = strtok(line, LSH_TOK_DELIM);
  	while (token != NULL) {
    	tokens[position] = token;
    	position++;

    	if (position >= bufsize) {
      		bufsize += LSH_TOK_BUFSIZE;
      		tokens = realloc(tokens, bufsize * sizeof(char*));
      		if (!tokens) {
        		fprintf(stderr, "lsh: allocation error\n");
        		exit(EXIT_FAILURE);
      		}
    	}

   		token = strtok(NULL, LSH_TOK_DELIM);
  	}
  
	tokens[position] = NULL;
  	return tokens;
}

/******************************************************************************
* Function:         int lsh_launch
* Arguments:		char **args	
* Return:           1 or EXIT_FAILURE
* Error:            none

* Description:      The function that creates processes in the system, which
* enables the shell to run with proper structure and long existance.
*****************************************************************************/
int lsh_launch(char **args)
{
	pid_t pid;
	int status;

  	pid = fork();
  	if (pid == 0) {
    	// Child process
    	if (execvp(args[0], args) == -1) {
      		perror("lsh");
    	}
    	exit(EXIT_FAILURE);
  	} else if (pid < 0) {
    	// Error forking
    	perror("lsh");
  	} else {
    
		// Parent process
    	do {
      		waitpid(pid, &status, WUNTRACED);
    	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  	"cd",
  	"help",
  	"exit"
};

int (*builtin_func[]) (char **) = {
  	&lsh_cd,
	&lsh_help,
	&lsh_exit
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int lsh_cd(char **args)
{
  	if (args[1] == NULL) {
    	fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  	} else {
    	if (chdir(args[1]) != 0) {
      	perror("lsh");
    	}
  	}
  	return 1;
}

int lsh_help(char **args)
{
	FILE *fhelp;
	char *shelp;

	if (NULL == (fhelp = fopen (HELPFILE, "r"))){
		fprintf(stderr, "error: helper.txt file DNE!\n");
		exit(EXIT_FAILURE);
	}

	fseek(fhelp, 0, SEEK_END);
	int fileLen = ftell(fhelp);
	shelp = (char*) malloc(sizeof(char) * fileLen);
	fseek(fhelp, 0, SEEK_SET);
	fread(shelp, fileLen, sizeof(char), fhelp);
	fclose(fhelp);

	shelp[fileLen - 1] = 0;

	printf("%s\n", shelp);
	free(shelp);
 	return 1;
}

int lsh_exit(char **args)
{
  	return 0;
}

/*
 * List of the functions from log.h. 
 */

char *log_str[] = {
  	"add",
	"a",
	"set",
	"del",
	"d"
};

int (*log_func[]) (char **, Log*) = {
  	&add_plan,
  	&add_plan,
	&set_log,
	&del_plan,
	&del_plan
};

int log_func_num() {
  	return sizeof(log_str) / sizeof(char *);
}



/******************************************************************************/
int lsh_execute(char **args)
{
	if (args[0] == NULL) {
    	// An empty command was entered.
    	return 1;
  	}

	for (int i = 0; i < lsh_num_builtins(); i++) {
    	if (strcmp(args[0], builtin_str[i]) == 0) {
		return (*builtin_func[i])(args);
    	}
  	}

	for (int i = 0; i < log_func_num(); ++i){
		if (strcmp(args[0], log_str[i]) == 0)
			return (*log_func[i])(args, log_0);// log needs to be predefined in somewhere of surda.c or here.
	}

  	return lsh_launch(args);
}

