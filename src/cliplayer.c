/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include  "stdlib.h"
#include  "string.h"
#include  "cliplayer.h"

static BoardCoord getCLIPlayerMove(CLIPlayer *self);

CLIPlayer createCLIPlayer(Game *game)
{
  CLIPlayer self;
  self.player.game = game;
  self.player.getMove = &getCLIPlayerMove;
  return self;
}

static BoardCoord getCLIPlayerMove(CLIPlayer *self)
{
  BoardCoord coord;
  BoardCoordString string;
  int  rowIndex;
  Game* game = ((Player *)self)->game;
  printBoard(&game->board);
  do {
    printf("Player %d step: ", game->actualPlayer);
    scanf("%3s", string.chars);
    if (strcmp(string.chars,"q") == 0) exit(0); //todo: exit properly
    coord = stringToBoardCoord(string);
  } while (!game->vtable->validMove(game, coord)
           && (printf("Invalid move.\n"),1) );
  return(coord);
}
