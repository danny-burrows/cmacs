#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>

#include "ui/label.h"
#include "ui/button.h"

static char        *window_title  = "cmacs";
static unsigned int window_width  = 640;
static unsigned int window_height = 480;

SDL_Color white = {255, 255, 255, 255};

static SDL_Window *init_sdl2_window()
{
    // Initialise SDL.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());

        SDL_Quit();
        return NULL;
    }

    // Initialise SDL window.
    SDL_Window *window = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width,
        window_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (window == NULL) {
        fprintf(stderr, "SDL failed to create window: %s\n", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }

    return window;
}


// SDL Requires this exact signature for cross platform.
int main(int argc, char *args[]) 
{
    SDL_Window *window = init_sdl2_window();
    if (window == NULL) return -1;

    // Initialise image and font support.
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    // Create SDL2 Renderer.
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "SDL Failed to create renderer: %s\n", SDL_GetError());

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialise and prepare fonts.
    TTF_Font *font_regular = TTF_OpenFont("open_sans.ttf", 15);
    TTF_Font *font_large = TTF_OpenFont("open_sans.ttf", 20);
    TTF_Font *font_larger = TTF_OpenFont("open_sans.ttf", 64);
    if (font_regular == NULL || font_large == NULL || font_larger == NULL) {
        fprintf(stderr, "TTF Failed to load font: Not found :/\n");

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Tagline text.
    Label *tagline = Label_Create(renderer, 
        "Inoperable project to make Emacs with a better paradigm.", 
        0, 0, font_large, white, 0, 0, 0
    );
    Label_SetPos(tagline, 
        window_width / 2 - (tagline->rect.w / 2),
        window_height / 2 - (tagline->rect.h / 2) + 115
    );

    // Logo image.
    SDL_Surface *logo_image_surface = IMG_Load("assets/cmacs-icon.svg");
    SDL_Rect logo_rect = {
        window_width / 2 - (logo_image_surface->w / 4),
        window_height / 2 - (logo_image_surface->h / 4) - 40,
        logo_image_surface->w / 2,
        logo_image_surface->h / 2
    };
    SDL_Texture *logo_image = SDL_CreateTextureFromSurface(renderer, logo_image_surface);
    SDL_FreeSurface(logo_image_surface);

    Label *new_label = Label_Create(renderer, "Hello Labels.", 20, 20, font_regular, white, 1, 10, 1);
    Button *new_button = Button_Create(renderer, 20, 55, "Hello Buttons.", font_regular, white, 10, 1);

    SDL_Event event;
    while (1) {

        // Process events.
        while (SDL_PollEvent(&event)) {

            switch (event.type) {
                case SDL_QUIT:
                    return 1;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_Q:
                        case SDL_SCANCODE_ESCAPE:
                        return 0;
                        default:
                        break;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    Button_HoverCheck(new_button, event.motion.x, event.motion.y);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        Button_PressCheck(new_button);
                    }                    
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        Button_ReleaseCheck(new_button);
                    }                           
                    break;
            }

            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        window_width = event.window.data1; 
                        window_height = event.window.data2;

                        // Calc text position.
                        Label_SetPos(tagline, 
                            window_width / 2 - (tagline->rect.w / 2),
                            window_height / 2 - (tagline->rect.h / 2) + 115
                        );

                        // Calc logo position.
                        logo_rect.x = window_width / 2 - (logo_rect.w / 2);
                        logo_rect.y = window_height / 2 - (logo_rect.h / 2) - 40;

                        break;
                }
            }
        }

        // Clear screen.
        SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
        SDL_RenderClear(renderer);
        
        Label_RenderCopy(renderer, tagline);
        SDL_RenderCopy(renderer, logo_image, NULL, &logo_rect);

        Label_RenderCopy(renderer, new_label);
        Button_RenderCopy(renderer, new_button);
        
        // Present frame.
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }
    return 0;
}
