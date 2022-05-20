#ifndef CMACS_CONFIG_H
#define CMACS_CONFIG_H

#include "cmacs_lang_layer.h"

typedef struct Config_ {
    uint tabwidth;
    uint window_width;
    char window_width_overridden;
    uint window_height;
    char window_height_overridden;
    char file_path[256]; // File path temporarily capped at 256 chars.
    char enabled;
} Config;

extern Config globalConfig;

char config_load();

#endif
