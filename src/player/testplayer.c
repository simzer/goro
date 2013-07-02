/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include  "stdlib.h"
#include  "string.h"
#include  "testplayer.h"

static GameMove getTestPlayerMove(TestPlayer *self);

TestPlayer createTestPlayer(Game *game)
{
  TestPlayer self;
  self.player.game = game;
  self.player.getMove = &getTestPlayerMove;
  return self;
}

static GameMove getTestPlayerMove(TestPlayer *self)
{
  GameMove move;
  Game* game = ((Player *)self)->game;
  if (game->lastMove.type == invalidMove) {
    move = createPlayMove(boardTengen(&self->player.game->board));
  } else if (game->lastMove.type == passMove) {
    move = createPassMove();
  } else {
    move = createPlayMove(
             mirrorBoardCoord(&self->player.game->board, game->lastMove.coord));
  }
  while (!game->vtable->validMove(game, move)) {
    move = createPlayMove(createBoardCoord(rand()%(game->board.height),
                                           rand()%(game->board.width)));
  };
  return(move);
}
