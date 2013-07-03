#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "boardcoord.h"

int main (int argc, char *argv[]) {
  BoardCoord coord = createBoardCoord(5, 6);
  assert(coord.col == 6);
  assert(coord.row == 5);
}



