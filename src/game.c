
#include "game.h"

const BoardCell game_playerCells[player_num] = {
  boardCell_black,
  boardCell_white
};

BoardCell game_actPlayerCell(Game *self)
{
  return game_playerCells[self->actPlayer];
}

Game game_create()
{
  Game self;
  self.actPlayer = player_none;
  return self;
}

void game_switchPlayer(Game *self)
{
  self->actPlayer = (self->actPlayer == player_1)
                    ? player_2
                    : player_1;
}
