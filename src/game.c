/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include "game.h"

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
  self.actualPlayer = noPlayer;
  self.board = board;
  return self;
}

Game copyGame(Game *self)
{
  Game game = *self;
  game.board = copyBoard(&(self->board));
  return(game);
}

void switchGamePlayer(Game *self)
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
