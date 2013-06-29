/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>

#include "game.h"

static void switchGamePlayer(Game *self);

const BoardCell gamePlayerCells[numberOfPlayers] = {
  blackBoardCell,
  whiteBoardCell
};

const char *gamePlayerNames[numberOfPlayers] = {
  "First player",
  "Second Player"
};

BoardCell actualGamePlayerCell(Game *self)
{
  return gamePlayerCells[self->actualPlayer];
}

Game createGame(Board board)
{
  Game self;
  self.actualPlayer = firstPlayer;
  self.board = board;
  return self;
}

Game *copyGame(Game *self)
{
  Game *copy = (Game *)malloc(sizeof(Game));
  *copy = *self;
  copy->board = copyBoard(&(self->board));
  return copy;
}

void destructGame(Game *self)
{
  destructBoard(&(self->board));
  free(self);
}

void gameMove(Game *self, BoardCoord coord)
{
  setBoardCell(&self->board, coord, actualGamePlayerCell(self));
  switchGamePlayer(self);
}

static void switchGamePlayer(Game *self)
{
  self->actualPlayer = (self->actualPlayer == firstPlayer)
                       ? secondPlayer
                       : firstPlayer;
}

int nextValidGameMove(Game *self, BoardIterator *iterator)
{
  int result;
  do {
    result = !boardIteratorFinished(iterator);
  } while(   (result == 1)
          && !self->vtable->validMove(self, iterator->coord));
  return result;
}

int nextMoveWorthChecking(Game *self, BoardIterator *iterator)
{
  int result;
  do {
    result = nextValidGameMove(self,iterator);
  } while(   (result == 1)
          && (self->vtable->moveWorthChecking != 0)
          && !self->vtable->moveWorthChecking(self, iterator->coord));
  return result;
}
