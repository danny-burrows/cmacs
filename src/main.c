#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>

#include "fonts.h"
#include "textures.h"
#include "str_buffer.h"
#include "cmacs_buffer.h"
#include "ui/label.h"
#include "ui/button.h"

#define MAX_LINES   15000
#define MAX_COLUMNS 100000

#define STRINGIFY(i) #i
#define DIGITS(i) (sizeof(STRINGIFY(i)) - 1) // (-1 for \0)
#define LINE_NUM_BUFFSIZE (DIGITS(MAX_LINES) + 1)

static bool         cmacs_running = true;
static char        *window_title  = "cmacs";
static unsigned int window_width  = 640;
static unsigned int window_height = 480;

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

    Label *new_label = Label_Create(renderer, 500, 20, "Hello Labels.", fonts.font_regular, white, 1, 10, 1);
    Label *test_label = Label_Create(renderer, 200, 20, "Testing Label 2", fonts.font_regular, white, 1, 10, 1);

    Button *new_button = Button_Create(renderer, 500, 55, "Hello Buttons.", fonts.font_regular, white, 10, 1, NULL);

    Button *new_button1 = Button_Create(renderer, 500, 100, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button2 = Button_Create(renderer, 500, 130, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button3 = Button_Create(renderer, 500, 160, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button4 = Button_Create(renderer, 500, 190, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button5 = Button_Create(renderer, 500, 220, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);
    Button *new_button6 = Button_Create(renderer, 500, 250, "List of Buttons", fonts.font_regular, white, 10, 1, NULL);

    CmacsBuffer *cmacs_buffer = CmacsBuffer_Create();

    SDL_Surface *text_buffer_surface = NULL;
    SDL_Texture *text_buffer_texture = NULL;
    SDL_Rect type_text_rect;

    SDL_StartTextInput();

    SDL_Event event;
    while (cmacs_running) {

        bool render_text = false;

        // Process events.
        while (SDL_PollEvent(&event)) {

            switch (event.type) {
                case SDL_QUIT:
                    cmacs_running = false;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_RETURN:
                            CmacsBuffer_AddLine(cmacs_buffer);
                            render_text = true;
                            break;
                        case SDL_SCANCODE_UP:
                            CmacsBuffer_UpLine(cmacs_buffer);
                            render_text = true;
                            break;
                        case SDL_SCANCODE_DOWN:
                            CmacsBuffer_DownLine(cmacs_buffer);
                            render_text = true;
                            break;
                        case SDL_SCANCODE_BACKSPACE:
                            if (cmacs_buffer->cursor.column < 1) {
                                if (!cmacs_buffer->current_line->prev) break;
                                CmacsBuffer_RemoveLine(cmacs_buffer);
                                break;
                            } else {
                                cmacs_buffer->cursor.column--;
                            };

                            StrBuffer_RemoveChar(cmacs_buffer->current_line, cmacs_buffer->cursor.column);
                            render_text = true;
                            break;
                        case SDL_SCANCODE_LEFT:
                            if (cmacs_buffer->cursor.column < 1) {
                                if (cmacs_buffer->current_line && cmacs_buffer->current_line->prev) {
                                    cmacs_buffer->current_line = cmacs_buffer->current_line->prev;
                                    cmacs_buffer->cursor.line--;

                                    cmacs_buffer->cursor.column = cmacs_buffer->current_line->str_length;
                                }
                                break;
                            }
                            cmacs_buffer->cursor.column--;
                            break;
                        case SDL_SCANCODE_RIGHT:
                            if (cmacs_buffer->cursor.column >= cmacs_buffer->current_line->str_length) {
                                if (cmacs_buffer->current_line && cmacs_buffer->current_line->next) {
                                    cmacs_buffer->current_line = cmacs_buffer->current_line->next;
                                    cmacs_buffer->cursor.line++;
                                    
                                    cmacs_buffer->cursor.column = 0;
                                }
                                break;
                            };
                            cmacs_buffer->cursor.column++;
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
                    StrBuffer_AddChar(cmacs_buffer->current_line, event.text.text[0], cmacs_buffer->cursor.column);
                    
                    cmacs_buffer->cursor.column++;
                    render_text = true;
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

                        render_text = true;
                        break;
                }
            }
        }

        // Clear screen.
        SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
        SDL_RenderClear(renderer);
        
        char line_num_buffer[LINE_NUM_BUFFSIZE];

        printf("%d\n", LINE_NUM_BUFFSIZE);

        SDL_Texture *line_num_texture = NULL;
        SDL_Surface *line_num_surface = NULL;

        SDL_Rect cursor_rect = {
            0, 0, 9, 20,
        };

        int i = 0;
        StrBuffer *line = cmacs_buffer->head;
        while (line) {
            
            if (line_num_texture) SDL_DestroyTexture(line_num_texture);

            sprintf(line_num_buffer, "%d", i + 1);
            line_num_surface = TTF_RenderText_Blended(
                fonts.font_mono_regular, 
                line_num_buffer,
                white
            );

            line_num_texture = SDL_CreateTextureFromSurface(renderer, line_num_surface);
            SDL_FreeSurface(line_num_surface);

            type_text_rect.y = i * (line_num_surface->h + 2);
            type_text_rect.h = line_num_surface->h;
            
            type_text_rect.w = 40;
            SDL_RenderCopy(renderer, textures.label_background, NULL, &type_text_rect);
            type_text_rect.w = line_num_surface->w;
            SDL_RenderCopy(renderer, line_num_texture, NULL, &type_text_rect);

            // Render line text
            if (line->str_length) {
                SDL_DestroyTexture(text_buffer_texture);

                text_buffer_surface = TTF_RenderText_Blended(
                    fonts.font_mono_regular, 
                    line->data,
                    white
                );

                type_text_rect.x = 50;
                type_text_rect.w = text_buffer_surface->w;
                type_text_rect.h = text_buffer_surface->h;

                text_buffer_texture = SDL_CreateTextureFromSurface(renderer, text_buffer_surface);
                SDL_FreeSurface(text_buffer_surface);
                SDL_RenderCopy(renderer, text_buffer_texture, NULL, &type_text_rect);
            }
            type_text_rect.x = 0;
            type_text_rect.y = 0;

            //Cursor
            if (cmacs_buffer->cursor.line == i) {
                cursor_rect.x = (cmacs_buffer->cursor.column * 9) + 50;
                cursor_rect.y = i * (type_text_rect.h + 2);

                SDL_SetRenderDrawColor(renderer, 225, 225, 255, 90);
                
                SDL_RenderFillRect(renderer, &cursor_rect);
            }
            
            line = line->next;
            i++;
        }

        SDL_SetRenderDrawColor(renderer, 225, 225, 245, 255);

        SDL_RenderCopy(renderer, logo_image, NULL, &logo_rect);

        Label_RenderCopy_AllLabels(renderer);
        Button_RenderCopy_AllButtons(renderer);
        
        // Present frame.
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }

    // Graceful exit.
    CmacsBuffer_Destroy(cmacs_buffer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
