
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "board.h"
#include "boarditerator.h"

const BoardCoord boardCoord_null = { 0, 0 };

const char board_signs[] = { '.', '@', 'O'};

BoardCoord boardCoord_create(BoardSize row, BoardSize col) {
  BoardCoord self;
  self.row = row;
  self.col = col;
  return self;
}

Board board_create(BoardSize width,
                   BoardSize height)
{
  Board self;
  self.width  = width;
  self.height = height;
  self.board  = malloc(width * height * sizeof(self.board[0]));
  assert(self.board != 0);
  board_clear(&self);
  return self;
}

void board_clear(Board *self)
{
  memset(self->board, boardCell_empty,
         self->width * self->height * sizeof(self->board[0]));
}

Board board_copy(Board *self)
{
  Board res = board_create(self->width, self->height);
  memcpy(res.board, self->board,
         self->width * self->height * sizeof(self->board[0]));
  return(res);
}

void board_destruct(Board *self)
{
  free(self->board);
}

int board_coordInBoard(Board *self, BoardCoord coord)
{
  return (   coord.row >= 0 && coord.row < self->height
          && coord.col >= 0 && coord.col < self->width);
}

BoardCell board_getCell(Board *self, BoardCoord coord)
{
  if(board_coordInBoard(self, coord))
    return self->board[coord.row*self->width + coord.col];
  return boardCell_invalid;
}

void board_setCell(Board *self, BoardCoord coord, BoardCell cell)
{
  assert(board_coordInBoard(self, coord));
  self->board[coord.row * self->width + coord.col] = cell;
}

void board_print(Board *self)
{
  BoardIterator iter = boardIterator_create(self);
  for(;boardIterator_next(&iter);)
  {
    printf("%c ", board_signs[board_getCell(self, iter.coord)]);
    if (iter.coord.col == self->width-1) printf("\n");
  }
}

void board_save(Board *self, FILE *file)
{
  BoardSize width  = self->width;
  BoardSize height = self->height;
  if(    ( fwrite(&width,       sizeof(int), 1, file) == 1 )
      && ( fwrite(&height,      sizeof(int), 1, file) == 1 )
      && ( fwrite(self->board, sizeof(self->board[0]),
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
    if (   fread(self.board, sizeof(self.board[0]), width*height, file)
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
