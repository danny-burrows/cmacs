#include "ui/label.h"

LabelRegister label_register;

Label *Label_Create(
    SDL_Renderer *renderer, 
    int x, int y, 
    const char *label_text, 
    TTF_Font *label_font, 
    SDL_Color text_color, 
    bool background, 
    int pad_x, int pad_y
){
    Label *label = (Label *)malloc(sizeof(Label));

    // Label reg stuff...
    label->next = label_register.top_label;
    label_register.top_label = label;

    label->text = label_text;
    label->background = background;
    label->text_color = text_color;
    label->pad_x = pad_x;
    label->pad_y = pad_y;

    SDL_Surface *label_text_surface = TTF_RenderText_Blended(
        label_font, 
        label->text, 
        label->text_color
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

    label->text_texture = SDL_CreateTextureFromSurface(renderer, label_text_surface);
    SDL_FreeSurface(label_text_surface);

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
        SDL_RenderCopy(renderer, textures.label_background, NULL, &label->rect);
    }

    SDL_RenderCopy(renderer, label->text_texture, NULL, &label->text_rect);
}

void Label_Destroy(Label *label) {
    SDL_DestroyTexture(label->text_texture);
    free(label);
}

// These functions act on all labels in the label register...

// Run a callback on all labels.
void Label_CallFor_AllLabels(void (*callback)(Label *lbl)) {
    Label *lbl = label_register.top_label;
    while (lbl) {
        callback(lbl);
        lbl = lbl->next;
    }
}

// Render copy all buttons.
void Label_RenderCopy_AllLabels(SDL_Renderer *renderer) {
    Label *lbl = label_register.top_label;
    while (lbl) {
        Label_RenderCopy(renderer, lbl);
        lbl = lbl->next;
    }
}
