/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <math.h>

#include "boarditerator.h"
#include "minimax.h"

static double searchMaxScore(const MiniMax *self,
                             GameMove *bestMove,
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
                             GameMove *bestMove,
                             Game *game,
                             int lookahead)
{
  double result;
  if (   !game->vtable->over(game)
      && (lookahead > 0))
  {
    double score;
    double maxScore = -INFINITY;
    GameMove maxScoredMove = *bestMove;
    MoveIterator iterator = createMoveIterator(&game->board);
    while(nextMoveWorthChecking(game, &iterator)) {
      GameMove ignoredMove;
      Game *nextGame = game->vtable->copy(game);
      nextGame->vtable->move(nextGame, iterator.move);
      score = - searchMaxScore(self, &ignoredMove, nextGame, lookahead-1);
      destructGame(nextGame);
      if (score >= maxScore) {
        maxScore = score;
        maxScoredMove = iterator.move;
      }
    }
    *bestMove = maxScoredMove;
    result = maxScore;
  } else {
    result = game->vtable->evalPosition(game);
  }
  return result;
}

GameMove getMiniMaxMove(MiniMax *self)
{
  MoveIterator iterator = createMoveIterator(&(((Player *)self)->game->board));
  GameMove move;
  nextValidGameMove(((Player *)self)->game, &iterator);
  move = iterator.move;
  searchMaxScore(self, &move, ((Player *)self)->game, self->lookahead);
  return(move);
}
