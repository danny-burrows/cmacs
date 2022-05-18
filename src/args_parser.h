#ifndef ANNA_ARGS_PARSER_H
#define ANNA_ARGS_PARSER_H

typedef char (*arg_func)(char**);

typedef struct argument_t
{
	arg_func func;
	int args;
	char flag[32];
}argument;

extern argument argOptions[];

char args_parser(int argc, char** argv);

#endif
