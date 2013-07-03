#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include <options.h>

#include "boardcoord.h"

#define TEST(testname) \
{ \
  if(cliOptionSet(&options, #testname)) testname(); \
}

int main (int argc, char *argv[]) {
  CliOptionParser options = createCliOptionParser(argc, argv);
  TEST(testboard)
  TEST(testboardcoord)
  TEST(testboarditerator)
  TEST(testgoro)
  TEST(testoptions)
  TEST(testgame)
  TEST(testgamemove)
  TEST(testgo)
  TEST(testgomoko)
  TEST(testgroup)
  TEST(testtictactoe)
  TEST(testcliplayer)
  TEST(testgtp)
  TEST(testminimax)
  TEST(testtestplayer)
  return 0;
}



