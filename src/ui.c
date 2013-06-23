
#include  "ui.h"

BoardCoord ui_move(Game *game)
{
  BoardCoord coord;
  char c;
  int  n;
  printf("Player %d step: ", game->actualPlayer);
  do {
    scanf ("%c%d", &c, &n);
    coord = boardCoord_create(n - 1, c - 'a');
  } while (!game->vtable->validMove(game, coord));
  return(coord);
}
