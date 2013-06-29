/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "game.h"
#include "minimax.h"
#include "go.h"
#include "boarditerator.h"
#include "group.h"

static int goMoveWorthChecking(Go *self, BoardCoord coord);
static int validGoMove(Go *self, BoardCoord coord);
static int goGameOver(Go *self);
static PlayerId goWinner(Go *self);
static double evalGoPosition(Go *self);
static Go *copyGoGame(Go *self);
static void goMove(Go *self, BoardCoord coord);
static int repeatedGoPosition(Go *self);
static int goMoveDiedInstantly(Go *self, BoardCoord coord);
static void removeDeadGroups(Go *self, BoardCell cell);
static int goTerritory(Go *self, BoardCell cell);

static const GameVirtualTable goVirtualtable = {
  &goMove,
  &validGoMove,
  &goMoveWorthChecking,
  &goGameOver,
  &goWinner,
  &evalGoPosition,
  &copyGoGame
};

Go createGo(Board board)
{
  Go self;
  self.game = createGame(board);
  self.game.vtable = &goVirtualtable;
  self.history[0] = 0;
  self.history[1] = 0;
  self.komi = 6.5;
  return self;
}

static void goMove(Go *self, BoardCoord coord)
{
  Go *stored = copyGoGame(self);
  destructGame(self->history[1]);
  self->history[1] = self->history[0];
  self->history[0] = stored;
  gameMove(self, coord);
  removeDeadGroups(self, gamePlayerCells[actualGamePlayer(self)]);
  removeDeadGroups(self, gamePlayerCells[otherGamePlayer(self)]);
}

static void removeDeadGroups(Go *self, BoardCell cell) {
  Group group;
  GroupIterator iterator = createGroupIterator(&(self->game.board));
  while(!groupIteratorFinished(&iterator)) {
    if (groupLiberties(iterator.group) == 0) {
      removeGroup(iterator.group);
    }
  }
}

static Go *copyGoGame(Go *self)
{
  Go *copy = copyGame(self);
  copy = (Go *)realloc(copy, sizeof(Go));
  copy->history[0] = self->history[0];
  copy->history[1] = self->history[1];
  copy->komi = self->komi;
  return copy;
}

static int validGoMove(Go *self, BoardCoord coord)
{
  if (getBoardCell(&self->game.board, coord) != emptyBoardCell) {
    return(0);
  } else {
    Go *copy = copyGoGame(self);
    goMove(copy, coord);
    if (goMoveDiedInstantly(copy, coord)) {
      return(0);
    }
    if (repeatedGoPosition(copy)) {
      return(0);
    }
  }
  return(1);
}

static int goMoveDiedInstantly(Go *self, BoardCoord coord) {
  return getBoardCell(&self->game.board, coord) == emptyBoardCell;
}

static int repeatedGoPosition(Go *self) {
  return boardEqual(self, self->history[1]);
}

static int goMoveWorthChecking(Go *self, BoardCoord coord)
{
  return 1;
}

static int goGameOver(Go *self)
{
  int gameOver = 1;
  BoardIterator iterator = createBoardIterator(&(self->game.board));
  while(!boardIteratorFinished(&iterator)) {
    if(validGoMove(self, iterator.coord)) {
      gameOver = 0;
    }
  }
  return gameOver;
}

static int goTerritory(Go *self, BoardCell cell) {
  // todo implement
  assert(0);
  return 0;
}

static PlayerId goWinner(Go *self)
{
  if(goGameOver(self)) {
    return (  goTerritory(self, gamePlayerCells[firstPlayer])
            > goTerritory(self, gamePlayerCells[secondPlayer]) + self->komi )
           ? firstPlayer : secondPlayer;
  } else {
    return(noPlayer);
  }
}

static double evalGoPosition(Go *self)
{
  PlayerId winner = goWinner(self);
  if (winner == actualGamePlayer(self)) {
    return miniMaxWinScore;
  } else if (winner == otherGamePlayer(self)) {
    return miniMaxLoseScore;
  } else {
    return(0);
  }
}

