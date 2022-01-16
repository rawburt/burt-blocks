#include "ui.h"
#include <cassert>

#define SIZE 30
#define WHITE                                                                  \
  { 255, 255, 255, 255 }

static void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                       const int x, const int y, const int align = ALIGN_LEFT) {
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, WHITE);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  SDL_Rect dest = {x, y, 0, 0};

  SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

  if (align == ALIGN_CENTER) {
    dest.x = (WINDOW_WIDTH / 2) - (dest.w / 2);
  } else if (align == ALIGN_RIGHT) {
    dest.x = WINDOW_WIDTH - dest.w - x;
  }

  SDL_RenderCopy(renderer, texture, NULL, &dest);

  SDL_DestroyTexture(texture);
}

static void renderGrid(ui_state &ui, const grid g) {
  SDL_Rect dest = {SIZE, SIZE, SIZE, SIZE};

  for (auto y = 10; y < 30; y++) {
    for (auto x = 0; x < 10; x++) {
      dest.x = SIZE + (x * SIZE);
      dest.y = SIZE * 2 + (y - 10) * SIZE;

      SDL_RenderCopy(ui.renderer, ui.textures[TEXTURE_BG], NULL, &dest);

      if (g[y][x] != GRID_EMPTY_CELL) {
        SDL_RenderCopy(ui.renderer, ui.textures[TEXTURE_CELL], NULL, &dest);
      }
    }
  }
}

static void renderTetrominos(ui_state &ui, const game_state &game) {
  SDL_Rect dest = {SIZE, SIZE, SIZE, SIZE};

  for (auto y = 0; y < 4; y++) {
    for (auto x = 0; x < 4; x++) {
      if (checkCell(game.tid, game.rid, x, y)) {
        if (game.fy + y >= 10) {
          dest.x = SIZE + (x + game.fx) * SIZE;
          dest.y = SIZE * 2 + (y + game.fy - 10) * SIZE;
          SDL_RenderCopy(ui.renderer, ui.textures[TEXTURE_CELL], NULL, &dest);
        }
        if (game.gy + y >= 10) {
          dest.x = SIZE + (x + game.fx) * SIZE;
          dest.y = SIZE * 2 + (y + game.gy - 10) * SIZE;
          SDL_RenderCopy(ui.renderer, ui.textures[TEXTURE_CELLG], NULL, &dest);
        }
      }
    }
  }
}

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *file_path,
                         const int alpha) {
  SDL_Surface *surface = IMG_Load(file_path);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  assert(texture);

  SDL_FreeSurface(surface);

  if (alpha < 255) {
    SDL_SetTextureAlphaMod(texture, alpha);
  }

  return texture;
}

void initUI(ui_state &ui) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    exit(1);
  }

  if (TTF_Init() != 0) {
    SDL_Log("Unable to initialize TTF");
    exit(1);
  }

  ui.window = SDL_CreateWindow("Burt Blocks", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, 360, 690, 0);

  if (!ui.window) {
    SDL_Log("Could not create window: %s", SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

  ui.renderer = SDL_CreateRenderer(
      ui.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!ui.renderer) {
    SDL_Log("Could not create renderer: %s", SDL_GetError());
    exit(1);
  }

  IMG_Init(IMG_INIT_PNG);

  ui.textures[TEXTURE_CELL] = loadTexture(ui.renderer, "./assets/cell.png");
  ui.textures[TEXTURE_CELLG] =
      loadTexture(ui.renderer, "./assets/cell.png", 125);
  ui.textures[TEXTURE_BG] = loadTexture(ui.renderer, "./assets/cell-bg.png");
  ui.fonts[FONT_MAIN] = TTF_OpenFont("./assets/FFFFORWA.TTF", 12);
  ui.fonts[FONT_TITLE] = TTF_OpenFont("./assets/FFFFORWA.TTF", 36);
}

void destroyUI(ui_state &ui) {
  for (auto t : ui.textures) {
    SDL_DestroyTexture(t);
  }

  for (auto f : ui.fonts) {
    TTF_CloseFont(f);
  }

  IMG_Quit();
  SDL_DestroyRenderer(ui.renderer);
  SDL_DestroyWindow(ui.window);
  SDL_Quit();
}

void render(ui_state &ui, const game_state &game) {
  SDL_SetRenderDrawColor(ui.renderer, 32, 32, 32, 255);
  SDL_RenderClear(ui.renderer);

  switch (game.status) {
  case WAITING: {
    renderText(ui.renderer, ui.fonts[FONT_TITLE], "BURT BLOCKS", 0, 200,
               ALIGN_CENTER);
    renderText(ui.renderer, ui.fonts[FONT_MAIN], "Press R to play.", 0, 290,
               ALIGN_CENTER);
  } break;
  case PLAYING: {
    renderGrid(ui, game.matrix);
    renderTetrominos(ui, game);

    sprintf(ui.level_text, "LEVEL - %03d", game.level);
    renderText(ui.renderer, ui.fonts[FONT_MAIN], ui.level_text, 30, 30);

    sprintf(ui.score_text, "SCORE - %03d", game.lines);
    renderText(ui.renderer, ui.fonts[FONT_MAIN], ui.score_text, 30, 30,
               ALIGN_RIGHT);
  } break;
  case OVER: {
    renderText(ui.renderer, ui.fonts[FONT_TITLE], "GAME OVER", 0, 200,
               ALIGN_CENTER);
    renderText(ui.renderer, ui.fonts[FONT_MAIN], "Press R to restart.", 0, 290,
               ALIGN_CENTER);
  } break;
  }

  SDL_RenderPresent(ui.renderer);
}
