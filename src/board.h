
#ifndef __BOARD_H__
#define __BOARD_H__

#include "stdio.h"

typedef enum BoardCell {
  boardCell_empty = 0,
  boardCell_black = 1,
  boardCell_white = 2,
  boardCell_invalid = 3
} BoardCell;

typedef char BoarCellContainer;

typedef int BoardSize;

typedef struct {
  BoardSize row;
  BoardSize col;
} BoardCoord;

typedef struct {
  BoardSize width;
  BoardSize height;
  BoarCellContainer *board;
} Board;

extern Board board_create(BoardSize width, BoardSize height);
extern void board_clear(Board *self);
extern void board_destruct(Board *self);
extern BoardCell board_getCell(Board *self, BoardSize row, BoardSize col);
extern int board_setCell(Board *self, BoardSize row, BoardSize col, BoardCell cell);
extern Board board_copy(Board *self);
extern void board_print(Board *self);
extern void board_save(FILE *file, Board *self);
extern Board board_load(FILE *file);
extern int board_validMove(BoardSize row, BoardSize col, Board *self);
extern int board_movesPossible(Board *self);
extern BoardCell board_winner(Board *self);

#endif
