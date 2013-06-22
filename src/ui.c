
#include  "ui.h"

BoardCoord ui_move(Game *game)
{
  BoardCoord coord;
  char c;
  int  n;
  printf("Player %d step: ", game->actPlayer);
  do {
    scanf ("%c%d", &c, &n);
    coord = boardCoord_create(n - 1, c - 'a');
  } while (!game->validMove(game, coord));
  return(coord);
}
