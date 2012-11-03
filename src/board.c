
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"

const char board_signs[] = { ' ', 'X', 'O'};

board *board_create(int width,
                    int height)
{
  board *self = malloc(sizeof(board));
  if (self == NULL) return(NULL);
  self->width  = width;
  self->height = height;
  self->board  = malloc(width * height * sizeof(int));
  board_clear(self);
  return(self);
}

void board_clear(board *self)
{
  memset(self->board, 0, self->width * self->height * sizeof(int));
}

board *board_copy(board *self)
{
  board *res = board_create(self->width, self->height);
  if (res == NULL) return (NULL);
  memcpy(res->board, self->board, self->width * self->height * sizeof(int));
  return(res);
}

void board_destruct(board *self)
{
  free(self->board);
  free(self);
}

int board_get_cell(board *self, int row, int col)
{
  if (   row < 0 || row >= self->height
      || col < 0 || col >= self->width) {
    return(board_invalid_cell);
  } else {
    return(self->board[row*self->width + col]);
  }
}

int board_set_cell(board *self, int row, int col, int cell)
{
  if (   row < 0 || row >= self->height
      || col < 0 || col >= self->width)
    return(1);

  self->board[row*self->width + col] = cell;
  return(0);
}

void board_print(board *self)
{
  int row, col;
  for(row = 0; row < self->height; row++)
  {
    for(col = 0; col < self->width; col++)
    {
      printf("%c ", board_signs[board_get_cell(self, row, col)]);
    }
    printf("\n");
  }
}

void board_save(FILE *file, board *self)
{
  int width  = self->width;
  int height = self->height;
  if(    ( fwrite(&width,       sizeof(int), 1, file) == 1 )
      && ( fwrite(&height,      sizeof(int), 1, file) == 1 )
      && ( fwrite(self->board, sizeof(int), width*height, file) == width*height) )
  {
    return;
  }
  else
  {
    fprintf(stderr,"Can not save board file.");
    return;
  }
}

board *board_load(FILE *file)
{
  int width, height;
  board *self;
  if (   (fread(&width,  sizeof(int), 1, file) == 1)
      && (fread(&height, sizeof(int), 1, file) == 1) )
  {
    self = board_create(width, height);
    if (fread(self->board, sizeof(int), width*height, file) == width*height)
    {
      return(self);
    }
    board_destruct(self);
  }
  printf(stderr,"Can not load board file.");
  return(NULL);
}


int board_validMove(int i, int j, board *self)
{ 
/// to be implemented
}

int board_movesPossible(board *self)
{
/// to be implemented
}

player board_winner(board *self)
{
/// to be implemented
}
