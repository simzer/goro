/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

int sameBoardsOnDifferentMemoryLocation(Board *board1, Board *board2)
{
  return    board1->cells != board2->cells
         && board1->height == board2->height
         && board1->width == board2->width
         && boardEqual(board1, board2);
}

Board full19x19Board(void) {
  Board board = createSquareBoard(19);
  memset(board.cells, blackBoardCell, 19*19);
  return board;
}

static void boardCellEmptyFillStateDetected(void)
{
  assert(boardCellFilled(blackBoardCell));
  assert(boardCellFilled(whiteBoardCell));
  assert(!boardCellFilled(emptyBoardCell));
  assert(!boardCellFilled(invalidBoardCell));
}

static void newBoardGetsSizes(void)
{
  Board board = createBoard(3,4);
  assert(board.width == 3);
  assert(board.height == 4);
  destructBoard(&board);
}

static void clearBoardDetected(void) {
  Board board = createSquareBoard(4);
  assert(isBoardClear(&board));
  setBoardCell(&board, createBoardCoord(3,3), blackBoardCell);
  assert(!isBoardClear(&board));
  destructBoard(&board);
}

static void squareBoardHasSameSizedSides(void)
{
  Board board = createSquareBoard(4);
  assert(board.width == 4);
  assert(board.height == 4);
  destructBoard(&board);
}

static void newBoardIsClear(void)
{
  Board board = createSquareBoard(4);
  assert(isBoardClear(&board));
  destructBoard(&board);
}

static void testLoadBoard(void)
{
  // todo: create test, function not used yet, is it necessary at all?
}

static void copyResultsEqualBoardsOnDifferentMemoryLocation(void)
{
  int i;
  Board board = createBoard(10, 15);
  Board board2;
  for (i = 0; i < 10*15; i++) board.cells[i] = rand() % 4;
  board2 = copyBoard(&board);
  assert(sameBoardsOnDifferentMemoryLocation(&board, &board2));
  destructBoard(&board);
  destructBoard(&board2);
}

static void boardCanBeCleared(void)
{
  Board board = createBoard(3,4);
  clearBoard(&board);
  assert(isBoardClear(&board));
  destructBoard(&board);
}

static void equalityDetectedOnSameAndDifferentBoards(void)
{
  Board board = createBoard(10, 15);
  Board board2 = createBoard(10, 15);
  assert(boardEqual(&board, &board2));
  setBoardCell(&board, createBoardCoord(14,9), blackBoardCell);
  assert(!boardEqual(&board, &board2));
  setBoardCell(&board2, createBoardCoord(14,9), blackBoardCell);
  assert(boardEqual(&board, &board2));
  destructBoard(&board);
  destructBoard(&board2);
}

static void boardDestructionNullifiesMemoryPointer(void)
{
  Board board = createSquareBoard(19);
  destructBoard(&board);
  assert(board.cells == 0);
}

static void cellCanBeReadFromBoard(void)
{
  Board board = createSquareBoard(19);
  board.cells[19*19/2] = blackBoardCell;
  assert(getBoardCell(&board, boardTengen(&board)) == blackBoardCell);
  destructBoard(&board);
}

static void setCellReadBackAsItSet(void)
{
  Board board = createSquareBoard(19);
  setBoardCell(&board, createBoardCoord(5,7), blackBoardCell);
  assert(getBoardCell(&board, createBoardCoord(5,7)) == blackBoardCell);
  destructBoard(&board);
  // todo: how to test out of range assertion?
}

static void clearedCellReadBackAsClear(void)
{
  Board board = createSquareBoard(19);
  memset(board.cells, blackBoardCell, 19*19);
  clearBoardCell(&board, createBoardCoord(5,7));
  assert(getBoardCell(&board, createBoardCoord(5,6)) != emptyBoardCell);
  assert(getBoardCell(&board, createBoardCoord(5,7)) == emptyBoardCell);
  destructBoard(&board);
  // todo: how to test out of range assertion?
}

