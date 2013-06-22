
#ifndef __AI_H__
#define __AI_H__

#include "game.h"
#include "board.h"

extern double ai_mcThreshold;

extern void ai_move(BoardSize *i, BoardSize *j, Game *game, Board *board);

#endif
