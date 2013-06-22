
#include "game.h"
#include "boarditerator.h"

static Game game_create(Board board);

static int tictactoe_validMove(Game *self, BoardCoord coord);
static int tictactoe_movesPossible(Game *self);
static Player tictactoe_winner(Game *self);

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

Game tictactoe_create(BoardSize size)
{
  Game self = game_create(board_create(size, size));
  self.movesPossible = &tictactoe_movesPossible;
  self.validMove = &tictactoe_validMove;
  self.winner = &tictactoe_winner;
  return self;
}

static int tictactoe_validMove(Game *self, BoardCoord coord)
{
  return board_getCell(&(self->board), coord) == boardCell_empty;
}

// is this valid for any game?
static int tictactoe_movesPossible(Game *self)
{
  int valid = 0;
  BoardIterator iter = boardIterator_create(&self->board);
  for(;boardIterator_next(&iter);) {
    valid |= tictactoe_validMove(self, iter.coord);
  }
  return(valid);
}

static Player tictactoe_winner(Game *self)
{
  BoardSize size = self->board.width;
  BoardSize i, j;
  Player player;
  BoardCoord coord;
  for(player = player_1; player < player_num; player++) {
    int fullLeftDiagonal = 1;
    int fullRightDiagonal = 1;
    for(i = 0; i < size; i++) {
      int fullRow = 1;
      int fullCol = 1;
      for(j = 0; j < size; j++) {
        fullRow &= board_getCell(&(self->board), boardCoord_create(i, j))
                    == game_playerCells[player];
        fullCol &= board_getCell(&(self->board), boardCoord_create(j, i))
                    == game_playerCells[player];
      }
      if (fullRow || fullCol) return(player);

      fullLeftDiagonal &= board_getCell(&(self->board),
                                        boardCoord_create(i, i))
                          == game_playerCells[player];
      fullRightDiagonal &= board_getCell(&(self->board),
                                        boardCoord_create(i, size - 1 - i))
                           == game_playerCells[player];
    }
    if (fullLeftDiagonal || fullRightDiagonal) return(player);
  }
  return(player_none);
}
