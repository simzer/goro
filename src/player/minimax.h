/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __MINIMAX_H__
#define __MINIMAX_H__

#include "player.h"

typedef struct MiniMax {
  Player player;
  int lookahead;
} MiniMax;

extern const double miniMaxWinScore;
extern const double miniMaxLoseScore;

extern MiniMax createMiniMax(Game *game);

#endif
