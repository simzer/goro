/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "go.h"
#include "goro.h"
#include "tictactoe.h"
#include "gomoko.h"
#include "cliplayer.h"
#include "minimax.h"
#include "gtp.h"

static void goGameCreatedByDefault(void) {
  Goro goro = createGoroFromCLIOption(1, 0);
  assert(goro.game->vtable == &goVirtualtable);
}

static void tictactoeCreatedOnTictactoeOption(void) {
  char *argv[] = { "", "--tictactoe" };
  Goro goro = createGoroFromCLIOption(2, argv);
  assert(goro.game->vtable == &ticTacToeVirtualTable);
}

static void gomokoCreatedOnGomokoOption(void) {
  char *argv[] = { "", "--gomoko" };
  Goro goro = createGoroFromCLIOption(2, argv);
  assert(goro.game->vtable == &gomokoVirtualTable);
}

static void boardOptionSetsBoardSize(void) {
  char *argv[] = { "", "--board", "5" };
  Goro goro = createGoroFromCLIOption(3, argv);
  assert(goro.game->board.height == 5);
  assert(goro.game->board.width == 5);
}

static void defaultBoardSizeIs19x19(void) {
  Goro goro = createGoroFromCLIOption(1, 0);
  assert(goro.game->board.height == 19);
  assert(goro.game->board.width == 19);
}

static void tictactoeHas3x3BoardRegardlessBoardOption(void) {
  char *argv[] = { "", "--tictactoe", "--board", "5" };
  Goro goro = createGoroFromCLIOption(4, argv);
  assert(goro.game->board.height == 3);
  assert(goro.game->board.width == 3);
}

static void defaultPlayersAreSet(void) {
  Goro goro = createGoroFromCLIOption(1, 0);
  assert(goro.players[0] != 0);
  assert(goro.players[1] != 0);
}

static void defaultPlayersAreBlackCliWhiteAi(void) {
  Goro goro = createGoroFromCLIOption(1, 0);
  assert(goro.players[0]->getMove == getCLIPlayerMove);
  assert(goro.players[1]->getMove == getMiniMaxMove);
}

static void gameSetForPlayers(void) {
  Goro goro = createGoroFromCLIOption(1, 0);
  assert(goro.players[0]->game == goro.game);
  assert(goro.players[1]->game == goro.game);
}

static void playersSetByBlackWhiteOptions(void) {
  char *argv[] = { "", "--black", "gtp", "--white", "cli" };
  Goro goro = createGoroFromCLIOption(5, argv);
  assert(goro.players[0]->getMove == getGTPMove);
  assert(goro.players[1]->getMove == getCLIPlayerMove);
}

static void defaultKomiSetIfNoOption(void) {
  Goro goro = createGoroFromCLIOption(1, 0);
  assert(((Go*)goro.game)->komi == 6.5);
}

static void komiSetAccordingToKomiOption(void) {
  char *argv[] = { "", "--komi", "-0.5" };
  Goro goro = createGoroFromCLIOption(3, argv);
  assert(((Go*)goro.game)->komi == -0.5);
}

static void noHandicapByDefault(void) {
  char *argv[] = { "", "--handicap", "5" };
  Goro goro = createGoroFromCLIOption(3, argv);
  assert(getBoardCell(&goro.game->board, boardTengen(&goro.game->board))
         == blackBoardCell);
}

static void handicapSetAccordingToHandicapOption(void) {

  Goro goro = createGoroFromCLIOption(1, 0);
  assert(boardIsClear(&goro.game->board));
}
void testgoro(void)
{
  goGameCreatedByDefault();
  tictactoeCreatedOnTictactoeOption();
  gomokoCreatedOnGomokoOption();
  tictactoeHas3x3BoardRegardlessBoardOption();
  defaultBoardSizeIs19x19();
  boardOptionSetsBoardSize();
  defaultPlayersAreSet();
  defaultKomiSetIfNoOption();
  defaultPlayersAreBlackCliWhiteAi();
  gameSetForPlayers();
  playersSetByBlackWhiteOptions();
  komiSetAccordingToKomiOption();
  handicapSetAccordingToHandicapOption();
  noHandicapByDefault();
}
