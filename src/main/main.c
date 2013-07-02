/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "game.h"
#include "tictactoe.h"
#include "gomoko.h"
#include "go.h"
#include "cliplayer.h"
#include "minimax.h"
#include "gtp.h"
#include "testplayer.h"
#include "about.h"
#include "options.h"

typedef struct {
  CliOptionParser options;
  Game *game;
  Player *players[2];
} Goro;
static void runGoro(Goro *self);
static Goro createGoro(int argc, char *argv[]);
static void destructGoro(Goro *self);
static void initGoroGameFromCLI(Goro *self);
static void initGoroPlayersFromCLI(Goro *self);
static void setDefaultNotInitializedPlayers(Goro *self);
static void goParametersFromCLI(Goro *self);
static int boardSizeFromCLI(Goro *self);
static void printHelp(void);
static Player *createPlayerByName(Goro *self, char *type);

int main(int argc, char *argv[])
{
  Goro goro = createGoro(argc, argv);
  runGoro(&goro);
  destructGoro(&goro);
  return 0;
}

static Goro createGoro(int argc, char *argv[])
{
  Goro self;
  self.options = createCliOptionParser(argc, argv);
  if(cliOptionSet(&self.options, "--help")) {
    printHelp();
    exit(0);
  }
  initGoroGameFromCLI(&self);
  self.players[0] = 0;
  self.players[1] = 0;
  initGoroPlayersFromCLI(&self);
  setDefaultNotInitializedPlayers(&self);
  return self;
}

static void destructGoro(Goro *self)
{
  if(self->game) {
    destructGame(self->game);
    self->game = 0;
  }
  if(self->players[0]) free(self->players[0]); self->players[0] = 0;
  if(self->players[1]) free(self->players[1]); self->players[1] = 0;
}

static void runGoro(Goro *self)
{
  while(!self->game->vtable->over(self->game))
  {
    GameMove move;
    move = self->players[self->game->actualPlayer]
                ->getMove(self->players[self->game->actualPlayer]);
    self->game->vtable->move(self->game, move);
  }
  printGameOverInfo(self->game);
}

static void initGoroPlayersFromCLI(Goro *self)
{
  PlayerId player;
  char *playerCliOption[] = { "--black", "--white" };
  for(player = firstPlayer; player == secondPlayer; player++) {
    if(cliOptionSet(&self->options, playerCliOption[player])) {
      self->players[firstPlayer] =
          createPlayerByName(self,
                             getCliOptionValue(&self->options,
                                               playerCliOption[player]));
    }
  }
}

static Player *createPlayerByName(Goro *self, char *type)
{
  Player *player;
  if (strcmp(type, "ai") == 0) {
    player = malloc(sizeof(MiniMax));
    *(MiniMax *)player = createMiniMax(self->game);
  }
  else if (strcmp(type, "cli") == 0)  {
    player  = malloc(sizeof(CLIPlayer));
    *(CLIPlayer *)player  = createCLIPlayer(self->game);
  }
  else if (strcmp(type, "gtp") == 0) {
    player  = malloc(sizeof(GTP));
    *(GTP *)player  = createGTP(self->game);
  }
  else if (strcmp(type, "test") == 0) {
    player  = malloc(sizeof(TestPlayer));
    *(TestPlayer *)player = createTestPlayer(self->game);
  }
  else {
    fprintf(stderr, "Player type not recognized\n");
    exit(1);
  }
  return player;
}

static void setDefaultNotInitializedPlayers(Goro *self)
{
  if (self->players[firstPlayer] == 0) {
    self->players[firstPlayer] = malloc(sizeof(CLIPlayer));
    *(CLIPlayer *)(self->players[firstPlayer]) = createCLIPlayer(self->game);
  }
  if (self->players[secondPlayer] == 0) {
    self->players[secondPlayer] = malloc(sizeof(MiniMax));
    *(MiniMax *)(self->players[secondPlayer]) = createMiniMax(self->game);
  }
}

static void initGoroGameFromCLI(Goro *self)
{
  if(cliOptionSet(&self->options, "--tictactoe")) {
    self->game = malloc(sizeof(TicTacToe));
    *(TicTacToe *)self->game = createTicTacToe(standardBoardSizes[nanoBoard]);
  } else
  if(cliOptionSet(&self->options, "--gomoko")) {
    self->game = malloc(sizeof(Gomoko));
    *(Gomoko *)self->game = createGomoko(
        createSquareBoard(standardBoardSizes[normalBoard]));
  } else {
    self->game = malloc(sizeof(Go));
    *(Go *)self->game = createGo(createSquareBoard(boardSizeFromCLI(self)));
    goParametersFromCLI(self);
  }
}

static void goParametersFromCLI(Goro *self)
{
  if(cliOptionSet(&self->options, "--handicap")) {
    setGoHandicap(self->game,
                  atoi(getCliOptionValue(&self->options, "--handicap")));
  }
  if(cliOptionSet(&self->options, "--komi")) {
    ((Go *)self->game)->komi =
        atof(getCliOptionValue(&self->options, "--komi"));
  }
}

static int boardSizeFromCLI(Goro *self)
{
  if(cliOptionSet(&self->options, "--board")) {
    return atoi(getCliOptionValue(&self->options, "--board"));
  }
  return standardBoardSizes[normalBoard];
}

static void printHelp(void)
{
  printf("%s - %s\n", programName, programVersion);
  printf("%s\n\n", programDescription);
  printf("Options:\n\n");
  printf("%s\n\n", programCLIOptions);
}
