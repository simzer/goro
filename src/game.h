/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"
#include "boarditerator.h"

typedef enum {
  noPlayer = -1,
  firstPlayer = 0,
  secondPlayer = 1,
  numberOfPlayers = 2
} Player;

typedef struct Game Game;

typedef struct GameVirtualTable {
  int (*validMove)(Game *self, BoardCoord coord);
  int (*movesPossible)(Game *self);
  Player (*winner)(Game *self);
  double (*evalPosition)(Game *self);
} GameVirtualTable;

struct Game {
  Player actualPlayer;
  Board board;
  const GameVirtualTable *vtable;
};

extern const BoardCell gamePlayerCells[numberOfPlayers];
extern const char *gamePlayerNames[numberOfPlayers];
extern Game createGame(Board board);
extern Game copyGame(Game *self);
extern void switchGamePlayer(Game *self);
extern BoardCell actualGamePlayerCell(Game *self);
extern int nextValidGameMove(Game *self, BoardIterator *iterator);

#endif
