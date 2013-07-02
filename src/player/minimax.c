/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <math.h>

#include "boarditerator.h"
#include "minimax.h"

static BoardCoord getMiniMaxMove(MiniMax *self);

static double searchMaxScore(const MiniMax *self,
                             BoardCoord *coord,
                             Game *game,
                             int lookahead);

const double miniMaxWinScore = INFINITY;
const double miniMaxLoseScore = -INFINITY;

MiniMax createMiniMax(Game *game) {
  MiniMax self;
  self.player.getMove = &getMiniMaxMove;
  self.player.game = game;
  self.lookahead = 6;
  return self;
}

static double searchMaxScore(const MiniMax *self,
                             BoardCoord *bestMove,
                             Game *game,
                             int lookahead)
{
  double result;
  if (   !game->vtable->over(game)
      && (lookahead > 0))
  {
    double score;
    double maxScore = -INFINITY;
    BoardCoord maxScoredCoord = *bestMove;
    BoardIterator iterator = createBoardIterator(&game->board);
    while(nextMoveWorthChecking(game, &iterator)) {
      BoardCoord ignoredCoord;
      Game *nextGame = game->vtable->copy(game);
      nextGame->vtable->move(nextGame, iterator.coord);
      score = - searchMaxScore(self, &ignoredCoord, nextGame, lookahead-1);
      destructGame(nextGame);
      if (score >= maxScore) {
        maxScore = score;
        maxScoredCoord = iterator.coord;
      }
    }
    *bestMove = maxScoredCoord;
    result = maxScore;
  } else {
    result = game->vtable->evalPosition(game);
  }
  return result;
}

static BoardCoord getMiniMaxMove(MiniMax *self)
{
  BoardIterator iterator = createBoardIterator(&(((Player *)self)->game->board));
  BoardCoord coord;
  nextValidGameMove(((Player *)self)->game, &iterator);
  coord = iterator.coord;
  searchMaxScore(self, &coord, ((Player *)self)->game, self->lookahead);
  return(coord);
}
