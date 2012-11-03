
#ifndef __BOARD_H__
#define __BOARD_H__

typedef struct
{
  int width;
  int height;
  char *board;
} board;

extern board *board_create(int width, int height);
extern void board_clear(board *self);
extern void board_destruct(board *self);
extern char board_get_cell(board *self, int row, int col);
extern int  board_set_cell(board *self, int row, int col, char cell);
extern board *board_copy(board *self);
extern void board_print(board *self);
extern void board_save(FILE *file, board *self);
extern board *board_load(FILE *file);
extern int board_validMove(int row, int col, board *self);
extern int board_movesPossible(board *self);
extern int board_winner(board *self);

#endif