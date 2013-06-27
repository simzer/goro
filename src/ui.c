/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include  "ui.h"

BoardCoord ui_move(Game *game)
{
  BoardCoord coord;
  char c;
  int  n;
  printf("Player %d step: ", game->actualPlayer);
  do {
    scanf ("%c%d", &c, &n);
    coord = boardCoord_create(n - 1, c - 'a');
  } while (!game->vtable->validMove(game, coord));
  return(coord);
}
