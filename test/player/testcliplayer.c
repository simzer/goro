/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>
#include <unistd.h>

#include "tictactoe.h"
#include "cliplayer.h"

static void cliPlayerValidMoveAccepted(int output) {
  TicTacToe game = createTicTacToe(3);
  CLIPlayer player  = createCLIPlayer(&game);
  startGame(&game);
  write(output, "a1\n", strlen("a1\n"));
  GameMove move = getCLIPlayerMove(&player);
  write(output, "pass\n", strlen("pass\n"));
  move = getCLIPlayerMove(&player);
  write(output, "res\n", strlen("res\n"));
  move = getCLIPlayerMove(&player);
  assert(gameMoveEqual(move, createResignMove()));
}

static void cliPlayerInvalidMoveIgnored(int output) {
  TicTacToe game = createTicTacToe(3);
  CLIPlayer player  = createCLIPlayer(&game);
  startGame(&game);
  write(output, "blabla\na1\n", strlen("blabla\na1\n"));
  GameMove move = getCLIPlayerMove(&player);
  assert(gameMoveEqual(move, createPlayMove(createBoardCoord(0,0))));
}

void testcliplayer(void)
{
  int pipeFD[2];
  assert(pipe(pipeFD) != -1);

  dup2(pipeFD[0], STDIN_FILENO);
  cliPlayerValidMoveAccepted(pipeFD[1]);
  cliPlayerInvalidMoveIgnored(pipeFD[1]);

  close(pipeFD[0]);
  close(pipeFD[1]);
}
