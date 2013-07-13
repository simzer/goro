/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "tictactoe.h"

void testtictactoe(void)
{
  TicTacToe game = createTicTacToe(3);
  MoveIterator iterator = createMoveIterator(&game.board);
  startGame(&game);
  while(game.status != finishedStatus) {
    assert(nextMoveWorthChecking(&game, &iterator));
    game.vtable->move(&game, iterator.move);
    game.vtable->printGameStatus(&game);
  }
}
