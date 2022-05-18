#ifndef CMACS_CONFIG_H
#define CMACS_CONFIG_H

#include "cmacs_lang_layer.h"

typedef struct Config_
{
	uint tabwidth;
	uint window_width;
	uint window_height;
	char file_path[];
}Config;

extern Config globalConfig;

char config_load();

#endif
