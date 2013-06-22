
#ifndef __GAME_H__
#define __GAME_H__

typedef enum { player_none, player_1, player_2 } Player;

typedef struct {
  Player actPlayer;
} Game;

extern Game game_create();
extern void game_switchPlayer(Game *self);

#endif
