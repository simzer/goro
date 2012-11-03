
#ifndef __AI_H__
#define __AI_H__

#include "game.h"
#include "board.h"

extern double ai_mcThreshold;

extern int ai_move(int *i, int *j, game *actGame, board *actBoard);

#endif