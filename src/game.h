
#ifndef __GAME_H__
#define __GAME_H__

typedef enum { PLAYER_NONE, PLAYER_X, PLAYER_O } player;

typedef struct game {
  player actPlayer;
} game;

extern void game_init(game *self);
extern void game_switchPlayer(game *self);

#endif
