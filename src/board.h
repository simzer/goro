/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "stdio.h"

typedef enum Direction Direction;

typedef enum BoardCell {
  emptyBoardCell = 0,
  blackBoardCell = 1,
  whiteBoardCell = 2,
  invalidBoardCell = 3
} BoardCell;

typedef struct BoardCoordString {
  char chars[4];
} BoardCoordString;

typedef int BoardSize;

typedef struct {
  BoardSize row;
  BoardSize col;
} BoardCoord;
extern const BoardCoord nullBoardCoord;
extern BoardCoord createBoardCoord(BoardSize row, BoardSize col);
extern BoardCoord addBoardCoords(BoardCoord *self, BoardCoord *add);
extern int boardCoordsEqual(BoardCoord *self, BoardCoord *reference);
extern BoardCoord getBoardCoordNeighbour(BoardCoord *self,
                                         Direction direction,
                                         BoardSize distance);
extern BoardCoord stringToBoardCoord(BoardCoordString string);
extern BoardCoordString boardCoordToString(BoardCoord coord);

enum Direction {
  fullRoundDirectionBegin,
  northWestDirection = 0,
  northDirection     = 1,
  northEastDirection = 2,
  westDirection      = 3,
  halfRoundDirectionBegin,
  eastDirection      = 4,
  southWestDirection = 5,
  southDirection     = 6,
  southEastDirection,
  roundDirectionEnd  = 7,
  directionNumber    = 8
};
extern const BoardCoord directionCoords[directionNumber];

typedef struct {
  BoardSize width;
  BoardSize height;
  char *board;
} Board;

extern Board createBoard(BoardSize width, BoardSize height);
extern Board loadBoard(FILE *file);
extern Board copyBoard(Board *self);
extern void clearBoard(Board *self);
extern void destructBoard(Board *self);
extern BoardCell getBoardCell(Board *self, BoardCoord coord);
extern void setBoardCell(Board *self, BoardCoord coord, BoardCell cell);
extern int coordInBoard(Board *self, BoardCoord coord);
extern void printBoard(Board *self);
extern void saveBoard(Board *self, FILE *file);
extern int boardHasEmptyCell(Board *self);
extern int boardCellHasNeighbour(Board *self, BoardCoord coord);

#endif
