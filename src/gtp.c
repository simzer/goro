/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "about.h"
#include "gtp.h"

static BoardCoord getGTPMove(GTP *self);

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
  setvbuf(stdout, NULL, _IONBF, 0);
  return self;
}

static BoardCoord getGTPMove(GTP *self)
{
  BoardCoord coord;
  //todo
  /*
  (?:\d+\s+)*(\w+)(.*)/i;
  my ($command, $args) = ($1, $2);
  */
  return(coord);
}

static void warning(GTP *self, char *message) {
  printf("%c %s\n\n", GTPWarningCode, message);
}

static void response(GTP *self, char *message) {
  printf("%c %s\n\n", GTPResponseCode, message);
}

static void processCommand(GTP *self, char *command, char *argument) {
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

static void printKnownCommands(GTP *self, char *argument) {
  int i;
  char *list = malloc(1);
  list[0] = '\0';
  for(i = 0;
      i < sizeof(GTPCommandTable)/sizeof(GTPCommandTable[0]);
      i++)
  {
    list = realloc(list,   strlen(list)
                         + strlen(GTPCommandTable[i].command) + 1);
    strcat(list, strlen(GTPCommandTable[i].command));
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
    // todo
  } else if(strcmp(move, "pass") == 0) {
    // todo
  } else {
    BoardCoord coord;
    BoardCoordString string;
    strcpy(string.chars, move);
    coord = stringToBoardCoord(string);
  }
  response(self, "");
  return;
error:
  warning(self, "play syntax not recognized.");
}

static void placeFreeHandicap(GTP *self, char *argument) {
/*
      if($args =~ /\s*(\d+)\s* /i) {
        $handicap = $1;
        @handicaps = qw(k10 d16 q4 d4 q16 d10 q10 k16 k4);
        for(@handicaps[0..($handicap-1)]) {
          my ($col,$row) = processMove($move);
          $table->[$col][$row] = 1;
        }
        response($id,join(" ",@handicaps[0..($handicap-1)]));
      } else { warning($id,"boardsize not recognized."); }
*/
}

static void setFreeHandicap(GTP *self, char *argument) {
/*      while($args =~ s/(\w\d+)//i) {
        $move = $1;
        ($col,$row) = processMove($move);
        $table->[$col][$row] = -1;
      }
*/
  response(self, "");
}

static void generateMove(GTP *self, char *argument) {
  // todo
  response(self,"pass");
}
