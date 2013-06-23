
#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"

typedef enum {
  player_none = -1,
  player_1 = 0,
  player_2 = 1,
  player_num = 2
} Player;

typedef struct Game Game;

typedef struct Game_vtable {
  int (*validMove)(Game *self, BoardCoord coord);
  int (*movesPossible)(Game *self);
  Player (*winner)(Game *self);
  double (*evalPosition)(Game *self);
} Game_vtable;

struct Game {
  Player actPlayer;
  Board board;
  const Game_vtable *vtable;
};

extern const BoardCell game_playerCells[player_num];
extern const char *game_playerNames[player_num];
extern Game game_create(Board board);
extern Game game_copy(Game *self);
extern void game_switchPlayer(Game *self);
extern BoardCell game_actPlayerCell(Game *self);

#endif
