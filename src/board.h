
#ifndef __BOARD_H__
#define __BOARD_H__

#include "stdio.h"

typedef enum BoardCell {
  boardCell_empty = 0,
  boardCell_black = 1,
  boardCell_white = 2,
  boardCell_invalid = 3
} BoardCell;

typedef int BoardSize;

typedef struct {
  BoardSize row;
  BoardSize col;
} BoardCoord;
extern const BoardCoord boardCoord_null;
extern BoardCoord boardCoord_create(BoardSize row, BoardSize col);
extern BoardCoord boardCoord_add(BoardCoord *self, BoardCoord *add);

typedef struct {
  BoardSize width;
  BoardSize height;
  char *board;
} Board;

extern Board board_create(BoardSize width, BoardSize height);
extern Board board_load(FILE *file);
extern Board board_copy(Board *self);
extern void board_clear(Board *self);
extern void board_destruct(Board *self);
extern BoardCell board_getCell(Board *self, BoardCoord coord);
extern void board_setCell(Board *self, BoardCoord coord, BoardCell cell);
extern int board_coordInBoard(Board *self, BoardCoord coord);
extern void board_print(Board *self);
extern void board_save(Board *self, FILE *file);
extern int board_isThereEmptyCell(Board *self);

#endif
