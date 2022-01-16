#ifndef UI_H
#define UI_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "game.h"

#define WINDOW_WIDTH 360
#define WINDOW_HEIGHT 690

enum { TEXTURE_CELL = 0, TEXTURE_CELLG, TEXTURE_BG, TEXTURE_MAX };

enum { FONT_MAIN = 0, FONT_TITLE, FONT_MAX };

enum { ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER };

struct ui_state {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *textures[TEXTURE_MAX];
  TTF_Font *fonts[FONT_MAX];
  char level_text[12];
  char score_text[12];
};

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *file_path,
                         const int alpha = 255);
void initUI(ui_state &ui);
void destroyUI(ui_state &ui);
void render(ui_state &ui, const game_state &game);

#endif // UI_H
