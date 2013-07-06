/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef TESTBOARD_H_
#define TESTBOARD_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

extern int sameBoardsOnDifferentMemoryLocation(Board *board1, Board *board2);
extern Board full19x19Board(void);

#endif
