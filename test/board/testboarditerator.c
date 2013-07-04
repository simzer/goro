/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "boarditerator.h"

static void neighbourIteratorCreatedInGoodPosition(void)
{
  NeighbourIterator iterator =
      createNeighbourIterator(createBoardCoord(5,7), eightNeighbourhood);
  assert(boardCoordsEqual(iterator.center, createBoardCoord(5,7)));
}

static void neighbourIteratorInitialisedWithNullCoord(void)
{
  NeighbourIterator iterator =
      createNeighbourIterator(createBoardCoord(5,7), eightNeighbourhood);
  assert(boardCoordsEqual(iterator.neighbour, nullBoardCoord));
}

static void testGetNeighbours(void)
{
  int i;
  const int reference[][2] = { {1, 0}, {0, 1}, {0, -1}, {-1, 0} };
  BoardCoord distance;
  NeighbourIterator iterator =
      createNeighbourIterator(createBoardCoord(5,7), fourNeighbourhood);
  for(i = 0; i < 4; i++) {
    assert(getNeighbours(&iterator));
    distance = subBoardCoords(iterator.center,iterator.neighbour);
    assert(   distance.row == reference[i][0]
           && distance.col == reference[i][1]);
  }
  assert(!getNeighbours(&iterator));
}



static void testGetNeighbourDirection(void)
{

}

static void testCreateBoardLineIterator(void)
{

}

static void testGetBoardLineCoords(void)
{

}

static void testCreateBoardIterator(void)
{

}

static void testBoardIteratorFinished(void)
{

}

void testboarditerator(void)
{
  neighbourIteratorCreatedInGoodPosition();
  neighbourIteratorInitialisedWithNullCoord();
  testGetNeighbours();
  testGetNeighbourDirection();
  testCreateBoardLineIterator();
  testGetBoardLineCoords();
  testCreateBoardIterator();
  testBoardIteratorFinished();
}
