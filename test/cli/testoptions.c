/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "options.h"

static char* argv[6] = {
  "--option1",
  "--option2",
  "--option3",
  "option4",
  "--option5",
  "option6",
};

CliOptionParser createCliOptionParser(int argc, char **argv);
extern int cliOptionSet(CliOptionParser *self, char *option);
extern char *getCliOptionValue(CliOptionParser *self, char *option);

void testoptions(void)
{
  CliOptionParser options = createCliOptionParser(sizeof(argv)/sizeof(char*), argv);
  assert(options.size == 6);
  assert(cliOptionSet(&options, "--option1"));
  assert(cliOptionSet(&options, "option6"));
  assert(!cliOptionSet(&options, "--option7"));
  assert(strcmp(getCliOptionValue(&options, "--option5"),"option6") == 0);
  assert(strcmp(getCliOptionValue(&options, "--options7"),"") == 0);
}
