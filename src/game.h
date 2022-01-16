#ifndef GAME_H
#define GAME_H

#include "user_input.h"

#define GRID_EMPTY_CELL 9
#define GRID_IN_BOUNDS(x, y) (x >= 0 && x < 10 && y >= 0 && y < 30)

typedef int grid[30][10];

typedef int bag[7];

enum { WAITING, PLAYING, OVER };

struct game_state {
  int status;
  int tid;
  int rid;
  int fx;
  int fy;
  int gy;
  int level;
  int lines;
  long last_drop;
  grid matrix;
  bag bags[2];
  int b;
  int bid;
};

bool checkCell(const int tid, const int rid, const int x, const int y);
void initGame(game_state &g, int status = WAITING);
void gameTick(game_state &game, user_input &input, long ticks);

#endif // GAME_H
