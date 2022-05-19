#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "config.h"
#include "cmacs_lang_layer.h"

#define strval(s) str(s)
#define str(s) #s

#define CMACS_CONFIG_BUFFER_LENGTH 128

Config globalConfig = {
    .tabwidth = 8,
    .window_width = 800,
    .window_width_overridden = 0,
    .window_height = 600,
    .window_height_overridden = 0,
    .file_path = "cmacs.conf",
    .enabled = 1};

// local definitions
local char config_set_value(char *key, char *value);
local char config_parse_uint(uint* dest, char* value);


local char config_set_value(char* key, char* value)
{
    if(strcmp(key, "tabwidth") == 0)
    {
        if(config_parse_uint(&globalConfig.tabwidth, value) != 0)
        {
            printf("in key %s\n", key);
            return -1;
        }
    }
    else if (strcmp(key, "window_width") == 0)
    {
        if(!globalConfig.window_width_overridden)
        {
            if(config_parse_uint(&globalConfig.window_width, value) != 0)
            {
                printf("in key %s\n", key);
                return -1;
            }
        }
    }
    else if (strcmp(key, "window_height") == 0)
    {
        if(!globalConfig.window_height_overridden)
        {
            if(config_parse_uint(&globalConfig.window_height, value) != 0)
            {
                printf("in key %s\n", key);
                return -1;
            }
        }
    }
    else
    {
        printf("[WARN] unknown key (%s) with value (%s)\n", key, value);
        return -1;
    }
    return 0;
}

local char config_parse_uint(uint* dest, char* value)
{
    char* check = 0;
    uint new_val = strtoul(value, &check, 10);
    if(check == value)
    {
        printf("[WARN] could not parse unsigned int ");
        return -1;
    }
    if(new_val > UINT_MAX)
    {
        printf("[WARN] unsigned int bound (%d) exceeded, tried to read %ul ", UINT_MAX, new_val);
    }
    *dest = new_val;
    return 0;
}

char config_load()
{
    if(!globalConfig.enabled)
    {
        printf("[INFO] config not loaded (argument passed), using defaults\n");
        return 0;
    }
    FILE* file_ptr = fopen(globalConfig.file_path, "r");
    if(file_ptr == NULL)
    {
        fprintf(stderr, "[WARN] Could not open config file at %s: ", globalConfig.file_path);
        perror("");
        return -1;
    }
    char key_buffer[CMACS_CONFIG_BUFFER_LENGTH+1] = {0};
    char value_buffer[CMACS_CONFIG_BUFFER_LENGTH+1] = {0};
    char eof_reached = 0;
    eof_reached = fscanf(file_ptr, "%s" strval(CMACS_CONFIG_BUFFER_LENGTH), key_buffer);
    eof_reached = fscanf(file_ptr, "%s" strval(CMACS_CONFIG_BUFFER_LENGTH), value_buffer);
    int i = 1;
    while(eof_reached != EOF)
    {
        if(key_buffer[CMACS_CONFIG_BUFFER_LENGTH] != 0)
        {
            printf("[WARN] could not parse line %d of config file %s, key too long!\n", i, globalConfig.file_path);
            fclose(file_ptr);
            return -1;
        }
        else if(value_buffer[CMACS_CONFIG_BUFFER_LENGTH] != 0)
        {
            printf("[WARN] could not parse line %d (with key %s) of config file %s, value too long\n!", i, key_buffer, globalConfig.file_path);
            fclose(file_ptr);
            return -1;
        }
        config_set_value(key_buffer, value_buffer);
        eof_reached = fscanf(file_ptr, "%s" strval(CMACS_CONFIG_BUFFER_LENGTH), key_buffer);
        eof_reached = fscanf(file_ptr, "%s" strval(CMACS_CONFIG_BUFFER_LENGTH), value_buffer);
    }
    fclose(file_ptr);
    return 0;
}

