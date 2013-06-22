
#include "game.h"

const BoardCell game_playerCells[player_num] = {
  boardCell_black,
  boardCell_white
};

BoardCell game_actPlayerCell(Game *self)
{
  return game_playerCells[self->actPlayer];
}

Game game_create(Board board)
{
  Game self;
  self.actPlayer = player_none;
  self.board = board;
  return self;
}

Game game_copy(Game *self)
{
  Game res = *self;
  res.board = board_copy(&(self->board));
  return(res);
}

void game_switchPlayer(Game *self)
{
  self->actPlayer = (self->actPlayer == player_1)
                    ? player_2
                    : player_1;
}
