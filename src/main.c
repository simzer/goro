/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "goro.h"

int main(int argc, char *argv[])
{
  Goro goro = createGoro(argc, argv);
  runGoro(&goro);
  destructGoro(&goro);
  return 0;
}
