#include "fonts.h"

Fonts fonts;

int load_fonts(void) 
{
    fonts.font_mono_regular = TTF_OpenFont("assets/NotoSansMono-Regular.ttf", 13);
    if (fonts.font_mono_regular == NULL) return -1;

    fonts.font_regular = TTF_OpenFont("assets/open-sans.ttf", 15);
    if (fonts.font_regular == NULL) return -1;

    fonts.font_large = TTF_OpenFont("assets/open-sans.ttf", 20);
    if (fonts.font_large == NULL) return -1;

    fonts.font_larger = TTF_OpenFont("assets/open-sans.ttf", 64);
    if (fonts.font_larger == NULL) return -1;

    return 0;
}
