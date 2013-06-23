
#include "game.h"
#include "boarditerator.h"

static int tictactoe_validMove(Game *self, BoardCoord coord);
static int tictactoe_movesPossible(Game *self);
static Player tictactoe_winner(Game *self);
static double tictactoe_evalPosition(Game *self);

static const Game_vtable tictactoe_vtable = {
  &tictactoe_validMove,
  &tictactoe_movesPossible,
  &tictactoe_winner,
  &tictactoe_evalPosition
};

Game tictactoe_create(BoardSize size)
{
  Game self = game_create(board_create(size, size));
  self.vtable = &tictactoe_vtable;
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

static double tictactoe_evalPosition(Game *self)
{
  Player winner = self->vtable->winner(self);
  return (winner == self->actPlayer ? +1 :
          winner == player_none     ?  0 :
                                      -1 );
}
