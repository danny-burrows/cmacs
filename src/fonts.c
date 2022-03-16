#include "fonts.h"

Fonts fonts;

int load_fonts(void) {
    fonts.font_mono_regular = TTF_OpenFont("assets/monoid-reg.ttf", 12);
    fonts.font_regular = TTF_OpenFont("assets/open-sans.ttf", 15);
    fonts.font_large = TTF_OpenFont("assets/open-sans.ttf", 20);
    fonts.font_larger = TTF_OpenFont("assets/open-sans.ttf", 64);
    if (fonts.font_regular == NULL || fonts.font_large == NULL || fonts.font_larger == NULL) {
        return -1;
    }
    return 0;
}
