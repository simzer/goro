/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>

#include "gamemove.h"

const GameMove nullMove = { nullBoardCoord, invalidMove };

GameMove createGameMove(BoardCoord coord, GameMoveType type)
{
  GameMove self;
  self.coord = coord;
  self.type = type;
  return self;
}

int gameMoveEqual(GameMove self, GameMove reference)
{
  return    (self.type == reference.type)
         && (self.type == playMove
             ? boardCoordsEqual(self.coord, reference.coord) : 1);
}

GameMove createPassMove()
{
  return createGameMove(nullBoardCoord, passMove);
}

GameMove createResignMove()
{
  return createGameMove(nullBoardCoord, resignMove);
}

GameMove createPlayMove(BoardCoord coord)
{
  GameMove self;
  self.coord = coord;
  self.type = playMove;
  return self;
}

GameMove stringToGameMove(char *string) {
  GameMove self;
  self.coord = stringToBoardCoord(string);
  if (!boardCoordsEqual(self.coord, nullBoardCoord)) {
    self.type = playMove;
  } else {
    if (strcmp(string, "pass") == 0) self.type = passMove;
    else if (strcmp(string, "res") == 0) self.type = resignMove;
    else self.type = invalidMove;
  }
  return self;
}

GameMoveString createGameMoveString(char *string) {
  GameMoveString self;
  snprintf(self.chars, sizeof(self.chars), "%s", string);
  return self;
}

GameMoveString gameMoveToString(GameMove move) {
  switch(move.type) {
    case playMove: return stringToBoardCoord(move.coord);
    case resignMove: return createGameMoveString("res");
    case passMove: return createGameMoveString("pass");
    default: return createGameMoveString("null");
  }
}

MoveIterator createMoveIterator(Board *board) {
  MoveIterator self;
  self.boardIterator = createBoardIterator(board);
  self.move = nullMove;
  return self;
}

int moveIteratorFinished(MoveIterator *self) {
  if(self->move.type < playMove) {
    self->move.type++;
    return 0;
  } else if(!boardIteratorFinished(&self->boardIterator)) {
    return 0;
  } else {
    self->move = nullMove;
    return 1;
  }
}
