
#include "game.h"

void game_init(game *self)
{
  self->actPlayer = PLAYER_NONE;
}

void game_switchPlayer(game *self)
{
  self->actPlayer = (self->actPlayer == PLAYER_X)
                    ? PLAYER_O 
                    : PLAYER_X;
}
