
#ifndef __BOARDITERATOR_H__
#define __BOARDITERATOR_H__

#include "board.h"

typedef struct
{
  int run;
  int row;
  int col;
  board *board;
} board_iterator;

extern board_iterator *board_iterator_create(board_type *board);
extern void board_iterator_destruct(board_iterator *self);
extern int board_iterator_next(int *row, int *col, int **pcell, board_iterator *self);

#endif
