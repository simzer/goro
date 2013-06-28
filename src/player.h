/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "game.h"
#include "board.h"

typedef struct Player Player;

struct Player {
  Game *game;
  BoardCoord (*getMove)(Player *self);
};

#endif
