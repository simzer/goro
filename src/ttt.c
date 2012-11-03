
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "game.h"
#include "ui.h"
#include "ai.h"


int main(void)
{
  board actBoard;
  game  actGame;
  board_init(&actBoard);
  game_init(&actGame);
  int i, j;
  player winner;
  while(winner = board_winner(&actBoard), 
        (   winner == PLAYER_NONE
         && board_movesPossible(&actBoard)) )
  {
    game_switchPlayer(&actGame);
    board_print(&actBoard);
    if (actGame.actPlayer == PLAYER_X) {
      ui_move(&i, &j, &actGame, &actBoard);
    } else {
      ai_move(&i, &j, &actGame, &actBoard);
    }
    board_move(i, j, actGame.actPlayer, &actBoard);
  }
  board_print(&actBoard);
  printf(winner == 0 ? "Nobody won!\n" : "Player %d won!\n", winner);
  
  return 0;
}