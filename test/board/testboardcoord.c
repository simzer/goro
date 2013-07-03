/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "boardcoord.h"

static void testCreateBoardCoord(void)
{
  BoardCoord coord = createBoardCoord(5, 6);
  assert(coord.col == 6);
  assert(coord.row == 5);
}

void testboardcoord(void)
{
  testCreateBoardCoord();
}
