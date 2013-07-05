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

static int goMoveWorthChecking(Go *self, GameMove move);
static int validGoMove(Go *self, GameMove move);
static int goGameOver(Go *self);
static PlayerId goWinner(Go *self);
static double evalGoPosition(Go *self);
static Go *copyGoGame(Go *self);
static void goMove(Go *self, GameMove move);
static int repeatedGoPosition(Go *self);
static int goMoveDiedInstantly(Go *self, BoardCoord coord);
static int removeDeadGroups(Go *self, BoardCell cell);
static int countGoTerritory(Go *self, BoardCell cell);
static double goScore(Go *self);
static void printGoGameStatus(Go *self);

const BoardCoordString standardGoHandicaps[maxStandardGoHandicaps]
                                          [maxStandardGoHandicaps] =
{
  {},
  {{"D4"}, {"Q16"}},
  {{"D4"}, {"Q16"}, {"D16"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"K10"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"D10"}, {"Q10"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"D10"}, {"Q10"}, {"K10"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"D10"}, {"Q10"}, {"K4"}, {"K16"}},
  {{"D4"}, {"Q16"}, {"D16"}, {"Q4"}, {"D10"}, {"Q10"}, {"K4"}, {"K16"}, {"K10"}},
};

static const GameVirtualTable goVirtualtable = {
  &goMove,
  &validGoMove,
  &goMoveWorthChecking,
  &goGameOver,
  &goWinner,
  &evalGoPosition,
  &copyGoGame,
  &goScore,
  &printGoGameStatus
};

void setGoHandicap(Go *self, int handicap) {
  int i;
  assert(handicap >= 2 && handicap <= maxStandardGoHandicaps);
  for(i = 0; i < handicap; i++) {
    setBoardCell(&self->game.board,
                 stringToBoardCoord(standardGoHandicaps[handicap-1][i]),
                 gamePlayerCells[firstPlayer]);
  }
  self->game.actualPlayer = secondPlayer;
}

Go createGo(Board board)
{
  Go self;
  self.game = createGame(board);
  self.game.vtable = &goVirtualtable;
  self.history[0] = copyBoard(&board);
  self.history[1] = copyBoard(&board);
  self.komi = 6.5;
  self.captures[0] = 0;
  self.captures[1] = 0;
  return self;
}

static void goMove(Go *self, GameMove move)
{
  if (move.type == passMove && self->game.lastMove.type == passMove) {
    self->game.winner = goWinner(self); // implement a game finished flag instead
  } else {
    destructBoard(&self->history[1]);
    self->history[1] = self->history[0];
    self->history[0] = copyBoard(&self->game.board);
    genericGameMove(self, move);
    self->captures[otherGamePlayer(self)] +=
      removeDeadGroups(self, gamePlayerCells[actualGamePlayer(self)]);
    self->captures[actualGamePlayer(self)] +=
      removeDeadGroups(self, gamePlayerCells[otherGamePlayer(self)]);
  }
}

static int removeDeadGroups(Go *self, BoardCell cell) {
  int removedStones = 0;
  GroupList groupList = createGroupList(&(self->game.board));
  GroupIterator iterator = createGroupIterator(&groupList);
  while(getGroupsByColor(&iterator, cell)) {
    if (groupLiberties(iterator.group) == 0) {
      removedStones += iterator.group->size;
      removeGroup(iterator.group);
    }
  }
  destructGroupList(&groupList);
  return removedStones;
}

static Go *copyGoGame(Go *self)
{
  Go *copy = copyGame(self);
  copy = (Go *)realloc(copy, sizeof(Go));
  copy->history[0] = copyBoard(&self->history[0]);
  copy->history[1] = copyBoard(&self->history[1]);
  copy->komi = self->komi;
  copy->captures[0] = self->captures[0];
  copy->captures[1] = self->captures[1];
  return copy;
}

static int validGoMove(Go *self, GameMove move)
{
  if (!validGameMove(self, move)) {
    return(0);
  } else {
    Go *copy = copyGoGame(self);
    goMove(copy, move);
    if(move.type == playMove) {
      if (goMoveDiedInstantly(copy, move.coord)) return(0);
      if (repeatedGoPosition(copy)) return(0);
    }
    destructGame(copy);
  }
  return(1);
}

static int goMoveDiedInstantly(Go *self, BoardCoord coord) {
  return getBoardCell(&self->game.board, coord) == emptyBoardCell;
}

static int repeatedGoPosition(Go *self) {
  return boardEqual(&self->game.board,
                    &self->history[1]);
}

static int goMoveWorthChecking(Go *self, GameMove move)
{
  return move.type != resignMove;
}

static int goGameOver(Go *self)
{
  if(genericGameOver(self))
    return 1;
  else {
    int gameOver = 1;
    MoveIterator iterator = createMoveIterator(&(self->game.board));
    while(!moveIteratorFinished(&iterator)) {
      if(validGoMove(self, iterator.move)) gameOver = 0;
    }
    return gameOver;
  }
}

static int countGoTerritory(Go *self, BoardCell cell) {
  int territory = 0;
  GroupList groupList = createGroupList(&(self->game.board));
  GroupIterator iterator = createGroupIterator(&groupList);
  while(getTerritories(&iterator)) {
    if (territoryOwner(iterator.group) == cell) {
      territory += iterator.group->size;
    }
  }
  destructGroupList(&groupList);
  return territory;
}

static double goScore(Go *self)
{
  return   countGoTerritory(self, gamePlayerCells[firstPlayer])
         + self->captures[firstPlayer]
         - countGoTerritory(self, gamePlayerCells[secondPlayer])
         - self->captures[secondPlayer]
         - self->komi;
}

static PlayerId goWinner(Go *self)
{
  if(goGameOver(self)) {
    return (goScore(self) > 0) ? firstPlayer : secondPlayer;
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
    return (actualGamePlayer(self) == firstPlayer ? 1 : -1)
           * goScore(self);
  }
}

static void printGoGameStatus(Go *self)
{
  printGameStatus(self);
  printf("Black captured: %d, White captured: %d\n\n",
         self->captures[firstPlayer], self->captures[secondPlayer]);
}
