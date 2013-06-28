/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <string.h>

#include "game.h"
#include "go.h"
#include "boarditerator.h"

static int validGoMove(Game *self, BoardCoord coord);
static int possibleGoMoves(Game *self);
static PlayerId goWinner(Game *self);
static double evalGoPosition(Game *self);

static const GameVirtualTable goVirtualtable = {
  &validGoMove,
  &possibleGoMoves,
  &goWinner,
  &evalGoPosition
};

/*
switchGamePlayer:
  player moves
  remove every group of other player without empty neighbour
  remove every white group without empty neighbour
*/

Game createGo(Board board)
{
  // to be implemented
}

static int validGoMove(Game *self, BoardCoord coord)
{
  // to be implemented
  /*
    move is on empty cell
    move not dies instantly
    not results the same position as the one before previous
  */
}

static int possibleGoMoves(Game *self)
{
  // to be implemented
}

static PlayerId goWinner(Game *self)
{
  // to be implemented
}

static double evalGoPosition(Game *self)
{
  // to be implemented
}

