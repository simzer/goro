/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "boarditerator.h"

static NeighbourIterator exampleNeighbourIterator(void)
{
  return createNeighbourIterator(createBoardCoord(5,7), fourNeighbourhood);
}

static NeighbourIterator lastItemNeighbourIterator(void)
{
  int i;
  NeighbourIterator iterator = exampleNeighbourIterator();
  for(i = 0; i < 4; i++) getNeighbours(&iterator);
  return iterator;
}

static void neighbourIteratorCreatedInGoodPosition(void)
{
  assert(boardCoordsEqual(exampleNeighbourIterator().center,
                          createBoardCoord(5,7)));
}

static void neighbourIteratorInitialisedWithNullCoord(void)
{
  assert(boardCoordsEqual(exampleNeighbourIterator().neighbour, nullBoardCoord));
}

static void neighbourIteratorFinishedWithNullCoord(void)
{
  NeighbourIterator iterator = lastItemNeighbourIterator();
  getNeighbours(&iterator);
  assert(boardCoordsEqual(iterator.neighbour, nullBoardCoord));
}

static void neighbourIteratorFinishAfterNeighboursIterated(void)
{
  NeighbourIterator iterator = lastItemNeighbourIterator();
  assert(!getNeighbours(&iterator));
 }

static void neighbourIteratorGivesNeighbours(void)
{
  int i;
  NeighbourIterator iterator = exampleNeighbourIterator();
  for(i = 0; i < 4; i++) {
    BoardCoord distance;
    getNeighbours(&iterator);
    distance = subBoardCoords(iterator.neighbour, iterator.center);
    assert(boardCoordsEqual(distance, getNeighbourDirection(&iterator)));
  }
}

static BoardLineIterator exampleBoardLineIterator(void)
{
  return createBoardLineIterator(createBoardCoord(5,7),
                                 createBoardCoord(1,-1), 5);
}

static BoardLineIterator boardLineIteratorAfterStepped(int steps)
{
  int i;
  BoardLineIterator iterator = exampleBoardLineIterator();
  for (i = 0; i < steps; i++) getBoardLineCoords(&iterator);
  return iterator;
}

static void notStartedBoardLineIteratorGivesNullCoord(void)
{
  assert(boardCoordsEqual(exampleBoardLineIterator().coord, nullBoardCoord));
}

static void boardLineIteratorStartsOnStartCoord(void)
{
  assert(boardCoordsEqual(boardLineIteratorAfterStepped(1).coord,
                          createBoardCoord(5,7)));
}

static void boardLineIteratorStepsInDefinedDirection(void)
{
  assert(boardCoordsEqual(boardLineIteratorAfterStepped(2).coord,
                          addBoardCoords(createBoardCoord(5,7),
                                         createBoardCoord(1,-1))));
}

static void boardLineIteratorResultFalseWhenFinised(void)
{
  BoardLineIterator iterator = boardLineIteratorAfterStepped(5);
  assert(!getBoardLineCoords(&iterator));
  }

static void boardLineIteratorStopsAtEndCoord(void)
{
  assert(boardCoordsEqual(boardLineIteratorAfterStepped(5).coord,
    addBoardCoords(createBoardCoord(5,7),
                   mulBoardCoord(4,createBoardCoord(1,-1)))));
}

static void finishedBoardLineIteratorGivesNullCoord(void)
{
  assert(boardCoordsEqual(boardLineIteratorAfterStepped(6).coord,
                          nullBoardCoord));
}

static BoardIterator exampleBoardIterator(void)
{
  Board *board = malloc(sizeof(Board));
  *board = createBoard(13,19);
  return createBoardIterator(board);
}

static BoardIterator boardIteratorAfterStepped(int steps)
{
  int i;
  BoardIterator iterator = exampleBoardIterator();
  for (i = 0; i < steps; i++) boardIteratorFinished(&iterator);
  return iterator;
}

static void notStartedBoardIteratorGivesNullCoord(void)
{
  assert(boardCoordsEqual(exampleBoardIterator().coord, nullBoardCoord));
}

static void boardIteratorStartsOnTopLeftCoord(void)
{
  assert(boardCoordsEqual(boardIteratorAfterStepped(1).coord,
                          createBoardCoord(0,0)));
}

static void increaseEveryBoardCell(Board *board) {
  BoardIterator iterator = createBoardIterator(board);
  while(!boardIteratorFinished(&iterator)) {
    setBoardCell(board, iterator.coord,
                 getBoardCell(board, iterator.coord)+1);
  }
}

static void boardIteratorGivesEveryCoordOnce(void)
{
  int i;
  Board board = createBoard(13,19);
  increaseEveryBoardCell(&board);
  for(i = 0; i < 13*19; i++) assert(board.cells[i] == 1);
  destructBoard(&board);
}

static void boardIteratorResultFalseWhenFinised(void)
{
  BoardIterator iterator = boardIteratorAfterStepped(13*19);
  assert(boardIteratorFinished(&iterator));
}

static void boardIteratorResultTrueBeforeFinised(void)
{
  BoardIterator iterator = boardIteratorAfterStepped(13*19-1);
  assert(!boardIteratorFinished(&iterator));
}

static void boardIteratorStopsBottomRightCoord(void)
{
  assert(boardCoordsEqual(boardIteratorAfterStepped(13*19).coord,
                          createBoardCoord(18,12)));
}

static void finishedBoardGivesNullCoord(void)
{
  assert(boardCoordsEqual(boardIteratorAfterStepped(13*19+1).coord,
                          nullBoardCoord));
}

static void testNeighbourIterator(void) {
  neighbourIteratorCreatedInGoodPosition();
  neighbourIteratorInitialisedWithNullCoord();
  neighbourIteratorFinishAfterNeighboursIterated();
  neighbourIteratorFinishedWithNullCoord();
  neighbourIteratorGivesNeighbours();
}

static void testBoardLineIterator(void) {
  notStartedBoardLineIteratorGivesNullCoord();
  boardLineIteratorStartsOnStartCoord();
  boardLineIteratorStepsInDefinedDirection();
  boardLineIteratorResultFalseWhenFinised();
  boardLineIteratorStopsAtEndCoord();
  finishedBoardLineIteratorGivesNullCoord();
}

static void testBoardIterator(void)
{
  notStartedBoardIteratorGivesNullCoord();
  boardIteratorStartsOnTopLeftCoord();
  boardIteratorGivesEveryCoordOnce();
  boardIteratorResultTrueBeforeFinised();
  boardIteratorResultFalseWhenFinised();
  boardIteratorStopsBottomRightCoord();
  finishedBoardGivesNullCoord();
}

void testboarditerator(void)
{
  testNeighbourIterator();
  testBoardLineIterator();
  testBoardIterator();
}
