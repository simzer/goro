/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "about.h"
#include "go.h"
#include "gtp.h"

static GameMove getGTPMove(GTP *self);
static void processCommand(GTP *self, char *command, char *argument);
static void sendLastMove(GTP *self);
static void warning(GTP *self, char *message);
static void response(GTP *self, char *message);

static void printKnownCommands(GTP *self, char *argument);
static void printName(GTP *self, char *argument);
static void printVersion(GTP *self, char *argument);
static void quit(GTP *self, char *argument);
static void setBoardSize(GTP *self, char *argument);
static void resetBoard(GTP *self, char *argument);
static void play(GTP *self, char *argument);
static void placeFreeHandicap(GTP *self, char *argument);
static void setFreeHandicap(GTP *self, char *argument);
static void generateMove(GTP *self, char *argument);

static const char GTPWarningCode = '?';
static const char GTPResponseCode = '=';

static const struct {
  char* command;
  void (*handler)(GTP *self, char *argument);
} GTPCommandTable[] = {
  { "name",                &printName },
  { "version",             &printVersion },
  { "quit",                &quit },
  { "list_commands",       &printKnownCommands },
  { "boardsize",           &setBoardSize },
  { "clear_board",         &resetBoard },
  { "play",                &play },
  { "genmove",             &generateMove },
  { "set_free_handicap",   &setFreeHandicap },
  { "place_free_handicap", &placeFreeHandicap }
};

GTP createGTP(Game *game)
{
  GTP self;
  self.player.game = game;
  self.player.getMove = &getGTPMove;
  self.ID = 0;
  self.moveRequested = 0;
  self.lastSentMove = nullMove;
  self.nextMove = nullMove;
  setvbuf(stdout, NULL, _IONBF, 0);
  return self;
}

static int lastMoveNotSent(GTP *self) {
  return !gameMoveEqual(self->lastSentMove,
                        self->player.game->lastMove);
}

static GameMove getGTPMove(GTP *self)
{
  char line[1024];
  char command[64];
  char argument[1024];
  if(self->moveRequested && lastMoveNotSent(self)) sendLastMove(self);
  // todo: can be a number at the begining, not handled!
  while(scanf("%[^\n]%*c",line)>0) {
    sscanf(line, "%s %[^\n]%*c",command, argument);
    processCommand(self, command, argument);
    if(self->moveRequested) {
      if(lastMoveNotSent(self)) sendLastMove(self); else break;
    }
  }
  return(self->nextMove);
}

static void sendLastMove(GTP *self) {
  if(self->player.game->lastMove != invalidMove) {
    GameMoveString lastMove =
      gameMoveToString(self->player.game->lastMove);
    response(self, lastMove.chars);
    self->lastSentMove = self->player.game->lastMove;
    self->moveRequested = 0;
  }
}

static void processCommand(GTP *self, char *command, char *argument)
{
  int i;
  for(i = 0;
      i < sizeof(GTPCommandTable)/sizeof(GTPCommandTable[0]);
      i++)
  {
    if(strcmp(command, GTPCommandTable[i].command) == 0) {
      GTPCommandTable[i].handler(self, argument);
      return;
    }
  }
  warning(self, "unknown command: $command");
}

static void warning(GTP *self, char *message) {
  printf("%c %s\n\n", GTPWarningCode, message);
}

static void response(GTP *self, char *message) {
  printf("%c %s\n\n", GTPResponseCode, message);
}

static void printKnownCommands(GTP *self, char *argument) {
  int i;
  char *list = malloc(1);
  list[0] = '\0';
  for(i = 0;
      i < sizeof(GTPCommandTable)/sizeof(GTPCommandTable[0]);
      i++)
  {
    list = realloc(list,   strlen(list)
                         + strlen(GTPCommandTable[i].command) + 2);
    strcat(list, GTPCommandTable[i].command);
    strcat(list, "\n");
  }
  response(self, list);
  free(list);
}

static void printName(GTP *self, char *argument) {
  response(self, programName);
}

static void printVersion(GTP *self, char *argument) {
  response(self, programVersion);
}

static void quit(GTP *self, char *argument) {
  exit(0);
}

static void setBoardSize(GTP *self, char *argument) {
  BoardSize size;
  if(sscanf(argument, "%d", &size) == 1) {
    self->player.game->board = createBoard(size, size);
    response(self, "");
  } else {
    warning(self, "boardsize not recognized.");
  }
}

static void resetBoard(GTP *self, char *argument) {
  clearBoard(&self->player.game->board);
  response(self, "");
}

static void play(GTP *self, char *argument) {
  char move[7];
  // (B|W|black|white) (\w\d+|pass|resign)
  if(sscanf(argument, "%*s %6s", move) != 1) {
    goto error;
  } else if(strcmp(move, "resign") == 0) {
    self->nextMove = createResignMove();
  } else if(strcmp(move, "pass") == 0) {
    self->nextMove = createPassMove();
  } else {
    GameMoveString string;
    strcpy(string.chars, move);
    self->nextMove = stringToGameMove(string);
  }
  response(self, "");
  return;
error:
  warning(self, "play syntax not recognized.");
}

static void placeFreeHandicap(GTP *self, char *argument) {
  int handicap, i;
  char message[40];
  scanf("%d", &handicap);
  setGoHandicap((Go *)(self->player.game), handicap);
  BoardCoordString *s = standardGoHandicaps[handicap-1];
  sprintf(message, "%s %s %s %s %s %s %s %s %s",
          s[0].chars, s[1].chars, s[2].chars,
          s[3].chars, s[4].chars, s[5].chars,
          s[6].chars, s[7].chars, s[8].chars);
  response(self, message);
}

static void setFreeHandicap(GTP *self, char *argument) {
  int i;
  BoardCoordString s[9];
  int handicap = scanf("%s %s %s %s %s %s %s %s %s", argument,
                      s[0].chars, s[1].chars, s[2].chars,
                      s[3].chars, s[4].chars, s[5].chars,
                      s[6].chars, s[7].chars, s[8].chars);
  for(i = 0; i <  handicap; i++) {
    setBoardCell(&self->player.game->board,
                 stringToBoardCoord(s[i]),
                 blackBoardCell);
    self->player.game->actualPlayer = secondPlayer;
  }
  response(self, "");
}

static void generateMove(GTP *self, char *argument) {
  self->moveRequested = 1;
  return;
}
