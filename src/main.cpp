#include "game.h"
#include "ui.h"
#include "user_input.h"

void gameLoop(ui_state *ui, game_state *game, user_input *input)
{
    bool running = true;

    while (running)
    {
        resetUserInput(input);

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                handleInput(input, &event.key.keysym.sym);
            default: break;
            }
        }

        gameTick(game, input, SDL_GetTicks());
        render(ui, game);
        SDL_Delay(16);
    }
}

int main(void)
{
    srand(time(NULL));

    ui_state ui;
    initUI(&ui);

    game_state game;
    initGame(&game);

    user_input input;

    gameLoop(&ui, &game, &input);

    destroyUI(&ui);
}
