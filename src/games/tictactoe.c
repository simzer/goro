/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <math.h>

#include "minimax.h"
#include "tictactoe.h"
#include "boarditerator.h"

static int playerWon(TicTacToe *self, PlayerId player);
static int anyCrossingLineFilled(TicTacToe *self, BoardCoord coord, BoardCell cell);
static int anyDiagonalFilled(TicTacToe *self, BoardCell cell);

static int ticTacToeGameOver(TicTacToe *self);
static PlayerId ticTacToeWinner(TicTacToe *self);
static double evalTicTacToePosition(TicTacToe *self);
static int ticTacToeMoveWorthChecking(Game *self, GameMove move);

const GameVirtualTable ticTacToeVirtualTable = {
  &genericGameMove,
  &validGameMove,
  &ticTacToeMoveWorthChecking,
  &ticTacToeGameOver,
  &ticTacToeWinner,
  &evalTicTacToePosition,
  &copyGame,
  0,
  &printGameStatus
};

TicTacToe createTicTacToe(BoardSize size)
{
  TicTacToe self = createGame(createBoard(size, size));
  self.vtable = &ticTacToeVirtualTable;
  return self;
}

static int ticTacToeMoveWorthChecking(Game *self, GameMove move) {
  return move.type == playMove;
}

static int ticTacToeGameOver(TicTacToe *self)
{
  return    genericGameOver(self)
         || (ticTacToeWinner(self) != noPlayer);
}

static PlayerId ticTacToeWinner(TicTacToe *self) // todo: change to void checkWinner
{
  PlayerId player;
  for(player = firstPlayer; player < numberOfPlayers; player++) {
    if(self->winner == noPlayer && playerWon(self, player)) {
      self->winner = player;
      break;
    }
  }
  return(self->winner);
}

static int playerWon(TicTacToe *self, PlayerId player)
{
  BoardSize i;
  for(i = 0; i < self->board.width; i++) {
    if(anyCrossingLineFilled(self, createBoardCoord(i, i),
                             gamePlayerCells[player]))
      return(1);
  }
  if(anyDiagonalFilled(self, gamePlayerCells[player])) return 1;
  return 0;
}

static int anyCrossingLineFilled(TicTacToe *self, BoardCoord coord, BoardCell cell)
{
  int j;
  int fullRow = 1;
  int fullCol = 1;
  for(j = 0; j < self->board.width; j++) {
    fullRow &= getBoardCell(&(self->board), createBoardCoord(coord.col, j))
                == cell;
    fullCol &= getBoardCell(&(self->board), createBoardCoord(j, coord.row))
                == cell;
  }
  return fullRow || fullCol;
}

static int anyDiagonalFilled(TicTacToe *self, BoardCell cell)
{
  BoardSize i;
  int fullLeftDiagonal = 1;
  int fullRightDiagonal = 1;
  for(i = 0; i < self->board.width; i++) {
    fullLeftDiagonal &=
        getBoardCell(&(self->board),
                     createBoardCoord(i, i)) == cell;
    fullRightDiagonal &=
        getBoardCell(&(self->board),
                     createBoardCoord(i, self->board.width - 1 - i)) == cell;
  }
  return fullLeftDiagonal || fullRightDiagonal;
}

static double evalTicTacToePosition(TicTacToe *self)
{
  return (self->winner == self->lastPlayer ? miniMaxLoseScore :
          self->winner == noPlayer         ? 0 :
                                             miniMaxWinScore );
}
