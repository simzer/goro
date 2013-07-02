/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include  "stdlib.h"
#include  "string.h"
#include  "testplayer.h"

static BoardCoord getTestPlayerMove(TestPlayer *self);

TestPlayer createTestPlayer(Game *game)
{
  TestPlayer self;
  self.player.game = game;
  self.player.getMove = &getTestPlayerMove;
  return self;
}

static BoardCoord getTestPlayerMove(TestPlayer *self)
{
  BoardCoord coord;
  Game* game = ((Player *)self)->game;
  if (boardCoordsEqual(game->lastMove, nullBoardCoord)) {
    coord = boardTengen(&self->player.game->board);
  } else {
    coord = mirrorBoardCoord(&self->player.game->board, game->lastMove);
  }
  while (!game->vtable->validMove(game, coord)) {
    coord = createBoardCoord(rand()%(game->board.height),
                             rand()%(game->board.width));
  };
  return(coord);
}
