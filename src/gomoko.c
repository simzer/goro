/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>

#include "gomoko.h"
#include "minimax.h"
#include "boarditerator.h"


static void countRowsInAllDirection(Gomoko *self, BoardCoord coord,
                                    int friendCounters[], int enemyCounters[]);
static void countRow(Gomoko *self, BoardCoord coord, Direction direction,
                     int *friends, int *enemies);
static int gomokoMoveWorthChecking(Gomoko *self, BoardCoord coord);
static int validGomokoMove(Gomoko *self, BoardCoord coord);
static int gomokoGameOver(Gomoko *self);
static PlayerId gomokoWinner(Gomoko *self);
static double evalGomokoPosition(Gomoko *self);
static Gomoko *copyGomokoGame(Gomoko *self);

static const GameVirtualTable gomokoVirtualtable = {
  &gameMove,
  &validGomokoMove,
  &gomokoMoveWorthChecking,
  &gomokoGameOver,
  &gomokoWinner,
  &evalGomokoPosition,
  &copyGomokoGame
};

Gomoko createGomoko(Board board)
{
  Gomoko self;
  self.game = createGame(board);
  self.game.vtable = &gomokoVirtualtable;
  self.winnerRowSize = 5;
  return self;
}

static int gomokoMoveWorthChecking(Gomoko *self, BoardCoord coord)
{
  return boardCellHasNeighbour(&((Game *)self)->board, coord);
}

static int validGomokoMove(Gomoko *self, BoardCoord coord)
{
  return getBoardCell(&(((Game *)self)->board), coord) == emptyBoardCell;
}

static int gomokoGameOver(Gomoko *self)
{
  return    !boardHasEmptyCell(&((Game *)self)->board)
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
  PlayerId player;
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
    double score;
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
    double weight = 1;
    int distance;
    for(distance = 1; distance < self->winnerRowSize; distance++) {
      score -= enemyCounters[distance] * weight;
      weight *= ((Game *)self)->board.height * ((Game *)self)->board.width;
      score += friendCounters[distance] * weight;
      weight *= ((Game *)self)->board.height * ((Game *)self)->board.width;
    }
    return score;
  }
}

static void countRowsInAllDirection(Gomoko *self, BoardCoord coord,
                                    int friendCounters[], int enemyCounters[])
{
  Direction direction;
  for(direction =  halfRoundDirectionBegin;
      direction <= roundDirectionEnd;
      direction++)
  {
    int friends = 0;
    int enemies = 0;
    countRow(self, coord, direction, &friends, &enemies);
    if ((enemies == 0) && (friends > 0)) {
      friendCounters[friends]++;
    }
    if ((friends == 0) && (enemies > 0)) {
      enemyCounters[enemies]++;
    }
  }
}

static void countRow(Gomoko *self, BoardCoord coord, Direction direction,
                     int *friends, int *enemies)
{
  int distance;
  for (distance = 0; distance < self->winnerRowSize; distance++)
  {
    BoardCoord neighbour =
      getBoardCoordNeighbour(&coord, direction, distance);
    BoardCell cell = getBoardCell(&((Game *)self)->board, neighbour);
    if (cell == gamePlayerCells[((Game *)self)->actualPlayer]) {
      (*friends)++;
    } else if (   cell != emptyBoardCell
               && cell != invalidBoardCell) {
      (*enemies)++;
    }
  }
}
