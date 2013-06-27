/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <string.h>

#include "game.h"
#include "boarditerator.h"

static int gomoko_validMove(Game *self, BoardCoord coord);
static int gomoko_movesPossible(Game *self);
static Player gomoko_winner(Game *self);
static double gomoko_evalPosition(Game *self);

static const BoardSize gomoko_winnerRowSize = 5;

static const Game_vtable gomoko_vtable = {
  &gomoko_validMove,
  &gomoko_movesPossible,
  &gomoko_winner,
  &gomoko_evalPosition
};

Game gomoko_create(Board board)
{
  Game self = game_create(board);
  self.vtable = &gomoko_vtable;
  return self;
}

static int gomoko_validMove(Game *self, BoardCoord coord)
{
  return board_getCell(&(self->board), coord) == boardCell_empty;
}

static int gomoko_movesPossible(Game *self)
{
  return board_isThereEmptyCell(&self->board);
}

static Player gomoko_winner(Game *self)
{
  Player player;
  for(player = player_1; player < player_num; player++) {
    BoardIterator iter = boardIterator_create(&self->board);
    for(;boardIterator_next(&iter);) {
      BoardDirection direction;
      for(direction =  boardDirection_halfRoundBegin;
          direction <= boardDirection_roundEnd;
          direction++)
      {
        BoardSize distance;
        int winnerAmountInARow = 1;
        for(distance = 0;
            distance < gomoko_winnerRowSize;
            distance++)
        {
          BoardCoord neighbour =
            boardCoord_neighbour(&iter.coord, direction, distance);
          winnerAmountInARow &=
            board_getCell(&(self->board), neighbour) == game_playerCells[player];
        }
        if (winnerAmountInARow) return(player);
      }
    }
  }
  return(player_none);
}

static double gomoko_evalPosition(Game *self)
{
  enum { friend = 0, enemy = 1 };
  double score;
  int counters[gomoko_winnerRowSize][2];
  int side;
  BoardIterator iterator = boardIterator_create(&(self->board));
  memset(counters, 0, gomoko_winnerRowSize * 2 * sizeof(counters[0]));
  // Count connected sections (xx, xxx, xxxx, xxxxx)
  while(boardIterator_next(&iterator))
  {
    BoardDirection direction;
    for(direction =  boardDirection_halfRoundBegin;
        direction <= boardDirection_roundEnd;
        direction++)
    {
      int friends = 0;
      int enemies = 0;
      int distance;
      for (distance = 0; distance < gomoko_winnerRowSize; distance++)
      {
        BoardCoord neighbour =
          boardCoord_neighbour(&iterator.coord, direction, distance);
        BoardCell cell = board_getCell(&self->board, neighbour);
        if (cell == game_playerCells[self->actualPlayer]) {
          friends++;
        } else if (   cell != boardCell_empty
                   && cell != boardCell_invalid) {
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
  for(distance = 3; distance < gomoko_winnerRowSize; distance++) {
    score -= counters[distance][friend] * weight;
    weight *= self->board.height * self->board.width;
    score += counters[distance][enemy] * weight;
    weight *= self->board.height * self->board.width;
  }
  return score;
}

