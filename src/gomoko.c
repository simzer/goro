
#include "game.h"
#include "boarditerator.h"

static int gomoko_validMove(Game *self, BoardCoord coord);
static int gomoko_movesPossible(Game *self);
static Player gomoko_winner(Game *self);
static double gomoko_evalPosition(Game *self);

static const Game_vtable gomoko_vtable = {
  &gomoko_validMove,
  &gomoko_movesPossible,
  &gomoko_winner,
  &gomoko_evalPosition
};

Game gomoko_create(Board board)
{
  Game self = game_create(board);
  self.vtable = &gomoko_vtable;
  return self;
}

static int gomoko_validMove(Game *self, BoardCoord coord)
{
  return board_getCell(&(self->board), coord) == boardCell_empty;
}

static int gomoko_movesPossible(Game *self)
{
  return board_isThereEmptyCell(&self->board);
}

static Player gomoko_winner(Game *self)
{
  BoardCoord neighbours[] = {             { 1, -1} ,
                                          { 1,  0} ,
                               { 0,  1} , { 1,  1} ,
  };
  Player player;
  for(player = player_1; player < player_num; player++) {
    BoardIterator iter = boardIterator_create(&self->board);
    for(;boardIterator_next(&iter);) {
      int i;
      for(i = 0; i < sizeof(neighbours)/sizeof(neighbours[0]); i++) {
        int j;
        int fiveInARow = 1;
        for(j = 0; j < 5; j++) {
          BoardCoord coord = iter.coord;
          coord.col += j * neighbours[i].col;
          coord.row += j * neighbours[i].row;
          fiveInARow &=
             board_getCell(&(self->board), coord) == game_playerCells[player];
        }
        if (fiveInARow) return(player);
      }
    }
  }
  return(player_none);
}

static double gomoko_evalPosition(Game *self) {
  return 0;
}
