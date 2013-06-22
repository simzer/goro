
#include "game.h"
#include "boarditerator.h"

static int tictactoe_validMove(Game *self, BoardCoord coord);
static int tictactoe_movesPossible(Game *self);
static Player tictactoe_winner(Game *self);

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

static int tictactoe_movesPossible(Game *self)
{
  return board_isThereEmptyCell(&self->board);
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
