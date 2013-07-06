/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GAME_H__
#define __GAME_H__

#include "gamemove.h"
#include "board.h"

typedef enum {
  firstPlayer = 0,
  secondPlayer = 1,
  noPlayer,
  numberOfPlayers = 2
} PlayerId;

typedef enum {
  notStartedStatus,
  activeStatus,
  finishedStatus
} GameStatus;

typedef struct Game Game;

typedef struct GameVirtualTable {
  int (*move)(Game *self, GameMove move);
  int (*validMove)(Game *self, GameMove move);
  int (*moveWorthChecking)(Game *self, GameMove move);
  int (*over)(Game *self);
  PlayerId (*winner)(Game *self);
  double (*evalPosition)(Game *self);
  Game *(*copy)(Game *self);
  double (*score)(Game *self);
  void (*printGameStatus)(Game *self);
} GameVirtualTable;

struct Game {
  GameStatus status;
  PlayerId actualPlayer;
  Board board;
  GameMove lastMove;
  PlayerId winner;
  const GameVirtualTable *vtable;
};

extern const BoardCell gamePlayerCells[numberOfPlayers + 1];
extern const char *gamePlayerNames[numberOfPlayers];
extern Game createGame(Board board);
extern Game *copyGame(Game *self);
extern void destructGame(Game *self);
extern void startGame(Game *self);
extern int genericGameMove(Game *self, GameMove move);
extern BoardCell actualGamePlayerCell(Game *self);
extern PlayerId otherGamePlayer(Game *self);
extern PlayerId actualGamePlayer(Game *self);
extern void printGameOverInfo(Game *self);
extern void printGameStatus(Game *self);

extern int nextValidGameMove(Game *self, MoveIterator *iterator);
extern int nextMoveWorthChecking(Game *self, MoveIterator *iterator);

extern int validGameMove(Game *self, GameMove move);
extern int genericGameOver(Game *self);

#endif
