
#include  "ui.h"

void ui_move(BoardCoord *coord, Game *game, Board *board)
{
  char c;
  int  n;
  printf("Player %d step: ", game->actPlayer);
  do {
    scanf ("%c%d", &c, &n);
    coord->col = c - 'a';
    coord->row = n - 1;
  } while (!board_validMove(board, *coord));
}
