/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include "boarditerator.h"

BoardLineIterator createBoardLineIterator(BoardCoord startCoord,
                                          BoardCoord direction,
                                          BoardSize size)
{
  BoardLineIterator self;
  self.index = -1;
  self.coord = nullBoardCoord;
  self.startCoord = startCoord;
  self.direction = direction;
  self.size = size;
  return self;
}

int getBoardLineCoords(BoardLineIterator *self)
{
  self->index++;
  if (self->index < self->size)
  {
    self->coord =
        addBoardCoords(self->startCoord,
                       mulBoardCoord(self->index, self->direction));
    return(1);
  }
  else
  {
    self->coord = nullBoardCoord;
    self->index = -1;
    return(0);
  }
}

NeighbourIterator createNeighbourIterator(BoardCoord coord,
                                          Neighbourhood neighbourhood)
{
  NeighbourIterator self;
  self.center = coord;
  self.neighbour = nullBoardCoord;
  self.index = neighbourhood.startIndex - 1;
  self.neighbourhood = neighbourhood;
  return self;
}

BoardCoord getNeighbourDirection(NeighbourIterator *self)
{
  return self->neighbourhood.neighbours[self->index];
}

int getNeighbours(NeighbourIterator *self)
{
  self->index++;
  if (self->index <= self->neighbourhood.endIndex)
  {
    self->neighbour =
        addBoardCoords(self->center,
                       self->neighbourhood.neighbours[self->index]);
    return(1);
  }
  else
  {
    self->neighbour = nullBoardCoord;
    self->index = self->neighbourhood.startIndex -1;
    return(0);
  }
}

BoardIterator createBoardIterator(Board *boardToIterate)
{
  BoardIterator iterator;
  iterator.coord = nullBoardCoord;
  iterator.board = boardToIterate;
  return iterator;
}

int boardIteratorFinished(BoardIterator *self)
{
  if (boardCoordsEqual(self->coord, nullBoardCoord))
  {
    self->coord.row = 0;
    self->coord.col = 0;
  }
  else
  {
    self->coord.col++;
    if (self->coord.col >= self->board->width) {
      self->coord.col = 0;
      self->coord.row++;
    }
    if (self->coord.row >= self->board->height) {
      self->coord = nullBoardCoord;
      return(1);
    }
  }
  return(0);
}
