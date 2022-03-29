#include "textures.h"
#include <stdio.h>

Textures textures;

int load_textures(SDL_Renderer *renderer)
{    
	//fprintf(stderr, "msg\n");

    SDL_Surface *label_background_surface = IMG_Load("assets/ui/label-background.svg");
    if(label_background_surface == NULL)
    {
	    fprintf(stderr, "Failed to load label image into surface.\n");
	    return -1;
    }
    textures.label_background = SDL_CreateTextureFromSurface(renderer, label_background_surface);
    if(textures.label_background == NULL)
    {
	    fprintf(stderr, "Failed to create texture for label background, likely a renderer issue.\n");
	    return -1;
    }
    SDL_FreeSurface(label_background_surface);

    SDL_Surface *button_hovered_background_surface = IMG_Load("assets/ui/button-hovered-background.svg");
    if(button_hovered_background_surface == NULL)
    {
	    fprintf(stderr, "Failed to load button hovered background into surface.\n");
	    return -1;
    }
    textures.button_hovered_background = SDL_CreateTextureFromSurface(renderer, button_hovered_background_surface);
    if(textures.label_background == NULL)
    {
	    fprintf(stderr, "Failed to create texture for button hovered background, likely a renderer issue.\n");
	    return -1;
    }
    SDL_FreeSurface(button_hovered_background_surface);

    SDL_Surface *button_pressed_background_surface = IMG_Load("assets/ui/button-pressed-background.svg");
    if(button_pressed_background_surface == NULL)
    {
	    fprintf(stderr, "Failed to load button pressed background into surface.\n");
	    return -1;
    }
    textures.button_pressed_background = SDL_CreateTextureFromSurface(renderer, button_pressed_background_surface);
    if(textures.label_background == NULL)
    {
	    fprintf(stderr, "Failed to create texture for button pressed background, likely a renderer issue.\n");
	    return -1;
    }
    SDL_FreeSurface(button_pressed_background_surface);
    
    return 0;
}
