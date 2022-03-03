#include "fonts.h"

Fonts fonts;

int load_fonts(void) {
    fonts.font_regular = TTF_OpenFont("assets/open_sans.ttf", 15);
    fonts.font_large = TTF_OpenFont("assets/open_sans.ttf", 20);
    fonts.font_larger = TTF_OpenFont("assets/open_sans.ttf", 64);
    if (fonts.font_regular == NULL || fonts.font_large == NULL || fonts.font_larger == NULL) {
        return -1;
    }
    return 0;
}
