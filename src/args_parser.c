#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "args_parser.h"

#define ARGS_PARSER_OPTION_AMOUNT 3

extern int   start_file;
extern char *start_file_path;

argument argOptions[ARGS_PARSER_OPTION_AMOUNT] = {0};

/*
  An example of how to do options that take no arguments
 */
static char displayHelp(char** options)
{
	printf("CMACS\n");
	return 1;
}

/*
  An example of how to do options that take a single argument
 */
static char displayHelpNTimes(char** options)
{
	char* opt = *(options+1);
	char* check = 0;
	unsigned int x = strtoul(opt, &check, 10);
	if(check == opt)
	{
		fprintf(stderr, "[ERROR] negative number passed to flag '%s'\n", *options);
		return -1;
	}
	for (int i=0; i < x; i++)
	{
		printf("help\n");
	}
	return 1;
}

static char openFile(char **options) {
	char* opt = *(options+1);
	printf("Opening File: %s\n", opt);
	
	start_file = 1;
	start_file_path = opt;

	return 0;
}

/*
  Returns:
  0 on success -> launch program
  1 on success -> do not launch program, print output and exit
  -1 on fail -> do not launch program
		uses the value `ret`
 */
char args_parser(int argc, char **argv)
{
	char ret = 0;

	// define flags, args + functions here
	argOptions[0].args = 0;
	argOptions[0].func = &displayHelp;
	strcpy(argOptions[0].flag, "-h");

	argOptions[1].args = 1;
	argOptions[1].func = &displayHelpNTimes;
	strcpy(argOptions[1].flag, "-H");

	argOptions[2].args = 1;
	argOptions[2].func = &openFile;
	strcpy(argOptions[2].flag, "-f");

	char found_flag; // set if we find the flag that the user passed
	for(int i=1; i < argc; i++)
	{
		found_flag = 0;
		for(int j=0; j < ARGS_PARSER_OPTION_AMOUNT; j++)
		{
			if(strcmp(*(argv+i), argOptions[j].flag) == 0)
			{
				// check the amount of arguments remaining could fit the flag
				if(i+1+argOptions[j].args > (argc))
				{
					fprintf(stderr, "[ERROR] too few arguments for flag '%s', "
							"%d expected\n", argOptions[j].flag,
							argOptions[j].args);
					return -1;
				}
				// run the argument function, and parse the args
				char c = (argOptions[j].func(argv+i)); 
				if (c != 0) // if the arg func returned not 0, the program should not run
					ret = 1;
				if (c == -1)
				{ // -1 designates a failure to parse for some reason
					fprintf(stderr, "[ERROR] could not parse command line argument"
					        "%s\n", *(argv+i));
					return -1;
				}
				i += argOptions[j].args; // skip over the amount of args used by the current flag
				found_flag = 1;
			}
		}
		if (!found_flag) // report the unknown flag to the user and exit
		{
			fprintf(stderr, "[ERROR] unknown flag '%s'\n", *(argv+i));
			return -1;
		}
	}
	return ret;
}
