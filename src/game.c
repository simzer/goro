
#include "game.h"

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
