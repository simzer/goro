/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>

#include "gomoko.h"
#include "minimax.h"
#include "boarditerator.h"



static void countRow(Gomoko *self, BoardCoord coord, Direction direction,
                     int *friends, int *enemies);
static int gomokoMoveWorthChecking(Gomoko *self, BoardCoord coord);
static int validGomokoMove(Gomoko *self, BoardCoord coord);
static int gomokoGameOver(Gomoko *self);
static PlayerId gomokoWinner(Gomoko *self);
static double evalGomokoPosition(Gomoko *self);
static Gomoko *copyGomokoGame(Gomoko *self);

static const GameVirtualTable gomokoVirtualtable = {
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
  for(player = firstPlayer; player < numberOfPlayers; player++) {
    BoardIterator iterator = createBoardIterator(&((Game *)self)->board);
    while(!boardIteratorFinished(&iterator)) {
      Direction direction;
      for(direction =  halfRoundDirectionBegin;
          direction <= roundDirectionEnd;
          direction++)
      {
        BoardSize distance;
        int winnerAmountInARow = 1;
        for(distance = 0;
            distance < self->winnerRowSize;
            distance++)
        {
          BoardCoord neighbour =
            getBoardCoordNeighbour(&iterator.coord, direction, distance);
          winnerAmountInARow &=
            getBoardCell(&(((Game *)self)->board), neighbour) == gamePlayerCells[player];
        }
        if (winnerAmountInARow) return(player);
      }
    }
  }
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
//    return(0);
    enum { friend = 0, enemy = 1 };
    double score;
    int counters[self->winnerRowSize][2];
    int side;
    BoardIterator iterator = createBoardIterator(&(((Game *)self)->board));
    memset(counters, 0, self->winnerRowSize * 2 * sizeof(counters[0]));
    // Count connected sections (xx, xxx, xxxx, xxxxx)
    while(!boardIteratorFinished(&iterator))
    {
      Direction direction;
      for(direction =  halfRoundDirectionBegin;
          direction <= roundDirectionEnd;
          direction++)
      {
        int friends = 0;
        int enemies = 0;
        countRow(self, iterator.coord, direction, &friends, &enemies);
        if ((enemies == 0) && (friends > 0)) {
          counters[friends][friend]++;
        }
        if ((friends == 0) && (enemies > 0)) {
          counters[enemies][enemy]++;
        }
      }
    }
    double weight = 1;
    int distance;
    for(distance = 1; distance < self->winnerRowSize; distance++) {
      score -= counters[distance][enemy] * weight;
      weight *= ((Game *)self)->board.height * ((Game *)self)->board.width;
      score += counters[distance][friend] * weight;
      weight *= ((Game *)self)->board.height * ((Game *)self)->board.width;
    }
    return score;
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
