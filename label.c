#include "label.h"

Label *Label_Create(SDL_Renderer *renderer, const char *label_text, int x, int y, TTF_Font *label_font) 
{
    Label *label = (Label *)malloc(sizeof(Label));

    label->text = label_text;

    SDL_Color white = {255, 255, 255, 255};
    label->color = white;

    SDL_Surface *label_text_surface = TTF_RenderText_Blended(
        label_font, 
        label->text, 
        label->color
    );

    label->rect.x = x + 10;
    label->rect.y = y;
    label->rect.w = label_text_surface->w;
    label->rect.h = label_text_surface->h;

    label->texture = SDL_CreateTextureFromSurface(renderer, label_text_surface);
    SDL_FreeSurface(label_text_surface);
    

    // Label Background.
    SDL_Surface *label_background_surface = IMG_Load("assets/ui/label-background.svg");

    label->background_rect.x = x;
    label->background_rect.y = y;
    label->background_rect.w = label->rect.w + 20;
    label->background_rect.h = label->rect.h + 2;

    label->background_texture = SDL_CreateTextureFromSurface(renderer, label_background_surface);
    SDL_FreeSurface(label_background_surface);

    return label;
}

void Label_RenderCopy(SDL_Renderer *renderer, Label *label)
{
    SDL_RenderCopy(renderer, label->background_texture, NULL, &label->background_rect);
    SDL_RenderCopy(renderer, label->texture, NULL, &label->rect);
}
