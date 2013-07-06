/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>

#include "gamemove.h"

const GameMove nullMove = { { -1, -1 }, invalidMove };

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

void printGameMove(GameMove self)
{
  GameMoveString string = gameMoveToString(self);
  printf("%s", string.chars);
}

GameMove stringToGameMove(GameMoveString string) {
  GameMove self;
  self.coord = stringToBoardCoord(createBoardCoordString(string.chars));
  if (!boardCoordsEqual(self.coord, nullBoardCoord)) {
    self.type = playMove;
  } else {
    if (strcmp(string.chars, "pass") == 0) self.type = passMove;
    else if (strcmp(string.chars, "res") == 0) self.type = resignMove;
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
  BoardCoordString string = boardCoordToString(move.coord);
  switch(move.type) {
    case playMove: return createGameMoveString(string.chars);
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
  };
  if(self->move.type == playMove) {
    if(!boardIteratorFinished(&self->boardIterator)) {
      self->move.coord = self->boardIterator.coord;
      return 0;
    } else {
      self->move = nullMove;
      return 1;
    }
  } else {
    return 0;
  }
}
