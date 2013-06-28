/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include "boarditerator.h"

BoardIterator createBoardIterator(Board *boardToIterate)
{
  BoardIterator iterator;
  iterator.coord = nullBoardCoord;
  iterator.run = 0;
  iterator.board = boardToIterate;
  return iterator;
}

int boardIteratorFinished(BoardIterator *self)
{
  if (!self->run)
  {
    self->coord.row = 0;
    self->coord.col = 0;
    self->run = 1;
  }
  else
  {
    self->coord.col++;
    if (self->coord.col >= self->board->width) {
      self->coord.col = 0;
      self->coord.row++;
    }
    if (self->coord.row >= self->board->height) {
      self->coord.row = 0;
      self->run = 0;
    }
  }
  return(!self->run);
}
