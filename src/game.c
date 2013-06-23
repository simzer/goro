
#include "game.h"

const BoardCell game_playerCells[player_num] = {
  boardCell_black,
  boardCell_white
};

const char *game_playerNames[player_num] = {
  "First player",
  "Second Player"
};

BoardCell game_actPlayerCell(Game *self)
{
  return game_playerCells[self->actualPlayer];
}

Game game_create(Board board)
{
  Game self;
  self.actualPlayer = player_none;
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
  self->actualPlayer = (self->actualPlayer == player_1)
                    ? player_2
                    : player_1;
}

int game_nextValidMove(Game *self, BoardIterator *iterator)
{
  int result;
  do {
    result = boardIterator_next(iterator);
  } while(   (result == 1)
          && !self->vtable->validMove(self, iterator->coord));
  return result;
}
