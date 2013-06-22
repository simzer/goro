
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "game.h"
#include "ui.h"
#include "ai.h"

int main(void)
{
  Board board = board_create(19,19);
  Game game = game_create();
  BoardSize i, j;
  Player winner;
  while(winner = board_winner(&board),
        (   winner == player_none
         && board_movesPossible(&board)) )
  {
    game_switchPlayer(&game);
    board_print(&board);
    if (game.actPlayer == player_1) {
      ui_move(&i, &j, &game, &board);
    } else {
      ai_move(&i, &j, &game, &board);
    }
    board_setCell(&board, i, j, game.actPlayer);
  }
  board_print(&board);
  printf(winner == 0 ? "Nobody won!\n" : "Player %d won!\n", winner);
  
  return 0;
}
