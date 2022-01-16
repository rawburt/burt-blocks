#ifndef USER_INPUT_H
#define USER_INPUT_H

#include "SDL2/SDL.h"

struct user_input
{
    int move_left;
    int move_right;
    int soft_drop;
    int hard_drop;
    int rotate_right;
    int rotate_left;
    int restart;
};

void resetUserInput(user_input & input);
void handleInput(user_input & input, SDL_Keycode *code);

#endif // USER_INPUT_H
