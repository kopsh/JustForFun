/*************************************************************************
    > File Name: shellex.c
    > Author: kopsh
    > Mail: kopbupt@126.com 
    > Created Time: Sat 31 Aug 2013 02:15:54 PM CST
 ************************************************************************/

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <ctype.h>  
#include <signal.h>  
#include <sys/types.h>  
#include <sys/wait.h>  
#include <errno.h>  

#define MAXARGS 128
#define MAXLINE 1024

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

void unix_error(char const *msg)
{
	fprintf(stdout, "%s:%s\n", msg, strerror(errno));
	exit(0);
}

int main()
{
	char cmdline[MAXLINE]; /* Command line */

	while(1)
	{
		printf("> ");
		fgets(cmdline, MAXLINE, stdin);

		if(feof(stdin))
			exit(0);
		
		eval(cmdline);
	}
}

void eval(char* cmdline)
{
	char *argv[MAXARGS]; /* Argument list execve() */
	char buf[MAXLINE];   /* Holds modified command line */
	int bg;				 /* Should the job run in bg or fg? */
	pid_t pid;           /* Process id */

	strcpy(buf, cmdline);
	bg = parseline(buf, argv);
	if(argv[0] == NULL)
		return;	/* Ignore empty lines */

	if(!builtin_command(argv))
		if((pid = fork()) == 0)	/* Child runs user job */
			if(execve(argv[0], argv, environ) < 0)
			{
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}

	/* Parent waits for foreground job to terminate */
	if(!bg)
	{
		int status;
		if(waitpid(pid, &status, 0) < 0)
			unix_error("waitfg: waitpid error");
	}
	else
		printf("%d %s", pid, cmdline);

	return;
}

/*If first arg is a builtin command, run it and return true */
int builtin_command(char **argv)
{
	if(!strcmp(argv[0], "quit")) /* quit command */
		exit(0);
	if(!strcmp(argv[0], "&"))    /* Ignore singleton */
		return 1;
	return 0;					 /* Not a builtin command */
}

/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv)
{
	char *delim;	/* Point to first space delimiter */
	int argc;       /* Number of args */
	int bg;			/* Background job? */

	buf[strlen(buf)-1] = ' ';
	while(*buf && (*buf == ' '))
		buf++;

	/* Build the argv list */
	argc = 0;
	while((delim = 
		buf = delim + 1;
		while(*buf && (*buf == ' '))
			buf++;
	}
	argv[argc] = NULL;

	if(argc == 0)
		return 1;

	/* Should the job run in the background? */
	if((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;

	return bg;
}
