#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "cmacs_lang_layer.h"

Config globalConfig = {.tabwidth = 8};

local char config_set_value(char* key, char* value)
{
	if(strcmp(key, "tabwidth") == 0)
	{
		char* check = 0;
		uint new_tabwidth = strtoul(value, &check, 10);
		if(check == value)
		{
			printf("[WARN] could not parse the value of key %s, likely cause: it is not an int\n", key);
			return -1;
		}
		globalConfig.tabwidth = new_tabwidth;
	}
	else
	{
		printf("[WARN] unknown key (%s) with value (%s)\n", key, value);
		return -1;
	}
	return 0;
}

char config_load(char* filepath)
{
	FILE* file_ptr = fopen(filepath, "r");
	if(file_ptr == NULL)
	{
		printf("[WARN] could not open config file at %s, likely cause: it does not exist\n", filepath);
		return -1;
	}
	char key_buffer[128] = {0};
	char value_buffer[128] = {0};
	char eof_reached = 0;
	eof_reached = fscanf(file_ptr, "%s128 %s128", key_buffer, value_buffer);
	int i = 1;
	while(eof_reached != EOF)
	{
		if(key_buffer[127] != 0)
		{
			printf("[WARN] could not parse line %d of config file %s, key too long!\n", i, filepath);
			return -1;
		}
		else if(value_buffer[127] != 0)
		{
			printf("[WARN] could not parse line %d (with key %s) of config file %s, value too long\n!", i, key_buffer, filepath);
			return -1;
		}

	}
	return 0;
}
