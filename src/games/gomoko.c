/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>

#include "gomoko.h"
#include "minimax.h"
#include "boarditerator.h"

static double calculateScore(Gomoko *self,
                             int friendCounters[], int enemyCounters[]);
static void countRowsInAllDirection(Gomoko *self, BoardCoord coord,
                                    int friendCounters[], int enemyCounters[]);
static void countRow(Gomoko *self, BoardLineIterator *iterator,
                     int *friends, int *enemies);

static int gomokoMoveWorthChecking(Gomoko *self, GameMove move);
static int gomokoGameOver(Gomoko *self);
static PlayerId gomokoWinner(Gomoko *self);
static double evalGomokoPosition(Gomoko *self);
static Gomoko *copyGomokoGame(Gomoko *self);

static const GameVirtualTable gomokoVirtualtable = {
  &genericGameMove,
  &validGameMove,
  &gomokoMoveWorthChecking,
  &gomokoGameOver,
  &gomokoWinner,
  &evalGomokoPosition,
  &copyGomokoGame,
  0,
  &printGameStatus
};

Gomoko createGomoko(Board board)
{
  Gomoko self;
  self.game = createGame(board);
  self.game.vtable = &gomokoVirtualtable;
  self.winnerRowSize = 5;
  return self;
}

static int gomokoMoveWorthChecking(Gomoko *self, GameMove move)
{
  return move.type == playMove
         && boardCellHasNeighbour(&((Game *)self)->board, move.coord,
                                  eightNeighbourhood);
}

static int gomokoGameOver(Gomoko *self)
{
  return    gameOver(self)
         || (gomokoWinner(self) != noPlayer);
}

static Gomoko *copyGomokoGame(Gomoko *self)
{
  Gomoko *copy = copyGame(self);
  copy = (Gomoko *)realloc(copy, sizeof(Gomoko));
  copy->winnerRowSize = self->winnerRowSize;
  return copy;
}

static PlayerId gomokoWinner(Gomoko *self)
{
  if(self->game.winner != noPlayer) return self->game.winner;

  BoardIterator iterator = createBoardIterator(&((Game *)self)->board);
  int friendCounters[self->winnerRowSize];
  int enemyCounters[self->winnerRowSize];
  memset(friendCounters, 0, self->winnerRowSize * sizeof(int));
  memset(enemyCounters, 0, self->winnerRowSize * sizeof(int));
  while(!boardIteratorFinished(&iterator)) {
    countRowsInAllDirection(self, iterator.coord,
                            friendCounters, enemyCounters);
  }
  if(friendCounters[self->winnerRowSize-1] > 0)
    return actualGamePlayer(self);
  if(enemyCounters[self->winnerRowSize-1] > 0)
    return otherGamePlayer(self);
  return(noPlayer);
}

static double evalGomokoPosition(Gomoko *self)
{
  PlayerId winner = gomokoWinner(self);
  if (winner == ((Game *)self)->actualPlayer) {
    return miniMaxWinScore;
  } else if (winner != noPlayer) {
    return miniMaxLoseScore;
  } else {
    BoardIterator iterator = createBoardIterator(&(((Game *)self)->board));
    int friendCounters[self->winnerRowSize];
    int enemyCounters[self->winnerRowSize];
    memset(friendCounters, 0, self->winnerRowSize * sizeof(int));
    memset(enemyCounters, 0, self->winnerRowSize * sizeof(int));
    // Count connected sections (xx, xxx, xxxx, xxxxx)
    while(!boardIteratorFinished(&iterator))
    {
      countRowsInAllDirection(self, iterator.coord,
                              friendCounters, enemyCounters);
    }
    return calculateScore(self, friendCounters, enemyCounters);
  }
}

static double calculateScore(Gomoko *self,
                             int friendCounters[], int enemyCounters[])
{
  double score;
  double weight = 1;
  int distance;
  for(distance = 0; distance < self->winnerRowSize; distance++) {
    score -= enemyCounters[distance] * weight;
    weight *= ((Game *)self)->board.height * ((Game *)self)->board.width;
    score += friendCounters[distance] * weight;
    weight *= ((Game *)self)->board.height * ((Game *)self)->board.width;
  }
  return score;
}

static void countRowsInAllDirection(Gomoko *self, BoardCoord coord,
                                    int friendCounters[], int enemyCounters[])
{
  NeighbourIterator iterator =
    createNeighbourIterator(coord, eightNeighbourhood);

  while(getNeighbours(&iterator))
  {
    int friends = 0;
    int enemies = 0;
    BoardLineIterator lineIterator =
      createBoardLineIterator(coord, getNeighbourDirection(&iterator),
                              self->winnerRowSize);

    countRow(self, &lineIterator, &friends, &enemies);
    if ((enemies == 0) && (friends > 0)) {
      friendCounters[friends-1]++;
    }
    if ((friends == 0) && (enemies > 0)) {
      enemyCounters[enemies-1]++;
    }
  }
}

static void countRow(Gomoko *self, BoardLineIterator *iterator,
                     int *friends, int *enemies)
{
  while(getBoardLineCoords(iterator))
  {
    BoardCell cell = getBoardCell(&((Game *)self)->board, iterator->coord);
    if (cell == gamePlayerCells[actualGamePlayerCell(self)]) {
      (*friends)++;
    } else if (cell == gamePlayerCells[otherGamePlayer(self)]) {
      (*enemies)++;
    }
  }
}
