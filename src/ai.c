
#include <stdlib.h>
#include <math.h>

#include "boarditerator.h"
#include "ai.h"

static double miniMax_search(const MiniMax *self,
                             BoardCoord *coord,
                             Game *game);

const double miniMax_winScore = INFINITY;
const double miniMax_loseScore = -INFINITY;

MiniMax miniMax_create(Game *game) {
  MiniMax self;
  self.monteCarloThreshold = 0.0;
  self.game = game;
  return self;
}

static double miniMax_search(const MiniMax *self,
                             BoardCoord *coord,
                             Game *game)
{
  double result;
  Player winner = game->vtable->winner(game);
  if (   game->vtable->movesPossible(game)
      && (winner == player_none) )
  {
    double score;
    double maxScore = -INFINITY;
    BoardCoord maxScoredCoord;
    BoardIterator iter = boardIterator_create(&game->board);
    for(;boardIterator_next(&iter);)
    {
      if (game->vtable->validMove(game, iter.coord)
          && (self->monteCarloThreshold < ((double)rand()/RAND_MAX))
         ) {
        BoardCoord tmpCoord;
        Game nextGame = game_copy(game);
        game_switchPlayer(&nextGame);
        board_setCell(&nextGame.board, iter.coord, game_actPlayerCell(game));
        score = - miniMax_search(self, &tmpCoord, &nextGame);
        board_destruct(&nextGame.board);
        if (score > maxScore) {
          maxScore = score;
          maxScoredCoord = iter.coord;
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
  miniMax_search(self, &coord, self->game);
  return(coord);
}
