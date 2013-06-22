
#include "game.h"

static Game game_create(Board board);

static int gomoko_validMove(Game *self, BoardCoord coord);
static int gomoko_movesPossible(Game *self);
static Player gomoko_winner(Game *self);

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

Game gomoko_create(Board board)
{
  Game self = game_create(board);
  self.movesPossible = &gomoko_movesPossible;
  self.validMove = &gomoko_validMove;
  self.winner = &gomoko_winner;
  return self;
}

static int gomoko_validMove(Game *self, BoardCoord coord)
{
/// to be implemented
  return(1);
}

static int gomoko_movesPossible(Game *self)
{
/// to be implemented
  return(1);
}

static Player gomoko_winner(Game *self)
{
/// to be implemented
  return(boardCell_invalid);
}
