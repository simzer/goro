/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include "board.h"
#include "group.h"

int groupLiberties(Group *self)
{
  Board board = copyBoard(self->board);
  int i;
  for (i = 0; i < self->size; i++) {
    Direction direction; // todo: go direction instead
    for (direction = northDirection;
         direction <= southDirection;
         direction += 2)
    setBoardCell(self->board, self->cells[i], emptyBoardCell);
  }

  destructBoard(&board);
  // todo implement
  assert(0);
  return 0;
}

void removeGroup(Group *self)
{
  int i;
  for (i = 0; i < self->size; i++) {
    setBoardCell(self->board, self->cells[i], emptyBoardCell);
  }
  self->size = 0;
  free(self->cells);
  self->cells = 0;
}

GroupIterator createGroupIterator(Board *board)
{
  // todo implement
  assert(0);
  return 0;
}

int groupIteratorFinished(GroupIterator *self)
{
  // todo implement
  assert(0);
  return 0;
}

