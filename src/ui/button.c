#include "ui/button.h"

ButtonRegister button_register;

Button *Button_Create(
    SDL_Renderer *renderer, 
    int x, int y, 
    const char *button_text, 
    TTF_Font *button_font, 
    SDL_Color text_color, 
    int pad_x, int pad_y
){
    Button *button = (Button *)calloc(1, sizeof(Button));

    // Button reg stuff...
    button->next = button_register.top_button;
    button_register.top_button = button;

    button->text = button_text;
    button->text_color = text_color;
    button->pad_x = pad_x;
    button->pad_y = pad_y;

    SDL_Surface *button_text_surface = TTF_RenderText_Blended(
        button_font, 
        button->text, 
        button->text_color
    );

    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = button_text_surface->w + (pad_x * 2); 
    button->rect.h = button_text_surface->h + (pad_y * 2) + 2;

    // Calc text position.
    button->text_rect.x = button->rect.x + pad_x;
    button->text_rect.y = button->rect.y + pad_y;
    button->text_rect.w = button_text_surface->w;
    button->text_rect.h = button_text_surface->h;

    button->text_texture = SDL_CreateTextureFromSurface(renderer, button_text_surface);
    SDL_FreeSurface(button_text_surface);

    // Background texture.
    SDL_Surface *button_background_surface = IMG_Load("assets/ui/label-background.svg");
    button->background_texture = SDL_CreateTextureFromSurface(renderer, button_background_surface);
    SDL_FreeSurface(button_background_surface);

    // Hovered Background texture.
    SDL_Surface *button_hovered_background_surface = IMG_Load("assets/ui/button-hovered-background.svg");
    button->hovered_background_texture = SDL_CreateTextureFromSurface(renderer, button_hovered_background_surface);
    SDL_FreeSurface(button_hovered_background_surface);

    // Pressed Background texture.
    SDL_Surface *button_pressed_background_surface = IMG_Load("assets/ui/button-pressed-background.svg");
    button->pressed_background_texture = SDL_CreateTextureFromSurface(renderer, button_pressed_background_surface);
    SDL_FreeSurface(button_pressed_background_surface);

    return button;
}

void Button_SetPos(Button *button, int x, int y) {
    button->rect.x = x;
    button->rect.y = y;

    // Ensure text in correct position. (Should probably done when moving text.)
    button->text_rect.x = button->rect.x + button->pad_x;
    button->text_rect.y = button->rect.y + button->pad_y;
}

void Button_HoverCheck(Button *button, int mouse_x, int mouse_y) {
    button->is_hovered = 0;

    if (mouse_x > button->rect.x && 
        mouse_x < (button->rect.x + button->rect.w) &&
        mouse_y > button->rect.y && 
        mouse_y < (button->rect.y + button->rect.h)
    ) {
        button->is_hovered = 1;
    }
}

void Button_PressCheck(Button *button) {
    if (button->is_hovered) button->is_pressed = 1;
}

void Button_ReleaseCheck(Button *button) {
    button->is_pressed = 0;
}

void Button_RenderCopy(SDL_Renderer *renderer, Button *button)
{
    // Ew... ew... messy... ew.
    if (button->is_hovered) {
        if (button->is_pressed) {
            SDL_RenderCopy(renderer, button->pressed_background_texture, NULL, &button->rect);
        } else {
            SDL_RenderCopy(renderer, button->hovered_background_texture, NULL, &button->rect);
        }
    } else {
        SDL_RenderCopy(renderer, button->background_texture, NULL, &button->rect);
    }

    SDL_RenderCopy(renderer, button->text_texture, NULL, &button->text_rect);
}

void Button_Destroy(Button *button) {
    SDL_DestroyTexture(button->text_texture);
    SDL_DestroyTexture(button->background_texture);
    free(button);
}

// These functions act on all buttons in the button register...

// Run a callback on all buttons.
void Button_CallFor_AllButtons(void (*callback)(Button *btn)) {
    Button *btn = button_register.top_button;
    while (btn) {
        callback(btn);
        btn = btn->next;
    }
}

// Check hover all buttons.
void Button_HoverCheck_AllButtons(int mouse_x, int mouse_y) {
    Button *btn = button_register.top_button;
    while (btn) {
        Button_HoverCheck(btn, mouse_x, mouse_y);
        btn = btn->next;
    }
}

// Render copy all buttons.
void Button_RenderCopy_AllButtons(SDL_Renderer *renderer) {
    Button *btn = button_register.top_button;
    while (btn) {
        Button_RenderCopy(renderer, btn);
        btn = btn->next;
    }
}
