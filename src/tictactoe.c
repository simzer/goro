/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <math.h>

#include "minimax.h"
#include "game.h"
#include "boarditerator.h"

static int validTicTacToeMove(Game *self, BoardCoord coord);
static int ticTacToeGameOver(Game *self);
static PlayerId ticTacToeWinner(Game *self);
static double evalTicTacToePosition(Game *self);

static const GameVirtualTable ticTacToeVirtualTable = {
  &validTicTacToeMove,
  0,
  &ticTacToeGameOver,
  &ticTacToeWinner,
  &evalTicTacToePosition,
  &copyGame
};

Game createTicTacToe(BoardSize size)
{
  Game self = createGame(createBoard(size, size));
  self.vtable = &ticTacToeVirtualTable;
  return self;
}

static int validTicTacToeMove(Game *self, BoardCoord coord)
{
  return getBoardCell(&(self->board), coord) == emptyBoardCell;
}

static int ticTacToeGameOver(Game *self)
{
  return    !boardHasEmptyCell(&self->board)
         || (ticTacToeWinner(self) != noPlayer);
}

static PlayerId ticTacToeWinner(Game *self)
{
  BoardSize size = self->board.width;
  BoardSize i, j;
  PlayerId player;
  BoardCoord coord;
  for(player = firstPlayer; player < numberOfPlayers; player++) {
    int fullLeftDiagonal = 1;
    int fullRightDiagonal = 1;
    for(i = 0; i < size; i++) {
      int fullRow = 1;
      int fullCol = 1;
      for(j = 0; j < size; j++) {
        fullRow &= getBoardCell(&(self->board), createBoardCoord(i, j))
                    == gamePlayerCells[player];
        fullCol &= getBoardCell(&(self->board), createBoardCoord(j, i))
                    == gamePlayerCells[player];
      }
      if (fullRow || fullCol) return(player);

      fullLeftDiagonal &= getBoardCell(&(self->board),
                                        createBoardCoord(i, i))
                          == gamePlayerCells[player];
      fullRightDiagonal &= getBoardCell(&(self->board),
                                        createBoardCoord(i, size - 1 - i))
                           == gamePlayerCells[player];
    }
    if (fullLeftDiagonal || fullRightDiagonal) return(player);
  }
  return(noPlayer);
}

static double evalTicTacToePosition(Game *self)
{
  PlayerId winner = ticTacToeWinner(self);
  return (winner == self->actualPlayer ? miniMaxWinScore :
          winner == noPlayer     ? 0 :
                                      miniMaxLoseScore );
}
