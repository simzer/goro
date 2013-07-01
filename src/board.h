/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "stdio.h"

#include "boardcoord.h"

typedef enum BoardCell {
  emptyBoardCell = 0,
  blackBoardCell = 1,
  whiteBoardCell = 2,
  invalidBoardCell = 3
} BoardCell;
extern const char* boardCellNames[];
extern int boardCellFilled(BoardCell self);

typedef struct {
  BoardSize width;
  BoardSize height;
  char *cells;
} Board;

extern Board createBoard(BoardSize width, BoardSize height);
extern Board createSquareBoard(BoardSize size);
extern Board loadBoard(FILE *file);
extern Board copyBoard(Board *self);
extern void clearBoard(Board *self);
extern int boardEqual(Board *self, Board *ref);
extern void destructBoard(Board *self);
extern BoardCell getBoardCell(Board *self, BoardCoord coord);
extern void setBoardCell(Board *self, BoardCoord coord, BoardCell cell);
extern void clearBoardCell(Board *self, BoardCoord coord);
extern int coordInBoard(Board *self, BoardCoord coord);
extern void printBoard(Board *self);
extern void saveBoard(Board *self, FILE *file);
extern int boardHasEmptyCell(Board *self);
extern int boardCellHasNeighbour(Board *self, BoardCoord coord,
                                 Neighbourhood neighbourhood);
extern BoardCoord mirrorBoardCoord(Board *board, BoardCoord coord);
extern BoardCoord boardTengen(Board *self);

#endif
