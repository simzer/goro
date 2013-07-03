/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

static void testBoardCellFilled(void)
{
  assert(boardCellFilled(blackBoardCell));
  assert(boardCellFilled(whiteBoardCell));
  assert(!boardCellFilled(emptyBoardCell));
  assert(!boardCellFilled(invalidBoardCell));
}

static void testCreateBoard(void)
{
  int i;
  Board board = createBoard(3,4);
  assert(board.width == 3);
  assert(board.height == 4);
  assert(board.cells != 0);
}

static void testIsBoardClear(void) {
  Board board = createSquareBoard(4);
  assert(isBoardClear(&board));
  setBoardCell(&board, createBoardCoord(3,3), blackBoardCell);
  assert(!isBoardClear(&board));
}

static void testCreateSquareBoard(void)
{
  Board board = createSquareBoard(4);
  assert(board.width == 4);
  assert(board.height == 4);
  assert(isBoardClear(&board));
}

static void testLoadBoard(void)
{
  // todo: create test, function not used yet, is it necessary at all?
}

static void testCopyBoard(void)
{
  int i;
  Board board = createBoard(10, 15);
  Board board2;
  for (i = 0; i < 10*15; i++) board.cells[i] = rand() % 4;
  board2 = copyBoard(&board);
  assert(board.cells != board2.cells);
  assert(board.height == board2.height);
  assert(board.width == board2.width);
  assert(boardEqual(&board, &board2));
}

static void testClearBoard(void)
{
  Board board = createBoard(3,4);
  clearBoard(&board);
  assert(isBoardClear(&board));
}

static void testBoardEqual(void)
{
  Board board = createBoard(10, 15);
  Board board2 = createBoard(10, 15);
  assert(boardEqual(&board, &board2));
  setBoardCell(&board, createBoardCoord(14,9), blackBoardCell);
  assert(!boardEqual(&board, &board2));
  setBoardCell(&board2, createBoardCoord(14,9), blackBoardCell);
  assert(boardEqual(&board, &board2));
}

static void testDestructBoard(void)
{
  Board board = createSquareBoard(19);
  destructBoard(&board);
  assert(board.cells == 0);
}

static void testGetBoardCell(void)
{
  Board board = createSquareBoard(19);
  board.cells[19*19/2] = blackBoardCell;
  assert(getBoardCell(&board, boardTengen(&board)) == blackBoardCell);
}

static void testSetBoardCell(void)
{
  Board board = createSquareBoard(19);
  setBoardCell(&board, createBoardCoord(5,7), blackBoardCell);
  assert(getBoardCell(&board, createBoardCoord(5,7)) == blackBoardCell);
  // todo: how to test out of range assertion?
}

static void testClearBoardCell(void)
{
  Board board = createSquareBoard(19);
  memset(board.cells, blackBoardCell, 19*19);
  clearBoardCell(&board, createBoardCoord(5,7));
  assert(getBoardCell(&board, createBoardCoord(5,6)) != emptyBoardCell);
  assert(getBoardCell(&board, createBoardCoord(5,7)) == emptyBoardCell);
  // todo: how to test out of range assertion?
}

static void testCoordInBoard(void)
{
  Board board = createSquareBoard(19);
  assert(!coordInBoard(&board, createBoardCoord(-1,0)));
  assert(!coordInBoard(&board, createBoardCoord(0,-1)));
  assert(coordInBoard(&board, createBoardCoord(0,0)));
  assert(coordInBoard(&board, createBoardCoord(18,18)));
  assert(!coordInBoard(&board, createBoardCoord(18,19)));
  assert(!coordInBoard(&board, createBoardCoord(19,18)));
}

static void testPrintBoard(void)
{
  // hard to test, but bug will be visible asap it is broken
}

static void testSaveBoard(void)
{
  // todo: create test, function not used yet, is it necessary at all?
}

static void testBoardHasEmptyCell(void)
{
  Board board = createSquareBoard(19);
  memset(board.cells, blackBoardCell, 19*19);
  assert(!boardHasEmptyCell(&board));
  clearBoardCell(&board, createBoardCoord(18,18));
  assert(boardHasEmptyCell(&board));
}

static void testBoardCellHasNeighbour(void)
{
  Board board = createSquareBoard(19);
  assert(!boardCellHasNeighbour(&board,boardTengen(&board), fourNeighbourhood));
  setBoardCell(&board, createBoardCoord(10,10), blackBoardCell);
  assert(!boardCellHasNeighbour(&board, boardTengen(&board), fourNeighbourhood));
  assert(boardCellHasNeighbour(&board, boardTengen(&board), eightNeighbourhood));
  setBoardCell(&board, createBoardCoord(9,10), blackBoardCell);
  assert(boardCellHasNeighbour(&board, boardTengen(&board), fourNeighbourhood));
  assert(boardCellHasNeighbour(&board, boardTengen(&board), eightNeighbourhood));
}

static void testMirrorBoardCoord(void)
{
  Board board = createSquareBoard(19);
  assert(boardCoordsEqual(mirrorBoardCoord(&board, createBoardCoord(0,18)),
                                                   createBoardCoord(18,0)));
  assert(boardCoordsEqual(mirrorBoardCoord(&board, createBoardCoord(18,18)),
                                                   createBoardCoord(0,0)));
  assert(boardCoordsEqual(mirrorBoardCoord(&board, boardTengen(&board)),
                                                   boardTengen(&board)));
}

static void testBoardTengen(void)
{
  Board board = createSquareBoard(19);
  assert(boardCoordsEqual(boardTengen(&board), createBoardCoord(9,9)));
}

void testboard(void)
{
  testBoardCellFilled();
  testCreateBoard();
  testCreateSquareBoard();
  testLoadBoard();
  testCopyBoard();
  testClearBoard();
  testBoardEqual();
  testDestructBoard();
  testGetBoardCell();
  testSetBoardCell();
  testClearBoardCell();
  testCoordInBoard();
  testPrintBoard();
  testSaveBoard();
  testBoardHasEmptyCell();
  testBoardCellHasNeighbour();
  testMirrorBoardCoord();
  testBoardTengen();
  testIsBoardClear();
}
