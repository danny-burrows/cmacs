#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>

#include "fonts.h"
#include "window.h"
#include "textures.h"
#include "str_buffer.h"
#include "cmacs_buffer.h"
#include "ui/label.h"
#include "ui/button.h"
#include "config.h"
#include "args_parser.h"

static bool         cmacs_running = true;
static char        *window_title  = "cmacs";
unsigned int window_width;
unsigned int window_height;

SDL_Color white = {255, 255, 255, 255};

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
        globalConfig.window_width,
        globalConfig.window_height,
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
	switch(args_parser(argc, args))
	{
	case 0:
		printf("[INFO] Correctly parsed arguments\n");
		break;
	case 1:
		printf("[INFO] Exiting without opening, due to command arguments\n");
		return 0;
	case -1:
		printf("[ERROR] Exiting program\n");
		return 1;
	}
	
	// load config first so we can affect window creation
    if(config_load() == -1)
    {
	    printf("[WARN] could not load config, using defaults\n");
    }
    window_width  = globalConfig.window_width;
    window_height = globalConfig.window_height;

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

    // Enable blending for transparancy effects etc...
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

    Label *test_label = Label_Create(renderer, window_width - 135, 15, "Testing Label", fonts.font_regular, white, 1, 10, 1);
    Button *test_button_list[8] = {0};
    for (int i = 0; i < 8; i++) {
        test_button_list[i] = Button_Create(renderer, window_width - 135, 45 + i * 30, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    }

    Window *text_window = Window_Create();

    SDL_StartTextInput();

    SDL_Event event;
    while (cmacs_running) {

        // Process events.
        while (SDL_PollEvent(&event)) {

            switch (event.type) {
                case SDL_QUIT:
                    cmacs_running = false;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_RETURN:
                            Window_NewLine(text_window);
                            break;
                        case SDL_SCANCODE_UP:
                            Window_CursorUp(text_window);
                            break;
                        case SDL_SCANCODE_DOWN:
                            Window_CursorDown(text_window);
                            break;
                        case SDL_SCANCODE_TAB:
                            // Tab-width of 4 spaces...
                            for (unsigned int i = 0; i < globalConfig.tabwidth; i++) {
                                StrBuffer_AddChar(text_window->buffer->current_line, ' ', text_window->cursor.column);
                                text_window->cursor.column++;
                                if(text_window->cursor.column % globalConfig.tabwidth == 0)
	                                break;
                            }
                            break;
                        case SDL_SCANCODE_BACKSPACE:
                            if (text_window->cursor.column < 1) {
                                if (text_window->cursor.line < 1) break;
                                Window_RemoveLine(text_window);
                            } else {
                                text_window->cursor.column--;
                                StrBuffer_RemoveChar(text_window->buffer->current_line, text_window->cursor.column);
                            };
                            break;
                        case SDL_SCANCODE_LEFT:
                            Window_CursorLeft(text_window);
                            break;
                        case SDL_SCANCODE_RIGHT:
                            Window_CursorRight(text_window);
                            break;
                        
                        case SDL_SCANCODE_Q:
                        case SDL_SCANCODE_ESCAPE:
                            cmacs_running = false;
                        default:
                        break;
                    }
                    break;

                case SDL_TEXTINPUT:
                    // This will cause problems for bigger chars. Will need to handle SDL_TEXTEDITNG
                    //                                                   |
                    StrBuffer_AddChar(text_window->buffer->current_line, event.text.text[0], text_window->cursor.column);
                    text_window->cursor.column++;
                    break;

                case SDL_MOUSEMOTION:
                    Button_HoverCheck_AllButtons(event.motion.x, event.motion.y);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        Button_CallFor_AllButtons(Button_PressCheck);
                    break;

                case SDL_MOUSEBUTTONUP:    
                    if (event.button.button == SDL_BUTTON_LEFT)
                        Button_CallFor_AllButtons(Button_ReleaseCheck);
                    break;
            }

            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        window_width = event.window.data1; 
                        window_height = event.window.data2;

                        // Calc logo position.
                        logo_rect.x = window_width / 2 - (logo_rect.w / 2);
                        logo_rect.y = window_height / 2 - (logo_rect.h / 2) - 40;

                        // Calc tagline position.
                        Label_SetPos(tagline, 
                            window_width / 2 - (tagline->rect.w / 2),
                            window_height / 2 - (tagline->rect.h / 2) + 115
                        );

                        // Update label & buttons.
                        Label_SetPos(test_label, window_width - 135, test_label->rect.y);
                        for (int i = 0; i < 8; i++) {
                            Button_SetPos(test_button_list[i], window_width - 135, test_button_list[i]->rect.y);
                        }
                        break;
                }
            }
        }

        // Clear screen.
        SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
        SDL_RenderClear(renderer);

        Window_Render(text_window, renderer);

        SDL_RenderCopy(renderer, logo_image, NULL, &logo_rect);

        Label_RenderCopy_AllLabels(renderer);
        Button_RenderCopy_AllButtons(renderer);
        
        // Present frame.
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }

    // Graceful exit.
    Window_Destroy(text_window);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
