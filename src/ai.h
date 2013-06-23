
#ifndef __AI_H__
#define __AI_H__

#include "game.h"
#include "board.h"

typedef struct MiniMax {
  Game *game;
  double monteCarloThreshold;
} MiniMax;

extern const double miniMax_winScore;
extern const double miniMax_loseScore;

extern MiniMax miniMax_create(Game *game);
extern BoardCoord miniMax_move(MiniMax *self);

#endif
