/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

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
  printf("Player %d step: ", game->actualPlayer);
  do {
    scanf("%3s", string.chars);
    coord = stringToBoardCoord(string);
  } while (!game->vtable->validMove(game, coord));
  return(coord);
}
