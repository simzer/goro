
#include <stdlib.h>
#include <math.h>

#include "boarditerator.h"
#include "ai.h"

static double miniMax_search(const MiniMax *self,
                             BoardCoord *coord,
                             Game *game,
                             int lookahead);

const double miniMax_winScore = INFINITY;
const double miniMax_loseScore = -INFINITY;

MiniMax miniMax_create(Game *game) {
  MiniMax self;
  self.monteCarloThreshold = 0.0;
  self.lookahead = 3;
  self.game = game;
  return self;
}

static double miniMax_search(const MiniMax *self,
                             BoardCoord *coord,
                             Game *game,
                             int lookahead)
{
  double result;
  Player winner = game->vtable->winner(game);
  if (   game->vtable->movesPossible(game)
      && (winner == player_none)
      && (lookahead > 0))
  {
    double score;
    double maxScore = -INFINITY;
    BoardCoord maxScoredCoord;
    BoardIterator iterator = boardIterator_create(&game->board);
    for(;game_nextValidMove(game, &iterator);) {
      if (self->monteCarloThreshold < ((double)rand()/RAND_MAX)) {
        BoardCoord tmpCoord;
        Game nextGame = game_copy(game);
        game_switchPlayer(&nextGame);
        board_setCell(&nextGame.board, iterator.coord, game_actPlayerCell(game));
        score = - miniMax_search(self, &tmpCoord, &nextGame, lookahead-1);
        board_destruct(&nextGame.board);
        if (score > maxScore) {
          maxScore = score;
          maxScoredCoord = iterator.coord;
        }
      }
    }
    *coord = maxScoredCoord;
    result = maxScore;
  } else {
    result = game->vtable->evalPosition(game);
  }
  return result;
}

BoardCoord miniMax_move(MiniMax *self)
{
  BoardCoord coord;
  miniMax_search(self, &coord, self->game, self->lookahead);
  return(coord);
}
