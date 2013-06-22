
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "game.h"
#include "ui.h"
#include "ai.h"

int main(void)
{
  Game game = tictactoe_create(3);
  BoardCoord coord;
  Player winner;
  while(winner = game.winner(&game),
        (   winner == player_none
         && game.movesPossible(&game)) )
  {
    game_switchPlayer(&game);
    board_print(&game.board);
    coord = (game.actPlayer == player_1) ? ui_move(&game) : ai_move(&game);
    board_setCell(&game.board, coord, game_actPlayerCell(&game));
  }
  board_print(&game.board);
  printf(winner == player_none
         ? "Nobody won!\n"
         : "Player %d won!\n", winner);
  
  return 0;
}
