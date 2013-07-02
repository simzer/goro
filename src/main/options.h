/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

typedef struct CliOptionParser {
  int size;
  char **options;
} CliOptionParser;

extern CliOptionParser createCliOptionParser(int argc, char **argv);
extern int cliOptionSet(CliOptionParser *self, char *option);
extern char *getCliOptionValue(CliOptionParser *self, char *option);

#endif
