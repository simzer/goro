
#include <stdlib.h>

#include "boarditerator.h"
#include "ai.h"

double ai_mcThreshold = 0.0;

static int ai_minimax(int *i, int *j, 
                       game *actGame, 
                       game *origGame, 
                       board *actBoard)
{
  int result;
  player winner = board_winner(actBoard);
  if (   board_movesPossible(actBoard)
      && (winner == PLAYER_NONE) )
  {
    int score;
    int extrScore = actGame->actPlayer == origGame->actPlayer ? -0x8000 : 0x7FFF;
    int extrI, extrJ;
    boardIterator iter;
    for(boardIterator_init(&iter);
        !boardIterator_finished(&iter);
        boardIterator_next(&iter)) 
    {
      int nextI, nextJ;
      boardIterator_get(&nextI, &nextJ, &iter);
      if (board_validMove(nextI, nextJ, actBoard)
          && (ai_mcThreshold < ((double)rand()/RAND_MAX))
         ) {
        int tmpI, tmpJ;
        board nextBoard;
        game  nextGame = *actGame;
        board_copy(&nextBoard, actBoard);
        game_switchPlayer(&nextGame);
        board_move(nextI, nextJ, actGame->actPlayer, &nextBoard);
        score = ai_minimax(&tmpI, &tmpJ, &nextGame, origGame,  &nextBoard);
        if ( actGame->actPlayer == origGame->actPlayer 
            ? (score > extrScore) 
            : (score < extrScore) ) 
        { 
          extrScore = score;
          extrI = nextI;
          extrJ = nextJ;
        }
      }
    }  
    *i = extrI;
    *j = extrJ; 
    result = extrScore;
  } else {
    result = (winner == origGame->actPlayer   ? +1 :
              winner == PLAYER_NONE           ?  0 : 
                                                -1 );
  }
/*printf("\n");board_print(actBoard);printf("%d\n", result);*/
  return result;
}

int ai_move(int *i, int *j, game *actGame, board *actBoard)
{
  ai_minimax(i, j, actGame, actGame, actBoard);
  printf("Player %d step: %c%d\n", actGame->actPlayer, 'a'+*i, 1+*j);
}
