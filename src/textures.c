#include "textures.h"

Textures textures;

int load_textures(SDL_Renderer *renderer) {
    
    SDL_Surface *label_background_surface = IMG_Load("assets/ui/label-background.svg");
    textures.label_background = SDL_CreateTextureFromSurface(renderer, label_background_surface);
    SDL_FreeSurface(label_background_surface);

    SDL_Surface *button_hovered_background_surface = IMG_Load("assets/ui/button-hovered-background.svg");
    textures.button_hovered_background = SDL_CreateTextureFromSurface(renderer, button_hovered_background_surface);
    SDL_FreeSurface(button_hovered_background_surface);

    SDL_Surface *button_pressed_background_surface = IMG_Load("assets/ui/button-pressed-background.svg");
    textures.button_pressed_background = SDL_CreateTextureFromSurface(renderer, button_pressed_background_surface);
    SDL_FreeSurface(button_pressed_background_surface);

    SDL_Surface *min_icon_surface = IMG_Load("assets/ui/min-icon.svg");
    textures.min_icon = SDL_CreateTextureFromSurface(renderer, min_icon_surface);
    SDL_FreeSurface(min_icon_surface);

    SDL_Surface *max_icon_surface = IMG_Load("assets/ui/max-icon.svg");
    textures.max_icon = SDL_CreateTextureFromSurface(renderer, max_icon_surface);
    SDL_FreeSurface(max_icon_surface);

    SDL_Surface *exit_icon_surface = IMG_Load("assets/ui/exit-icon.svg");
    textures.exit_icon = SDL_CreateTextureFromSurface(renderer, exit_icon_surface);
    SDL_FreeSurface(exit_icon_surface);

    SDL_Surface *red_label_background_surface = IMG_Load("assets/ui/red-label-background.svg");
    textures.red_label_background = SDL_CreateTextureFromSurface(renderer, red_label_background_surface);
    SDL_FreeSurface(red_label_background_surface);
    
    return 0;
}