#include "window.h"

// Creates a new text window. Will return NULL on failure.
Window *Window_Create(void)
{
    Window *window = (Window *)calloc(1, sizeof(Window));
    window->buffer = CmacsBuffer_Create();
    return window;
}

// Add a new line to window buffer at line 'line_num'.
int Window_NewLine(Window *window)
{
    CmacsBuffer *buff = window->buffer;

    if (buff->line_count >= MAX_LINES) return -1;

    CmacsBuffer_InsertLine(buff);
    StrBuffer_MoveAndAppendContents(buff->current_line->prev, buff->current_line, window->cursor.column);

    window->cursor.line++;
    window->cursor.column = 0;
    return 0;
}

int Window_RemoveLine(Window *window)
{
    CmacsBuffer *buff = window->buffer;
    
    int bytes_copied = 0;
    if (buff->current_line->prev)
        bytes_copied = StrBuffer_CopyAndAppendContents(buff->current_line, buff->current_line->prev, 0);
    
    CmacsBuffer_RemoveLine(buff);
    
    window->cursor.line--;
    window->cursor.column = buff->current_line->str_length - bytes_copied;
    return 0;
}

int Window_CursorUp(Window *window)
{
    CmacsBuffer *buff = window->buffer;

    if (buff->current_line && buff->current_line->prev) {
        buff->current_line = buff->current_line->prev;
        window->cursor.line--;
    }

    window->cursor.column = buff->current_line->str_length; // Improvments needed. 
    return 0; 
}

int Window_CursorDown(Window *window)
{
    CmacsBuffer *buff = window->buffer;

    if (buff->current_line && buff->current_line->next) {
        buff->current_line = buff->current_line->next;
        window->cursor.line++;
    }

    window->cursor.column = buff->current_line->str_length; // Improments needed.                 
    return 0;
}

int Window_CursorLeft(Window *window)
{
    CmacsBuffer *buff = window->buffer;

    if (window->cursor.column < 1) {
        if (buff->current_line->prev) {
            buff->current_line = buff->current_line->prev;
            window->cursor.line--;
            window->cursor.column = buff->current_line->str_length;
        }
        return 0;
    }
    window->cursor.column--;
    return 0;
}

int Window_CursorRight(Window *window)
{
    CmacsBuffer *buff = window->buffer;

    if (window->cursor.column >= buff->current_line->str_length) {
        if (buff->current_line->next) {
            buff->current_line = buff->current_line->next;
            window->cursor.line++;
            window->cursor.column = 0;
        }
        return 0;
    };
    window->cursor.column++;
    return 0;
}


int Window_OpenFile(Window *window, const char *filepath)
{
    // Should really probably ensure the window is clean here first...

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filepath, "r");
    if (fp == NULL) {
        perror("Failed to open file");
        return -1;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        int i = 0;
        char *c = line;
        while (*c)
        {    
            if (*c == '\n')
                Window_NewLine(window);
            else {
                StrBuffer_AddChar(window->buffer->current_line, *c, i);
                window->cursor.column++;
            }
            c++; i++;
        }
    }

    fclose(fp);
    if (line)
        free(line);

    return 0;
}


static SDL_Surface *text_buffer_surface = NULL;
static SDL_Texture *text_buffer_texture = NULL;
static SDL_Rect type_text_rect = {0};

// 11 to allow space for MAXINT digits...
static char line_num_buffer[11];
static SDL_Texture *line_num_texture = NULL;
static SDL_Surface *line_num_surface = NULL;

static SDL_Rect cursor_rect = {
    0, 0, 8, 20,
};

static SDL_Color white = {255, 255, 255, 255};

int Window_Render(Window *window, SDL_Renderer *renderer)
{
    // Still needs a lot of polishing...

    CmacsBuffer *buff = window->buffer;

    int i = 0;
    StrBuffer *line = buff->head;
    while (line) {
        
        // Draw line number...
        if (line_num_texture) SDL_DestroyTexture(line_num_texture);

        sprintf(line_num_buffer, "%d", i + 1);
        line_num_surface = TTF_RenderText_Blended(
            fonts.font_mono_regular, 
            line_num_buffer,
            white
        );

        line_num_texture = SDL_CreateTextureFromSurface(renderer, line_num_surface);
        
        type_text_rect.y = i * (line_num_surface->h + 2);
        type_text_rect.h = line_num_surface->h;
        
        type_text_rect.w = 40;
        SDL_RenderCopy(renderer, textures.label_background, NULL, &type_text_rect);
        type_text_rect.w = line_num_surface->w;
        SDL_RenderCopy(renderer, line_num_texture, NULL, &type_text_rect);
        SDL_FreeSurface(line_num_surface);

        // Draw line text...
        if (line->str_length) {
            if (text_buffer_texture) SDL_DestroyTexture(text_buffer_texture);

            text_buffer_surface = TTF_RenderText_Blended(
                fonts.font_mono_regular, 
                line->data,
                white
            );

            if (!text_buffer_surface) {
                SDL_LogError(0, "Failed to compile 'text_buffer_surface'!");
                return -1;
            }

            type_text_rect.x = 50;
            type_text_rect.w = text_buffer_surface->w;
            type_text_rect.h = text_buffer_surface->h;

            text_buffer_texture = SDL_CreateTextureFromSurface(renderer, text_buffer_surface);
            SDL_FreeSurface(text_buffer_surface);
            SDL_RenderCopy(renderer, text_buffer_texture, NULL, &type_text_rect);
        }
        type_text_rect.x = 0;
        type_text_rect.y = 0;

        line = line->next;
        i++;
    }

    // Draw Cursor...
    cursor_rect.x = (window->cursor.column * cursor_rect.w) + 50;
    cursor_rect.y = window->cursor.line * (type_text_rect.h + 2);
    SDL_SetRenderDrawColor(renderer, 225, 225, 255, 90);        
    SDL_RenderFillRect(renderer, &cursor_rect);
    return 0;
}

void Window_Destroy(Window *window)
{
    CmacsBuffer_Destroy(window->buffer);
    free(window);
}
