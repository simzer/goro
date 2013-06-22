
#include "boarditerator.h"

BoardIterator boardIterator_create(Board *boardToIterate)
{
  BoardIterator res;
  res.row = 0;
  res.col = 0;
  res.run = 0;
  res.board = boardToIterate;
  return res;
}

int boardIterator_next(BoardIterator *self)
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
  return(self->run);
}
