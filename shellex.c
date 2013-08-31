/*************************************************************************
    > File Name: shellex.c
    > Author: kopsh
    > Mail: kopbupt@126.com 
    > Created Time: Sat 31 Aug 2013 02:15:54 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAXARGS 128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

int main()
{

}
