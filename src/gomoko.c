/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <string.h>

#include "game.h"
#include "boarditerator.h"

static int validGomokoMove(Game *self, BoardCoord coord);
static int possibleGomokoMoves(Game *self);
static Player gomokoWinner(Game *self);
static double evalGomokoPosition(Game *self);

static const BoardSize gomokoWinnerRowSize = 5;

static const GameVirtualTable gomokoVirtualtable = {
  &validGomokoMove,
  &possibleGomokoMoves,
  &gomokoWinner,
  &evalGomokoPosition
};

Game createGomoko(Board board)
{
  Game self = createGame(board);
  self.vtable = &gomokoVirtualtable;
  return self;
}

static int validGomokoMove(Game *self, BoardCoord coord)
{
  return getBoardCell(&(self->board), coord) == emptyBoardCell;
}

static int possibleGomokoMoves(Game *self)
{
  return boardHasEmptyCell(&self->board);
}

static Player gomokoWinner(Game *self)
{
  Player player;
  for(player = firstPlayer; player < numberOfPlayers; player++) {
    BoardIterator iterator = createBoardIterator(&self->board);
    while(!boardIteratorFinished(&iterator)) {
      BoardDirection direction;
      for(direction =  halfRoundDirectionBegin;
          direction <= roundDirectionEnd;
          direction++)
      {
        BoardSize distance;
        int winnerAmountInARow = 1;
        for(distance = 0;
            distance < gomokoWinnerRowSize;
            distance++)
        {
          BoardCoord neighbour =
            getBoardCoordNeighbour(&iterator.coord, direction, distance);
          winnerAmountInARow &=
            getBoardCell(&(self->board), neighbour) == gamePlayerCells[player];
        }
        if (winnerAmountInARow) return(player);
      }
    }
  }
  return(noPlayer);
}

static double evalGomokoPosition(Game *self)
{
  enum { friend = 0, enemy = 1 };
  double score;
  int counters[gomokoWinnerRowSize][2];
  int side;
  BoardIterator iterator = createBoardIterator(&(self->board));
  memset(counters, 0, gomokoWinnerRowSize * 2 * sizeof(counters[0]));
  // Count connected sections (xx, xxx, xxxx, xxxxx)
  while(!boardIteratorFinished(&iterator))
  {
    BoardDirection direction;
    for(direction =  halfRoundDirectionBegin;
        direction <= roundDirectionEnd;
        direction++)
    {
      int friends = 0;
      int enemies = 0;
      int distance;
      for (distance = 0; distance < gomokoWinnerRowSize; distance++)
      {
        BoardCoord neighbour =
          getBoardCoordNeighbour(&iterator.coord, direction, distance);
        BoardCell cell = getBoardCell(&self->board, neighbour);
        if (cell == gamePlayerCells[self->actualPlayer]) {
          friends++;
        } else if (   cell != emptyBoardCell
                   && cell != invalidBoardCell) {
          enemies++;
        }
      }
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
  for(distance = 3; distance < gomokoWinnerRowSize; distance++) {
    score -= counters[distance][friend] * weight;
    weight *= self->board.height * self->board.width;
    score += counters[distance][enemy] * weight;
    weight *= self->board.height * self->board.width;
  }
  return score;
}

