
#include "boarditerator.h"

board_iterator *board_iterator_create(board *boardToIterate)
{
  board_iterator *res = malloc(sizeof(board_iterator));
  if (res == NULL)
  {
      fprintf(stderr, "Out of memory.\n");
      exit(1);
  }
  res->row = 0;
  res->col = 0;
  res->run = 0;
  res->board = boardToIterate;
  return res;
}

void board_iterator_destruct(board_iterator *self)
{
  free(self);
}

int board_iterator_next(int *row, int *col, int **pcell, board_iterator *self)
{
  if (!self->run)
  {
    self->row = 0;
    self->col = 0;
    self->run = 1;
  }
  else
  {
    self->col++;
    if (self->col >= self->board->width) {
      self->col = 0;
      self->row++;
    }
    if (self->row >= self->board->height) {
      self->row = 0;
      self->run = 0;
    }
  }
  *row  = self->row;
  *col  = self->col;
  *pcell = &(self->board->board[self->row*self->board->width
                                    + self->col]);
  return(self->run);
}
