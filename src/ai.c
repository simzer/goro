
#include <stdlib.h>

#include "boarditerator.h"
#include "ai.h"

double ai_mcThreshold = 0.0;

static int ai_minimax(BoardCoord *coord,
                      Game *game,
                      Game *origGame,
                      Board *board)
{
  int result;
  Player winner = board_winner(board);
  if (   board_movesPossible(board)
      && (winner == player_none) )
  {
    int score;
    int extrScore = game->actPlayer == origGame->actPlayer ? -0x8000 : 0x7FFF;
    BoardCoord extrCoord;
    BoardIterator iter = boardIterator_create(board);
    for(;boardIterator_next(&iter);)
    {
      BoardCoord nextCoord = iter.coord;
      if (board_validMove(board, nextCoord)
          && (ai_mcThreshold < ((double)rand()/RAND_MAX))
         ) {
        BoardCoord tmpCoord;
        Board nextBoard;
        Game nextGame = *game;
        nextBoard = board_copy(board);
        game_switchPlayer(&nextGame);
        board_setCell(&nextBoard, nextCoord, game_actPlayerCell(game));
        score = ai_minimax(&tmpCoord, &nextGame, origGame, &nextBoard);
        board_destruct(&nextBoard);
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

void ai_move(BoardCoord *coord, Game *game, Board *board)
{
  ai_minimax(coord, game, game, board);
  printf("Player %d step: %c%d\n",
         game->actPlayer, 'a'+coord->col, 1+coord->row);
}
