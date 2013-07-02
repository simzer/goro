/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GOMOKO_H__
#define __GOMOKO_H__

#include "game.h"

typedef struct Gomoko {
  Game game;
  int winnerRowSize;
} Gomoko;

extern Gomoko createGomoko(Board board);

#endif
