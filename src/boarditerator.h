/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __BOARDITERATOR_H__
#define __BOARDITERATOR_H__

#include "board.h"

typedef struct
{
  int index;
  BoardCoord center;
  BoardCoord neighbour;
  Neighbourhood neighbourhood;
} NeighbourIterator;

extern NeighbourIterator createNeighbourIterator(BoardCoord coord,
                                                 Neighbourhood neighbourhood);
extern int getNeighbours(NeighbourIterator *self);
extern BoardCoord getNeighbourDirection(NeighbourIterator *self);


typedef struct
{
  BoardSize index;
  BoardSize size;
  BoardCoord coord;
  BoardCoord startCoord;
  BoardCoord direction;
} BoardLineIterator;
extern BoardLineIterator createBoardLineIterator(BoardCoord startCoord,
                                                 BoardCoord direction,
                                                 BoardSize size);
extern int getBoardLineCoords(BoardLineIterator *self);

typedef struct
{
  BoardCoord coord;
  Board *board;
} BoardIterator;

extern BoardIterator createBoardIterator(Board *board);
extern int boardIteratorFinished(BoardIterator *self);

#endif
