/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <string.h>
#include <assert.h>

#include "boardcoord.h"

static void testCreateBoardCoordString(void)
{
  char string[] = "f5";
  BoardCoordString coordString = createBoardCoordString(string);
  assert(strcmp(coordString.chars, string) == 0);
}

static void testCharToBoardColumn(void)
{
  assert(charToBoardColumn('a') == 0);
  assert(charToBoardColumn('t') == 18);
  assert(charToBoardColumn('A') == 0);
  assert(charToBoardColumn('T') == 18);
  assert(charToBoardColumn('?') == -1);
}

static void testBoardColumnToChar(void)
{
  assert(boardColumnToChar(0) == 'A');
  assert(boardColumnToChar(18) == 'T');
  assert(boardColumnToChar(19) == '?');
  assert(boardColumnToChar(-1) == '?');
}

static void testCreateBoardCoord(void)
{
  BoardCoord coord = createBoardCoord(5, 6);
  assert(coord.col == 6);
  assert(coord.row == 5);
}

static void testAddBoardCoords(void)
{
  BoardCoord coord1 = createBoardCoord(5, 6);
  BoardCoord coord2 = createBoardCoord(7, 9);
  BoardCoord added = addBoardCoords(coord1, coord2);
  assert(added.col == 6+9);
  assert(added.row == 5+7);
}

static void testSubBoardCoords(void)
{
  BoardCoord coord1 = createBoardCoord(5, 6);
  BoardCoord coord2 = createBoardCoord(7, 9);
  BoardCoord substracted = subBoardCoords(coord2, coord1);
  assert(substracted.col == 9-6);
  assert(substracted.row == 7-5);
}

static void testMulBoardCoord(void)
{
  BoardCoord multiplied = mulBoardCoord(3, createBoardCoord(5, 6));
  assert(multiplied.col == 6*3);
  assert(multiplied.row == 5*3);
}

static void testBoardCoordsEqual(void)
{
  BoardCoord coord1 = createBoardCoord(5, 6);
  BoardCoord coord2 = createBoardCoord(5, 9);
  BoardCoord coord3 = createBoardCoord(7, 6);
  assert(!boardCoordsEqual(coord1, coord2));
  assert(!boardCoordsEqual(coord1, coord3));
  assert(boardCoordsEqual(coord1, coord1));
}

static void testStringToBoardCoord(void)
{
  BoardCoord coord1 = createBoardCoord(5, 6);
  BoardCoord coord2 = stringToBoardCoord(createBoardCoordString("g6"));
  BoardCoord coord3 = stringToBoardCoord(createBoardCoordString("G6"));
  BoardCoord coord4 = stringToBoardCoord(createBoardCoordString("45"));
  BoardCoord coord5 = stringToBoardCoord(createBoardCoordString("ff"));
  BoardCoord coord6 = createBoardCoord(15, 9);
  BoardCoord coord7 = stringToBoardCoord(createBoardCoordString("K16"));
  BoardCoord coord8 = stringToBoardCoord(createBoardCoordString("X16"));
  assert(boardCoordsEqual(coord1, coord2));
  assert(boardCoordsEqual(coord1, coord3));
  assert(boardCoordsEqual(nullBoardCoord, coord4));
  assert(boardCoordsEqual(nullBoardCoord, coord5));
  assert(boardCoordsEqual(coord6, coord7));
  assert(boardCoordsEqual(nullBoardCoord, coord8));
}

static void testBoardCoordToString(void)
{
  BoardCoordString string1 = boardCoordToString(createBoardCoord(5,6));
  BoardCoordString string2 = boardCoordToString(createBoardCoord(15,9));
  BoardCoordString string3 = boardCoordToString(createBoardCoord(-1,9));
  BoardCoordString string4 = boardCoordToString(createBoardCoord(5,-1));
  assert(strcmp(string1.chars, "G6") == 0);
  assert(strcmp(string2.chars, "K16") == 0);
  printf(string3.chars);
  assert(strcmp(string3.chars, "nan") == 0);
  assert(strcmp(string4.chars, "nan") == 0);
}

static void testHalfNeighbourhood(void)
{
  assert(halfNeighbourhood(&fourNeighbourhood).startIndex == 0);
  assert(halfNeighbourhood(&fourNeighbourhood).endIndex == 1);
  assert(halfNeighbourhood(&eightNeighbourhood).startIndex == 0);
  assert(halfNeighbourhood(&eightNeighbourhood).endIndex == 3);
}

void testboardcoord(void)
{
  testCreateBoardCoordString();
  testCharToBoardColumn();
  testBoardColumnToChar();
  testCreateBoardCoord();
  testAddBoardCoords();
  testSubBoardCoords();
  testMulBoardCoord();
  testBoardCoordsEqual();
  testStringToBoardCoord();
  testBoardCoordToString();
  testHalfNeighbourhood();
}
