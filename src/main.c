/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

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
  MiniMax miniMax = createMiniMax(&game);
  Player winner;
  if ((argc > 1) && (strcmp(argv[1], "--tictactoe") == 0)) {
    game = createTicTacToe(3);
    miniMax.lookahead = 9;
  } else {
    game = createGomoko(createBoard(9,9));
  }
  while(winner = game.vtable->winner(&game),
        (   winner == noPlayer
         && game.vtable->movesPossible(&game)) )
  {
    BoardCoord coord;
    switchGamePlayer(&game);
    printBoard(&game.board);
    coord = (game.actualPlayer == firstPlayer)
            ? getUIMove(&game)
            : getMiniMaxMove(&miniMax);
    setBoardCell(&game.board, coord, actualGamePlayerCell(&game));
    printf("Player %d step: %c%d\n",
           game.actualPlayer, 'a'+coord.col, 1+coord.row);
  }
  printBoard(&game.board);
  printf(winner == noPlayer
         ? "Nobody won!\n"
         : "%s won!\n", gamePlayerNames[winner]);

  return 0;
}