static void inRangeCoordsAreOnBoard(void)
{
  Board board = createSquareBoard(19);
  assert(coordInBoard(&board, createBoardCoord(0,0)));
  assert(coordInBoard(&board, createBoardCoord(18,18)));
  destructBoard(&board);
}

static void outOfRangeCoordsAreNotOnBoard(void)
{
  Board board = createSquareBoard(19);
  assert(!coordInBoard(&board, createBoardCoord(-1,0)));
  assert(!coordInBoard(&board, createBoardCoord(0,-1)));
  assert(!coordInBoard(&board, createBoardCoord(18,19)));
  assert(!coordInBoard(&board, createBoardCoord(19,18)));
  destructBoard(&board);
}

static void testPrintBoard(void)
{
  // hard to test, but bug will be visible asap it is broken
}

static void testSaveBoard(void)
{
  // todo: create test, function not used yet, is it necessary at all?
}

static void notFullBoardCanBeDetected(void)
{
  Board board = full19x19Board();
  assert(!boardHasEmptyCell(&board));
  clearBoardCell(&board, createBoardCoord(18,18));
  assert(boardHasEmptyCell(&board));
  destructBoard(&board);
}

static void testTengenNeigbour(Board *board, Neighbourhood neighbourhood,
                               int hasNeighbour)
{
  assert(boardCellHasNeighbour(board,boardTengen(board), neighbourhood)
          == hasNeighbour);
}

static void boardCellNeighbourDetected(void)
{
  Board board = createSquareBoard(19);
  testTengenNeigbour(&board, fourNeighbourhood, 0);
  setBoardCell(&board, createBoardCoord(10,10), blackBoardCell);
  testTengenNeigbour(&board, fourNeighbourhood, 0);
  testTengenNeigbour(&board, eightNeighbourhood, 1);
  setBoardCell(&board, createBoardCoord(9,10), blackBoardCell);
  testTengenNeigbour(&board, fourNeighbourhood, 1);
  testTengenNeigbour(&board, eightNeighbourhood, 1);
  destructBoard(&board);
}

static void boardCoordsAreMirror(int row, int col, int mirrorRow, int mirrorCol)
{
  Board board = createSquareBoard(19);
  assert(boardCoordsEqual(mirrorBoardCoord(&board, createBoardCoord(row,col)),
                          createBoardCoord(mirrorRow, mirrorCol)));
  destructBoard(&board);
}

static void boardCoordCanBeMirrored(void)
{
  boardCoordsAreMirror(0, 18, 18, 0);
  boardCoordsAreMirror(18, 18, 0, 0);
  boardCoordsAreMirror(18, 18, 0, 0);
}

static void tengenIsItselfIfMirrored(void)
{
  Board board = createSquareBoard(19);
  assert(boardCoordsEqual(mirrorBoardCoord(&board, boardTengen(&board)),
                                                   boardTengen(&board)));
  destructBoard(&board);
}

static void tengenIsCenterOfBoard(void)
{
  Board board = createSquareBoard(19);
  assert(boardCoordsEqual(boardTengen(&board), createBoardCoord(9,9)));
  destructBoard(&board);
}

void testboard(void)
{
  boardCellEmptyFillStateDetected();
  newBoardGetsSizes();
  newBoardIsClear();
  squareBoardHasSameSizedSides();
  copyResultsEqualBoardsOnDifferentMemoryLocation();
  boardCanBeCleared();
  equalityDetectedOnSameAndDifferentBoards();
  boardDestructionNullifiesMemoryPointer();
  cellCanBeReadFromBoard();
  setCellReadBackAsItSet();
  clearedCellReadBackAsClear();
  inRangeCoordsAreOnBoard();
  outOfRangeCoordsAreNotOnBoard();
  notFullBoardCanBeDetected();
  boardCellNeighbourDetected();
  boardCoordCanBeMirrored();
  tengenIsItselfIfMirrored();
  tengenIsCenterOfBoard();
  clearBoardDetected();
  testPrintBoard();
  testSaveBoard();
  testLoadBoard();
}
