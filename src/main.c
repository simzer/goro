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
#include "cliplayer.h"
#include "minimax.h"

int main(int argc, char *argv[])
{
  Game game;
  MiniMax computer = createMiniMax(&game);
  CLIPlayer human = createCLIPlayer(&game);
  Player *players[2] = { &human, &computer };
  PlayerId winner;

  if ((argc > 1) && (strcmp(argv[1], "--tictactoe") == 0)) {
    game = createTicTacToe(3);
    computer.lookahead = 9;
  } else {
    game = createGomoko(createBoard(9,9));
  }

  while(winner = game.vtable->winner(&game),
        (   winner == noPlayer
         && game.vtable->movesPossible(&game)) )
  {
    BoardCoord coord;
    BoardCoordString string;
    Player *actualPlayer;
    switchGamePlayer(&game);
    printBoard(&game.board);
    actualPlayer = players[game.actualPlayer];
    coord = actualPlayer->getMove(actualPlayer);
    setBoardCell(&game.board, coord, actualGamePlayerCell(&game));
    string = boardCoordToString(coord);
    printf("Player %s step: %s\n",
           gamePlayerNames[game.actualPlayer], string.chars);
  }
  printBoard(&game.board);
  printf(winner == noPlayer
         ? "Nobody won!\n"
         : "%s won!\n", gamePlayerNames[winner]);

  return 0;
}
