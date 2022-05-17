#ifndef CMACS_CONFIG_H
#define CMACS_CONFIG_H

#include "cmacs_lang_layer.h"

typedef struct Config_
{
	u8 tabwidth;
}Config;

extern Config globalConfig;

char config_load(char* filepath);

#endif
