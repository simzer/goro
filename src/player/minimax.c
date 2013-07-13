/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <math.h>

#include "boarditerator.h"
#include "minimax.h"

static double searchBestMove(const MiniMax *self,
                             Game *game,
                             int lookahead,
                             GameMove *bestMove,
                             double alpha,
                             double beta,
                             int maximizigPlayer);

const double miniMaxWinScore = INFINITY;
const double miniMaxLoseScore = -INFINITY;

MiniMax createMiniMax(Game *game) {
  MiniMax self;
  self.player.getMove = &getMiniMaxMove;
  self.player.game = game;
  self.lookahead = 10;
  return self;
}

static GameMove firstValidGameMove(const MiniMax *self) {
  MoveIterator iterator = createMoveIterator(&(((Player *)self)->game->board));
  nextValidGameMove(((Player *)self)->game, &iterator);
  return iterator.move;
}

static double searchBestMove(const MiniMax *self,
                             Game *game,
                             int lookahead,
                             GameMove *bestMove,
                             double alpha,
                             double beta,
                             int maximizigPlayer)
{
  if (   game->vtable->over(game)
      || lookahead == 0)
  {
    return game->vtable->evalPosition(game);
  } else {
    MoveIterator iterator = createMoveIterator(&game->board);
    while(nextMoveWorthChecking(game, &iterator)) {
      double score;
      GameMove ignoredMove;
      Game *nextGame = game->vtable->copy(game);
      nextGame->vtable->move(nextGame, iterator.move);
      score = searchBestMove(self, nextGame, lookahead-1, &ignoredMove,
                             alpha, beta, !maximizigPlayer);
      destructGame(nextGame);

{
int i;
for (i = 9; i >= lookahead; i--) printf ("          ");
printf ("%d,%d (%f,%f,%f)\n",
    iterator.move.coord.row,iterator.move.coord.col,
    alpha,beta,score);
}

      if(maximizigPlayer) {
        if (score >= alpha) {
          alpha = score;
          *bestMove = iterator.move;
        }
        if (alpha >= beta) {
          break;
        }
      } else {
        if (score <= beta) {
          beta = score;
          *bestMove = iterator.move;
        }
        if (beta <= alpha) {
          break;
        }
      }
    }

    if(maximizigPlayer) {
      return alpha;
    } else {
      return beta;
    }
  }
}

GameMove getMiniMaxMove(MiniMax *self)
{
  GameMove bestMove;
  searchBestMove(self, ((Player *)self)->game, self->lookahead, &bestMove,
                     -INFINITY, INFINITY, 1);
  return(bestMove);
}
