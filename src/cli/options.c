/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include "options.h"

char *invalidOptionValue = "";

CliOptionParser createCliOptionParser(int argc, char **argv)
{
  CliOptionParser self;
  self.size = argc-1;
  self.options = argv+1;
  return self;
}

int cliOptionSet(CliOptionParser *self, char *option)
{
  int index = self->size;
  while((--index) >= 0) {
    if (strcmp(self->options[index], option) == 0) return 1;
  }
  return 0;
}

char *getCliOptionValue(CliOptionParser *self, char *option)
{
  int index = self->size - 1;
  while((--index) >= 0) {
    if (strcmp(self->options[index], option) == 0) return self->options[index+1];
  }
  return invalidOptionValue;
}
