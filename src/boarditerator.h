
#ifndef __BOARDITERATOR_H__
#define __BOARDITERATOR_H__

#include "board.h"

typedef struct
{
  int run;
  BoardCoord coord;
  Board *board;
} BoardIterator;

extern BoardIterator boardIterator_create(Board *board);
extern int boardIterator_next(BoardIterator *self);

#endif
