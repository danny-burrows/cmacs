#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>

#include "fonts.h"
#include "textures.h"
#include "ui/label.h"
#include "ui/button.h"

static bool         cmacs_running = true;
static char        *window_title  = "cmacs";
static unsigned int window_width  = 640;
static unsigned int window_height = 480;

SDL_Color white = {255, 255, 255, 255};

void exit_cmacs(void) {
    cmacs_running = false;
}

static SDL_HitTestResult hit_test_callback(SDL_Window *window, const SDL_Point *pixel, void *data)
{
    // Top left box.
    if (pixel->y < 20 && pixel->x < 20) {
        return SDL_HITTEST_RESIZE_TOPLEFT;
    }

    // No top right box.

    // Bottom left box.
    if (pixel->y > (int)window_height - 20 && pixel->x < 20) {
        return SDL_HITTEST_RESIZE_BOTTOMLEFT;
    }

    // Bottom right box.
    if (pixel->y > (int)window_height - 20 && pixel->x > (int)window_width - 20) {
        return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
    }
    
    // Top side.
    if (pixel->y < 4) {
        return SDL_HITTEST_RESIZE_TOP;
    }

    // Top bar drags window position.
    if (pixel->y < 27 && pixel->x < (int)window_width - 101) {
        return SDL_HITTEST_DRAGGABLE;
    }

    // Bottom side.
    if (pixel->y > (int)window_height - 12) {
        return SDL_HITTEST_RESIZE_BOTTOM;
    }

    // Right side.
    if (pixel->x > (int)window_width - 12 && pixel->y > 27) {
        return SDL_HITTEST_RESIZE_RIGHT;
    }

    // Left side.
    if (pixel->x < 12) {
        return SDL_HITTEST_RESIZE_LEFT;
    }

    return SDL_HITTEST_NORMAL;
}

static SDL_Window *init_sdl2_window(void)
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

    // Set window borderless and setup hit testing for custom border.
    void *hit_test_data = NULL;
    SDL_SetWindowBordered(window, SDL_FALSE);
    SDL_SetWindowHitTest(window, hit_test_callback, hit_test_data);

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
    if (load_fonts() == -1) {
        fprintf(stderr, "TTF Failed to load fonts.\n");

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Load textures.
    if (load_textures(renderer) == -1) {
        fprintf(stderr, "Failed to load textures.\n");

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }


    Label *title = Label_Create(renderer, 6, 1, window_title, fonts.font_regular_bold, white, 0, 0, 0);

    // These button icons and sizes are really hacked together :/
    Button *minimise_btn = Button_Create(renderer, window_width - 101, 2, "A", fonts.font_regular, white, 10, 0, NULL);
    minimise_btn->text_texture = textures.min_icon;
    minimise_btn->text_rect.y += 13;
    minimise_btn->text_rect.h -= 18;

    Button *maximise_btn = Button_Create(renderer, window_width - 67, 2, "A", fonts.font_regular, white, 10, 0, NULL);
    maximise_btn->text_texture = textures.max_icon;
    maximise_btn->text_rect.y += 6;
    maximise_btn->text_rect.h -= 10;

    Button *exit_btn = Button_Create(renderer, window_width - 33, 2, "A", fonts.font_regular, white, 10, 0, exit_cmacs);
    exit_btn->text_texture = textures.exit_icon;
    exit_btn->text_rect.y += 5;
    exit_btn->text_rect.h = 13;
    exit_btn->text_rect.x -= 2;
    exit_btn->text_rect.w = 14;
    exit_btn->background_texture = textures.red_label_background;

    // Setup title bar, border.
    SDL_Rect title_bar_rect = {0};
    title_bar_rect.w = window_width;
    title_bar_rect.h = 27;
    
    SDL_Rect border_rect = {0};
    border_rect.w = window_width;
    border_rect.h = window_height;

    SDL_Rect s_border_rect = {0};
    s_border_rect.x = 1;
    s_border_rect.y = 1;
    s_border_rect.w = window_width - 2;
    s_border_rect.h = window_height - 2;

    // Tagline text.
    Label *tagline = Label_Create(renderer, 0, 0,
        "Inoperable project to make Emacs with a better paradigm.", 
        fonts.font_large, white, 0, 0, 0
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

    Label *new_label = Label_Create(renderer, 20, 40, "Hello Labels.", fonts.font_regular, white, 1, 10, 1);
    Label *test_label = Label_Create(renderer, 200, 40, "Testing Label 2", fonts.font_regular, white, 1, 10, 1);

    Button *new_button = Button_Create(renderer, 20, 75, "Hello Buttons.", fonts.font_regular, white, 10, 1, NULL);

    Button *new_button1 = Button_Create(renderer, 20, 120, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button2 = Button_Create(renderer, 20, 150, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button3 = Button_Create(renderer, 20, 180, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button4 = Button_Create(renderer, 20, 210, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button5 = Button_Create(renderer, 20, 240, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button6 = Button_Create(renderer, 20, 270, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);

    SDL_Event event;
    while (cmacs_running) {

        // Process events.
        while (SDL_PollEvent(&event)) {

            switch (event.type) {
                case SDL_QUIT:
                    return 1;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_Q:
                        case SDL_SCANCODE_ESCAPE:
                        cmacs_running = false;
                        default:
                        break;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    Button_HoverCheck_AllButtons(event.motion.x, event.motion.y);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        Button_CallFor_AllButtons(Button_PressCheck);            
                    }
                    break;

                case SDL_MOUSEBUTTONUP:    
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        Button_CallFor_AllButtons(Button_ReleaseCheck);
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

                        // Update title bar and border.
                        title_bar_rect.w = window_width;
                        
                        border_rect.w = window_width;
                        border_rect.h = window_height;

                        s_border_rect.w = window_width - 2;
                        s_border_rect.h = window_height - 2;

                        // Update title bar buttons.
                        Button_SetPos(minimise_btn, window_width - 101, 2);
                        minimise_btn->text_rect.y += 13;

                        Button_SetPos(maximise_btn, window_width - 67, 2);
                        maximise_btn->text_rect.y += 6;

                        Button_SetPos(exit_btn, window_width - 33, 2);
                        exit_btn->text_rect.y += 5;
                        exit_btn->text_rect.x -= 2;

                        break;
                }
            }
        }

        // Clear screen.
        SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
        SDL_RenderClear(renderer);
        
        // Title Bar
        SDL_SetRenderDrawColor(renderer, 70, 70, 85, 255);
        SDL_RenderFillRect(renderer, &title_bar_rect);
        // Window Border
        SDL_RenderDrawRect(renderer, &border_rect);
        SDL_RenderDrawRect(renderer, &s_border_rect);

        SDL_RenderCopy(renderer, logo_image, NULL, &logo_rect);

        Label_RenderCopy_AllLabels(renderer);
        Button_RenderCopy_AllButtons(renderer);

        // Present frame.
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }

    // Graceful exit.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
