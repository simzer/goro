
#include <stdlib.h>

#include "boarditerator.h"
#include "ai.h"

double ai_mcThreshold = 0.9;

static int ai_minimax(BoardCoord *coord,
                      Game *game,
                      Game *origGame)
{
  int result;
  Player winner = game->winner(game);
  if (   game->movesPossible(game)
      && (winner == player_none) )
  {
    int score;
    int extrScore = game->actPlayer == origGame->actPlayer ? -0x8000 : 0x7FFF;
    BoardCoord extrCoord;
    BoardIterator iter = boardIterator_create(&game->board);
    for(;boardIterator_next(&iter);)
    {
      BoardCoord nextCoord = iter.coord;
      if (game->validMove(game, nextCoord)
          && (ai_mcThreshold < ((double)rand()/RAND_MAX))
         ) {
        BoardCoord tmpCoord;
        Game nextGame = game_copy(game);
        game_switchPlayer(&nextGame);
        board_setCell(&nextGame.board, nextCoord, game_actPlayerCell(game));
        score = ai_minimax(&tmpCoord, &nextGame, origGame);
        board_destruct(&nextGame.board);
        if ( game->actPlayer == origGame->actPlayer
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
    result = (winner == origGame->actPlayer   ? +1 :
              winner == player_none           ?  0 :
                                                -1 );
  }
  return result;
}

BoardCoord ai_move(Game *game)
{
  BoardCoord coord;
  ai_minimax(&coord, game, game);
  printf("Player %d step: %c%d\n",
         game->actPlayer, 'a'+coord.col, 1+coord.row);
  return(coord);
}
