/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GO_H__
#define __GO_H__

#include "game.h"

typedef struct Go {
  Game game;
  Game *history[2];
  double komi;
} Go;

extern Go createGo(Board board);

#endif
