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

typedef struct {
  struct {
  int size;
  char **item;
  } cliArgument;
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
static PlayerId playerIdFromCLIOption(char *arg);
static int boardSizeFromCLI(Goro *self);

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
  self.cliArgument.size = argc;
  self.cliArgument.item = argv;
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
    BoardCoord coord;
    BoardCoordString string;
    coord = self->players[self->game->actualPlayer]
                ->getMove(self->players[self->game->actualPlayer]);
    self->game->vtable->move(self->game, coord);
  }
  printGameOverInfo(self->game);
}

static void initGoroPlayersFromCLI(Goro *self)
{
  int index = self->cliArgument.size;
  index--;
  while((--index) >= 0) {
    PlayerId playerId = playerIdFromCLIOption(self->cliArgument.item[index]);
    if(playerId != noPlayer) {
      if (strcmp(self->cliArgument.item[index+1], "ai") == 0) {
        self->players[playerId] = malloc(sizeof(MiniMax));
        *(MiniMax *)(self->players[playerId]) = createMiniMax(self->game);
      } else if (strcmp(self->cliArgument.item[index+1], "cli") == 0) {
        self->players[playerId] = malloc(sizeof(CLIPlayer));
        *(CLIPlayer *)(self->players[playerId]) = createCLIPlayer(self->game);
      } else {
        fprintf(stderr, "Player type not recognized\n");
        exit(1);
      }
    }
  }
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

static PlayerId playerIdFromCLIOption(char *arg) {
  if (strcmp(arg, "--black") == 0) return firstPlayer;
  if (strcmp(arg, "--white") == 0) return secondPlayer;
  return noPlayer;
}

static void initGoroGameFromCLI(Goro *self)
{
  int index = self->cliArgument.size;
  self->game = 0;
  while((--index) >= 0)
  {
    if (strcmp(self->cliArgument.item[index], "--tictactoe") == 0) {
      self->game = malloc(sizeof(TicTacToe));
      *(TicTacToe *)self->game = createTicTacToe(standardBoardSizes[nanoBoard]);
    } else if (strcmp(self->cliArgument.item[index], "--gomoko") == 0) {
      self->game = malloc(sizeof(Gomoko));
      *(Gomoko *)self->game = createGomoko(
          createSquareBoard(standardBoardSizes[normalBoard]));
    }
  }
  if(self->game == 0) {
    self->game = malloc(sizeof(Go));
    *(Go *)self->game = createGo(createSquareBoard(boardSizeFromCLI(self)));
    goParametersFromCLI(self);
  }
}

static void goParametersFromCLI(Goro *self)
{
  int index = self->cliArgument.size;
  index--;
  while((--index) >= 0)
    if (strcmp(self->cliArgument.item[index], "--handicap") == 0)
  {
    setGoHandicap(self->game, atoi(self->cliArgument.item[index+1]));
  }
}


static int boardSizeFromCLI(Goro *self)
{
  int index = self->cliArgument.size;
  index--;
  while((--index) >= 0)
    if (strcmp(self->cliArgument.item[index], "--board") == 0)
  {
    return atoi(self->cliArgument.item[index+1]);
  }
  return standardBoardSizes[normalBoard];
}
