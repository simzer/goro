
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "board.h"

const char board_signs[] = { ' ', '0', 'O'};

Board board_create(BoardSize width,
                   BoardSize height)
{
  Board self;
  self.width  = width;
  self.height = height;
  self.board  = malloc(width * height * sizeof(BoarCellContainer));
  assert(self.board != 0);
  board_clear(&self);
  return self;
}

void board_clear(Board *self)
{
  memset(self->board, 0,
         self->width * self->height * sizeof(BoarCellContainer));
}

Board board_copy(Board *self)
{
  Board res = board_create(self->width, self->height);
  memcpy(res.board, self->board,
         self->width * self->height * sizeof(BoarCellContainer));
  return(res);
}

void board_destruct(Board *self)
{
  free(self->board);
}

BoardCell board_getCell(Board *self, BoardSize row, BoardSize col)
{
  if (   row < 0 || row >= self->height
      || col < 0 || col >= self->width) {
    return(boardCell_invalid);
  } else {
    return(self->board[row*self->width + col]);
  }
}

int board_setCell(Board *self, BoardSize row, BoardSize col, BoardCell cell)
{
  if (   row < 0 || row >= self->height
      || col < 0 || col >= self->width)
    return(1);

  self->board[row*self->width + col] = cell;
  return(0);
}

void board_print(Board *self)
{
  BoardSize row, col;
  for(row = 0; row < self->height; row++)
  {
    for(col = 0; col < self->width; col++)
    {
      printf("%c ", board_signs[board_getCell(self, row, col)]);
    }
    printf("\n");
  }
}

void board_save(FILE *file, Board *self)
{
  BoardSize width  = self->width;
  BoardSize height = self->height;
  if(    ( fwrite(&width,       sizeof(int), 1, file) == 1 )
      && ( fwrite(&height,      sizeof(int), 1, file) == 1 )
      && ( fwrite(self->board, sizeof(BoarCellContainer),
                  width*height, file) == width*height) )
  {
    return;
  }
  else
  {
    fprintf(stderr,"Can not save board file.");
    return;
  }
}

Board board_load(FILE *file)
{
  BoardSize width, height;
  Board self;
  if (   (fread(&width,  sizeof(int), 1, file) == 1)
      && (fread(&height, sizeof(int), 1, file) == 1) )
  {
    self = board_create(width, height);
    if (   fread(self.board, sizeof(BoarCellContainer), width*height, file)
        == width*height)
    {
      return(self);
    }
    board_destruct(&self);
  }
  fprintf(stderr,"Can not load board file.");
  assert(0);
  return(self);
}


int board_validMove(BoardSize row, BoardSize col, Board *self)
{ 
/// to be implemented
}

int board_movesPossible(Board *self)
{
/// to be implemented
}

BoardCell board_winner(Board *self)
{
/// to be implemented
}
