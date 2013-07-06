/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <math.h>

#include "boarditerator.h"
#include "minimax.h"

typedef struct ScoredMove {
  GameMove move;
  double score;
} ScoredMove;

static double getScoreOfMove(const MiniMax *self, Game *game, int lookahead);
static ScoredMove searchBestMove(const MiniMax *self, Game *game, int lookahead);

const double miniMaxWinScore = INFINITY;
const double miniMaxLoseScore = -INFINITY;

MiniMax createMiniMax(Game *game) {
  MiniMax self;
  self.player.getMove = &getMiniMaxMove;
  self.player.game = game;
  self.lookahead = 6;
  return self;
}

static GameMove firstValidGameMove(const MiniMax *self) {
  MoveIterator iterator = createMoveIterator(&(((Player *)self)->game->board));
  nextValidGameMove(((Player *)self)->game, &iterator);
  return iterator.move;
}

static ScoredMove searchBestMove(const MiniMax *self,
                                 Game *game,
                                 int lookahead)
{
  ScoredMove maxScoredMove;
  maxScoredMove.move = firstValidGameMove(self);
  maxScoredMove.score = -INFINITY;
  MoveIterator iterator = createMoveIterator(&game->board);
  while(nextMoveWorthChecking(game, &iterator)) {
    double score;
    Game *nextGame = game->vtable->copy(game);
    nextGame->vtable->move(nextGame, iterator.move);
    score = - getScoreOfMove(self, nextGame, lookahead-1);
    destructGame(nextGame);
    if (score >= maxScoredMove.score) {
      maxScoredMove.score = score;
      maxScoredMove.move = iterator.move;
    }
  }
  return maxScoredMove;
}

static double getScoreOfMove(const MiniMax *self,
                             Game *game,
                             int lookahead)
{
  double result;
  if (   !game->vtable->over(game)
      && (lookahead > 0))
  {
    ScoredMove bestMove = searchBestMove(self, game, lookahead);
    return bestMove.score;
  } else {
    result = game->vtable->evalPosition(game);
  }
  return result;
}

GameMove getMiniMaxMove(MiniMax *self)
{
  ScoredMove bestMove =
      searchBestMove(self, ((Player *)self)->game, self->lookahead);
  return(bestMove.move);
}
