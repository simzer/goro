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

static void printWinnerPosition(Game *game);

int main(int argc, char *argv[])
{
  Game game;
  MiniMax computer = createMiniMax(&game);
  CLIPlayer human = createCLIPlayer(&game);
  Player *players[2] = { &human, &computer };

  if ((argc > 1) && (strcmp(argv[1], "--tictactoe") == 0)) {
    game = createTicTacToe(3);
    computer.lookahead = 9;
  } else {
    game = createGomoko(createBoard(9,9));
  }

  while(!game.vtable->over(&game))
  {
    BoardCoord coord;
    BoardCoordString string;
    coord = players[game.actualPlayer]->getMove(players[game.actualPlayer]);
    gameMove(&game, coord);
  }
  printWinnerPosition(&game);
  return 0;
}

static void printWinnerPosition(Game *game)
{
  PlayerId winner = game->vtable->winner(game);
  printBoard(&game->board);
  printf(winner == noPlayer
         ? "Nobody won!\n"
         : "%s won!\n", gamePlayerNames[winner]);
}
