#include "ui/label.h"

Label *Label_Create(SDL_Renderer *renderer, const char *label_text, int x, int y, TTF_Font *label_font, SDL_Color text_color, bool background, int pad_x, int pad_y) 
{
    Label *label = (Label *)malloc(sizeof(Label));

    label->text = label_text;
    label->background = background;
    label->color = text_color;
    label->pad_x = pad_x;
    label->pad_y = pad_y;

    SDL_Surface *label_text_surface = TTF_RenderText_Blended(
        label_font, 
        label->text, 
        label->color
    );

    label->rect.x = x;
    label->rect.y = y;
    label->rect.w = label_text_surface->w + (pad_x * 2); 
    label->rect.h = label_text_surface->h + (pad_y * 2) + 2;

    // Calc text position.
    label->text_rect.x = label->rect.x + pad_x;
    label->text_rect.y = label->rect.y + pad_y;
    label->text_rect.w = label_text_surface->w;
    label->text_rect.h = label_text_surface->h;

    label->texture = SDL_CreateTextureFromSurface(renderer, label_text_surface);
    SDL_FreeSurface(label_text_surface);
    
    if (label->background) {
        SDL_Surface *label_background_surface = IMG_Load("assets/ui/label-background.svg");

        label->background_texture = SDL_CreateTextureFromSurface(renderer, label_background_surface);
        SDL_FreeSurface(label_background_surface);
    }

    return label;
}

void Label_SetPos(Label *label, int x, int y) {
    label->rect.x = x;
    label->rect.y = y;

    // Ensure text in correct position. (Should probably done when moving text.)
    label->text_rect.x = label->rect.x + label->pad_x;
    label->text_rect.y = label->rect.y + label->pad_y;
}

void Label_RenderCopy(SDL_Renderer *renderer, Label *label)
{
    if (label->background) {
        SDL_RenderCopy(renderer, label->background_texture, NULL, &label->rect);
    }

    SDL_RenderCopy(renderer, label->texture, NULL, &label->text_rect);
}

void Label_Destroy(Label *label) {
    SDL_DestroyTexture(label->texture);
    SDL_DestroyTexture(label->background_texture);
    free(label);
}