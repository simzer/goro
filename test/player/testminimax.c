/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "minimax.h"

static const struct {
  BoardCoord move;
  BoardCoord childmoves[2];
  int score;
} moves[] = {
  { {-1,-1 }, {{ 1, 1 }, { 1, 2 }}, 0},
  { { 1, 1 }, {{ 2, 1 }, { 2, 2 }}, 0},
  { { 1, 2 }, {{ 2, 3 }, { 2, 4 }}, 0},
  { { 2, 1 }, {{ 3, 1 }, { 3, 2 }}, 0},
  { { 2, 2 }, {{ 3, 3 }, { 0, 0 }}, 0},
  { { 2, 3 }, {{ 3, 4 }, { 3, 5 }}, 0},
  { { 2, 4 }, {{ 3, 6 }, { 0, 0 }}, 0},
  { { 3, 1 }, {{ 4, 1 }, { 4, 2 }}, 0},
  { { 3, 2 }, {{ 4, 3 }, { 0, 0 }}, 0},
  { { 3, 3 }, {{ 4, 4 }, { 0, 0 }}, 0},
  { { 3, 4 }, {{ 4, 5 }, { 4, 6 }}, 0},
  { { 3, 5 }, {{ 4, 7 }, { 0, 0 }}, 0},
  { { 3, 6 }, {{ 4, 8 }, { 4, 9 }}, 0},
  { { 4, 1 }, {{ 0, 0 }, { 0, 0 }}, 10 },
  { { 4, 2 }, {{ 0, 0 }, { 0, 0 }}, 100 },
  { { 4, 3 }, {{ 0, 0 }, { 0, 0 }}, 5 },
  { { 4, 4 }, {{ 0, 0 }, { 0, 0 }}, -10 },
  { { 4, 5 }, {{ 0, 0 }, { 0, 0 }}, 7 },
  { { 4, 6 }, {{ 0, 0 }, { 0, 0 }}, 5 },
  { { 4, 7 }, {{ 0, 0 }, { 0, 0 }}, -100 },
  { { 4, 8 }, {{ 0, 0 }, { 0, 0 }}, -7 },
  { { 4, 9 }, {{ 0, 0 }, { 0, 0 }}, -5 }
};

static int actualIndex(Game *self) {
  int i;
  for (i = 0; i < sizeof(moves)/sizeof(moves[0]); i++) {
    if(boardCoordsEqual(moves[i].move, self->lastMove.coord)) return i;
  }
  return -1;
}

static double testEvalPosition(Game *self) {
  printf("%d, ",moves[actualIndex(self)].score);
  return moves[actualIndex(self)].score;
};

static PlayerId testWinner(Game *self) {
  return noPlayer;
}

static int testMoveWorthChecking(Game *self, GameMove move) {
  return 1;
}

static int testValidMove(Game *self, GameMove move) {
  int res =  (boardCoordsEqual(move.coord, moves[actualIndex(self)].childmoves[0])
          && !boardCoordsEqual(createBoardCoord(0,0), moves[actualIndex(self)].childmoves[0]))
       ||(boardCoordsEqual(move.coord, moves[actualIndex(self)].childmoves[1])
           && !boardCoordsEqual(createBoardCoord(0,0), moves[actualIndex(self)].childmoves[1]));
  return res;
}

static double testScore(Game *self) {
  return 0;
}
int testGameOver(Game *self) {
  return boardCoordsEqual(createBoardCoord(0,0), moves[actualIndex(self)].childmoves[0]);
}

static GameVirtualTable testGameVirtualTable = {
  genericGameMove,
  testValidMove,
  testMoveWorthChecking,
  testGameOver,
  testWinner,
  testEvalPosition,
  copyGame,
  testScore,
  printGameStatus
};

void testminimax(void)
{
  Board board = createBoard(10,10);
  Game game = createGame(board);
  MiniMax player = createMiniMax(&game);
  GameMove move;
  game.lastMove = nullMove;
  game.vtable = &testGameVirtualTable;
  startGame(&game);
  move = getMiniMaxMove(&player);
  printGameMove(move);
  assert(gameMoveEqual(move,createPlayMove(createBoardCoord(1,2))));
}
