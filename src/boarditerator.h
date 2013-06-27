/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __BOARDITERATOR_H__
#define __BOARDITERATOR_H__

#include "board.h"

typedef struct
{
  int run;
  BoardCoord coord;
  Board *board;
} BoardIterator;

extern BoardIterator createBoardIterator(Board *board);
extern int boardIteratorFinished(BoardIterator *self);

#endif
