/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GORO_H__
#define __GORO_H__

#include "game.h"
#include "player.h"
#include "options.h"

struct Goro {
  CliOptionParser options;
  Game *game;
  Player *players[2];
};
typedef struct Goro Goro;
void runGoro(Goro *self);
Goro createGoroFromCLIOption(int argc, char *argv[]);
void destructGoro(Goro *self);

#endif
