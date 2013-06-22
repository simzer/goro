
#include <stdlib.h>

#include "boarditerator.h"
#include "ai.h"

static int miniMax_search(MiniMax *self,
                          BoardCoord *coord,
                          Game *game,
                          Player origPlayer);

MiniMax miniMax_create(Game *game) {
  MiniMax self;
  self.monteCarloThreshold = 0.0;
  self.game = game;
  return self;
}

static int miniMax_search(MiniMax *self,
                          BoardCoord *coord,
                          Game *game,
                          Player origPlayer)
{
  int result;
  Player winner = game->winner(game);
  if (   game->movesPossible(game)
      && (winner == player_none) )
  {
    int score;
    int extrScore = game->actPlayer == origPlayer ? -0x8000 : 0x7FFF;
    BoardCoord extrCoord;
    BoardIterator iter = boardIterator_create(&game->board);
    for(;boardIterator_next(&iter);)
    {
      BoardCoord nextCoord = iter.coord;
      if (game->validMove(game, nextCoord)
          && (self->monteCarloThreshold < ((double)rand()/RAND_MAX))
         ) {
        BoardCoord tmpCoord;
        Game nextGame = game_copy(game);
        game_switchPlayer(&nextGame);
        board_setCell(&nextGame.board, nextCoord, game_actPlayerCell(game));
        score = miniMax_search(self, &tmpCoord, &nextGame, origPlayer);
        board_destruct(&nextGame.board);
        if ( game->actPlayer == origPlayer
            ? (score > extrScore) 
            : (score < extrScore) ) 
        { 
          extrScore = score;
          extrCoord = nextCoord;
        }
      }
    }  
    *coord = extrCoord;
    result = extrScore;
  } else {
    result = (winner == origPlayer   ? +1 :
              winner == player_none  ?  0 :
                                       -1 );
  }
  return result;
}

BoardCoord miniMax_move(MiniMax *self)
{
  BoardCoord coord;
  miniMax_search(self, &coord, self->game, self->game->actPlayer);
  return(coord);
}
