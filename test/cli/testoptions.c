/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "options.h"

static void optionSizeIsOneLessThanArgumentList(CliOptionParser *options) {
  assert(options->size == 6);
}

static void boundaryIndexedOptionsFound(CliOptionParser *options) {
  assert(cliOptionSet(options, "--option1"));
  assert(cliOptionSet(options, "option6"));
}

static void
queryForNotSetOptionResultsFalseWithEmptyValue(CliOptionParser *options)
{
  assert(!cliOptionSet(options, "--option7"));
  assert(strcmp(getCliOptionValue(options, "--options7"),"") == 0);
}

static void optionValueFoundForLastOption(CliOptionParser *options)
{
  assert(strcmp(getCliOptionValue(options, "--option5"),"option6") == 0);
}

CliOptionParser createExampleOptionParser(void) {
  static const char* argv[7] = {
    "bin/goro",
    "--option1",
    "--option2",
    "--option3",
    "option4",
    "--option5",
    "option6",
  };
  return createCliOptionParser(sizeof(argv)/sizeof(char*), argv);
}

void testoptions(void)
{
  CliOptionParser options = createExampleOptionParser();
  optionSizeIsOneLessThanArgumentList(&options);
  boundaryIndexedOptionsFound(&options);
  queryForNotSetOptionResultsFalseWithEmptyValue(&options);
  optionValueFoundForLastOption(&options);
}
