
#include <stdlib.h>

#include "boarditerator.h"
#include "ai.h"

double ai_mcThreshold = 0.0;

static int ai_minimax(int *row, int *col, 
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
    int extrRow, extrCol;
    BoardIterator iter = boardIterator_create(board);
    for(;boardIterator_next(&iter);)
    {
      int nextRow, nextCol;
      nextRow = iter.col;
      nextCol = iter.row;
      if (board_validMove(nextRow, nextCol, board)
          && (ai_mcThreshold < ((double)rand()/RAND_MAX))
         ) {
        BoardSize tmpI, tmpJ;
        Board nextBoard;
        Game nextGame = *game;
        nextBoard = board_copy(board);
        game_switchPlayer(&nextGame);
        board_setCell(&nextBoard, nextRow, nextCol, game->actPlayer);
        score = ai_minimax(&tmpI, &tmpJ, &nextGame, origGame, &nextBoard);
        board_destruct(&nextBoard);
        if ( game->actPlayer == origGame->actPlayer
            ? (score > extrScore) 
            : (score < extrScore) ) 
        { 
          extrScore = score;
          extrRow = nextRow;
          extrCol = nextCol;
        }
      }
    }  
    *row = extrRow;
    *col = extrCol; 
    result = extrScore;
  } else {
    result = (winner == origGame->actPlayer   ? +1 :
              winner == player_none           ?  0 :
                                                -1 );
  }
  return result;
}

void ai_move(BoardSize *i, BoardSize *j, Game *game, Board *board)
{
  ai_minimax(i, j, game, game, board);
  printf("Player %d step: %c%d\n", game->actPlayer, 'a'+*i, 1+*j);
}
