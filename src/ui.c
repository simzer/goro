
#include  "ui.h"

void ui_move(int *i, int *j, Game *game, Board *board)
{
  char c;
  int  n;
  printf("Player %d step: ", game->actPlayer);
  do {
    scanf ("%c%d", &c, &n);
    *i = c - 'a';
    *j = n - 1;
  } while (!board_validMove(*i, *j, board));
}
