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
  Game *game;
  Gomoko gomoko;
  Game tictactoe;
  MiniMax computer;
  CLIPlayer human;
  Player *players[2] = { &human, &computer };

  if ((argc > 1) && (strcmp(argv[1], "--tictactoe") == 0)) {
    tictactoe = createTicTacToe(3);
    game = &tictactoe;
  } else {
    gomoko = createGomoko(createBoard(19,19));
    game = &gomoko;
  }
  computer = createMiniMax(game);
  computer.lookahead = 3;
  human = createCLIPlayer(game);

  while(!game->vtable->over(game))
  {
    BoardCoord coord;
    BoardCoordString string;
    coord = players[game->actualPlayer]->getMove(players[game->actualPlayer]);
    gameMove(game, coord);
  }
  printWinnerPosition(game);
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
