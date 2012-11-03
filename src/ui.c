
#include  "ui.h"

void ui_move(int *i, int *j, game *actGame, board *actBoard)
{
  char c;
  int  n;
  printf("Player %d step: ", actGame->actPlayer);
  do {
    scanf ("%c%d", &c, &n);
    *i = c - 'a';
    *j = n - 1;
  } while (!board_validMove(*i, *j, actBoard));
}
