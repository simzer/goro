/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __BOARDCOORD_H__
#define __BOARDCOORD_H__

#include "stdio.h"

typedef struct BoardCoordString {
  char chars[4];
} BoardCoordString;

typedef enum {
  nanoBoard,
  mikroBoard,
  smallBoard,
  mediumBoard,
  tibetanBoard,
  normalBoard,
  standardBoardTypes
} StandardBoardType;

typedef int BoardSize;
extern const BoardSize standardBoardSizes[standardBoardTypes];
extern BoardSize charToBoardColumn(char character);
extern char boardColumnToChar(BoardSize column);

typedef struct {
  BoardSize row;
  BoardSize col;
} BoardCoord;
extern const BoardCoord nullBoardCoord;
extern BoardCoord createBoardCoord(BoardSize row, BoardSize col);
extern BoardCoord addBoardCoords(BoardCoord self, BoardCoord add);
extern BoardCoord subBoardCoords(BoardCoord self, BoardCoord sub);
extern BoardCoord mulBoardCoord(int multiplicator, BoardCoord self);
extern int boardCoordsEqual(BoardCoord self, BoardCoord reference);
extern BoardCoord stringToBoardCoord(BoardCoordString string);
extern BoardCoordString boardCoordToString(BoardCoord coord);

typedef struct {
  int startIndex;
  int endIndex;
  BoardCoord *neighbours;
} Neighbourhood;
extern const Neighbourhood fourNeighbourhood;
extern const Neighbourhood eightNeighbourhood;
extern Neighbourhood halfNeighbourhood(Neighbourhood *self);

#endif
