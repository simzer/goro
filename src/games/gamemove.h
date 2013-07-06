/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GAMEMOVE_H__
#define __GAMEMOVE_H__

#include "boarditerator.h"

typedef enum {
  invalidMove,
  passMove,
  resignMove,
  playMove
} GameMoveType;

typedef struct GameMove {
  BoardCoord coord;
  GameMoveType type;
} GameMove;
extern const GameMove nullMove;
extern GameMove createGameMove(BoardCoord coord, GameMoveType type);
extern GameMove createPlayMove(BoardCoord coord);
extern GameMove createResignMove();
extern GameMove createPassMove();
extern int gameMoveEqual(GameMove self, GameMove res);
extern void printGameMove(GameMove self);

typedef struct GameMoveString {
  char chars[5];
} GameMoveString;
extern GameMoveString gameMoveToString(GameMove move);
extern GameMoveString createGameMoveString(char *string);
extern GameMove stringToGameMove(GameMoveString string);

typedef struct MoveIterator {
  BoardIterator boardIterator;
  GameMove move;
} MoveIterator;
extern MoveIterator createMoveIterator(Board *board);
extern int moveIteratorFinished(MoveIterator *self);

#endif
