#include "game.h"

#include <cstdint>
#include <iostream>
#include <vector>

static bool
is_input_set(int x)
{
  return x == 1;
}

static const std::uint16_t tetromino_data[7][4] = {
  /* I tetromino */
  { 0b0000111100000000,
    0b0010001000100010,
    0b0000000011110000,
    0b0100010001000100 },
  /* O tetromino */
  { 0b0110011000000000,
    0b0110011000000000,
    0b0110011000000000,
    0b0110011000000000 },
  /* T tetromino */
  { 0b0100111000000000,
    0b0100011001000000,
    0b0000111001000000,
    0b0100110001000000 },
  /* J tetromino */
  { 0b1000111000000000,
    0b0110010001000000,
    0b0000111000100000,
    0b0100010011000000 },
  /* L tetromino */
  { 0b0010111000000000,
    0b0100010001100000,
    0b0000111010000000,
    0b1100010001000000 },
  /* S tetromino */
  { 0b0110110000000000,
    0b0100011000100000,
    0b0000011011000000,
    0b1000110001000000 },
  /* Z tetromino */
  { 0b1100011000000000,
    0b0010011001000000,
    0b0000110001100000,
    0b0100110010000000 }
};

static const int LEVEL_SPEED[15] = { 1000, 793, 618, 473, 355, 262, 190, 135,
                                     94,   64,  43,  28,  18,  11,  7 };

static int
dropSpeed(int level)
{
  if (level > 14) {
    return LEVEL_SPEED[14];
  }

  return LEVEL_SPEED[level];
}

static void
initGrid(grid g)
{
  for (auto y = 0; y < 30; y++) {
    for (auto x = 0; x < 10; x++) {
      g[y][x] = GRID_EMPTY_CELL;
    }
  }
}

/* fisher-yates because easy */
static void
shuffleBag(bag b)
{
  for (auto i = 6; i > 0; i--) {
    int j = i + rand() / (RAND_MAX / (7 - i) + 1);
    int t = b[j];
    b[j] = b[i];
    b[i] = t;
  }
}

static void
initQueue(game_state& g)
{
  g.b = 0;
  g.bid = 0;

  for (auto i = 0; i < 7; i++) {
    g.bags[0][i] = i;
    g.bags[1][i] = i;
  }

  shuffleBag(g.bags[0]);
  shuffleBag(g.bags[1]);
}

static int
randomTetromino(game_state& g)
{
  int tid = g.bags[g.b][g.bid];

  g.bid += 1;

  if (g.bid > 6) {
    shuffleBag(g.bags[g.b]);
    g.bid = 0;
    g.b = !g.b;
  }

  return tid;
}

static bool
playable(const grid g, const int x, const int y)
{
  return (x >= 0 && x < 10 && y >= 0 && y < 30) && g[y][x] == GRID_EMPTY_CELL;
}

static bool
gridCollision(const grid g,
              const int tid,
              const int rid,
              const int x,
              const int y)
{
  for (auto cy = 0; cy < 4; cy++) {
    for (auto cx = 0; cx < 4; cx++) {
      if (checkCell(tid, rid, cx, cy) && !playable(g, cx + x, cy + y)) {
        return true;
      }
    }
  }

  return false;
}

static int
dropY(grid g, const int tid, const int rid, const int x, int y)
{
  while (!gridCollision(g, tid, rid, x, y)) {
    y += 1;
  }

  return (y - 1);
}

static void
generateTetromino(game_state& game)
{
  game.fx = 3;
  game.fy = 8;
  game.gy = 0;
  game.rid = 0;
  game.tid = randomTetromino(game);
}

static void
placeTetromino(game_state& game)
{
  for (auto y = 0; y < 4; y++) {
    for (auto x = 0; x < 4; x++) {
      if (checkCell(game.tid, game.rid, x, y)) {
        game.matrix[game.fy + y][game.fx + x] = game.tid;
      }
    }
  }
}

static bool
lineFull(const grid g, const int y)
{
  for (auto x = 0; x < 10; x++) {
    if (g[y][x] == GRID_EMPTY_CELL) {
      return false;
    }
  }

  return true;
}

static void
clearLine(grid g, const int line)
{
  for (auto x = 0; x < 10; x++) {
    g[line][x] = GRID_EMPTY_CELL;
  }

  if (line == 0) {
    return;
  }

  for (auto y = line; y > 0; y--) {
    for (auto x = 0; x < 10; x++) {
      g[y][x] = g[y - 1][x];
    }
  }
}

static void
tryMove(game_state& game, const int x, const int y)
{
  if (!gridCollision(
        game.matrix, game.tid, game.rid, game.fx + x, game.fy + y)) {
    game.fx += x;
    game.fy += y;
  }
}

static void
tryRotate(game_state& game, int r)
{
  r = game.rid + r;
  r = r > 3 ? 0 : (r < 0 ? 3 : r);

  if (!gridCollision(game.matrix, game.tid, r, game.fx, game.fy)) {
    game.rid = r;
  }
}

static void
hardDrop(game_state& game)
{
  game.fy = dropY(game.matrix, game.tid, game.rid, game.fx, game.fy);
  placeTetromino(game);
  generateTetromino(game);
}

bool
checkCell(const int tid, const int rid, const int x, const int y)
{
  return tetromino_data[tid][rid] & (1 << (15 - ((4 * y) + x)));
}

void
initGame(game_state& g, int status)
{
  g.status = status;
  g.level = 0;
  g.lines = 0;
  g.last_drop = 0;
  initGrid(g.matrix);
  initQueue(g);
  generateTetromino(g);
}

void
handleInput(game_state& game, user_input& input)
{
  if (is_input_set(input.move_left)) {
    tryMove(game, -1, 0);
  }
  if (is_input_set(input.move_right)) {
    tryMove(game, 1, 0);
  }
  if (is_input_set(input.soft_drop)) {
    tryMove(game, 0, 1);
  }
  if (is_input_set(input.rotate_left)) {
    tryRotate(game, -1);
  }
  if (is_input_set(input.rotate_right)) {
    tryRotate(game, 1);
  }
  if (is_input_set(input.hard_drop)) {
    hardDrop(game);
  }
}

void
gameTick(game_state& game, user_input& input, long ticks)
{
  if (game.status != PLAYING) {
    if (is_input_set(input.restart)) {
      if (game.status == OVER) {
        initGame(game, PLAYING);
      } else if (game.status == WAITING) {
        game.status = PLAYING;
      }
    }
    return;
  }

  handleInput(game, input);

  if (ticks - game.last_drop > dropSpeed(game.level)) {
    game.last_drop = ticks;
    game.fy += 1;

    if (gridCollision(game.matrix, game.tid, game.rid, game.fx, game.fy)) {
      game.fy -= 1;
      placeTetromino(game);
      generateTetromino(game);
    }
  }

  if (gridCollision(game.matrix, game.tid, game.rid, game.fx, game.fy)) {
    game.status = OVER;
    return;
  }

  std::vector<int> lines;

  for (auto y = 0; y < 30; y++) {
    if (lineFull(game.matrix, y)) {
      lines.push_back(y);
    }
  }

  if (!lines.empty()) {
    game.lines += lines.size();
    game.level = game.lines / 10;

    for (auto y : lines) {
      clearLine(game.matrix, y);
    }
  }

  game.gy = dropY(game.matrix, game.tid, game.rid, game.fx, game.fy);
}
