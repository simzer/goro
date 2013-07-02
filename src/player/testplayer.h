/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __TESTPLAYER_H__
#define __TESTPLAYER_H__

#include "player.h"

typedef struct TestPlayer {
  Player player;
} TestPlayer;

extern TestPlayer createTestPlayer(Game *game);

#endif
