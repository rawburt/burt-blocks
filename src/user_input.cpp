#include "user_input.h"

void resetUserInput(user_input & input)
{
    input.move_left = 0;
    input.move_right = 0;
    input.soft_drop = 0;
    input.hard_drop = 0;
    input.rotate_right = 0;
    input.rotate_left = 0;
    input.restart = 0;
}

void handleInput(user_input & input, SDL_Keycode *code)
{
    switch (*code)
    {
    case SDLK_LEFT:
        input.move_left = 1;
        break;
    case SDLK_RIGHT:
        input.move_right = 1;
        break;
    case SDLK_DOWN:
        input.soft_drop = 1;
        break;
    case SDLK_SPACE:
        input.hard_drop = 1;
        break;
    case SDLK_UP:
        input.rotate_right = 1;
        break;
    case SDLK_z:
        input.rotate_left = 1;
        break;
    case SDLK_r:
        input.restart = 1;
    default: break;
    }
}
