
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "game.h"
#include "tictactoe.h"
#include "gomoko.h"
#include "ui.h"
#include "ai.h"

int main(int argc, char *argv[])
{
  Game game;
  if ((argc > 1) && (strcmp(argv[1], "--tictactoe") == 0)) {
    game = tictactoe_create(3);
  } else {
    game = gomoko_create(board_create(9,9));
  }
  MiniMax miniMax = miniMax_create(&game);
  Player winner;
  while(winner = game.vtable->winner(&game),
        (   winner == player_none
         && game.vtable->movesPossible(&game)) )
  {
    BoardCoord coord;
    game_switchPlayer(&game);
    board_print(&game.board);
    coord = (game.actPlayer == player_1)
            ? ui_move(&game)
            : miniMax_move(&miniMax);
    board_setCell(&game.board, coord, game_actPlayerCell(&game));
    printf("Player %d step: %c%d\n",
           game.actPlayer, 'a'+coord.col, 1+coord.row);
  }
  board_print(&game.board);
  printf(winner == player_none
         ? "Nobody won!\n"
         : "%s won!\n", game_playerNames[winner]);

  return 0;
}
