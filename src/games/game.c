/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <string.h>

#include "game.h"

static void switchGamePlayer(Game *self);

const BoardCell gamePlayerCells[numberOfPlayers + 1] = {
  blackBoardCell,
  whiteBoardCell,
  invalidBoardCell
};

const char *gamePlayerNames[numberOfPlayers] = {
  "First player",
  "Second Player"
};

BoardCell actualGamePlayerCell(Game *self)
{
  return gamePlayerCells[self->actualPlayer];
}

Game createGame(Board board)
{
  Game self;
  self.status = notStartedStatus;
  self.actualPlayer = noPlayer;
  self.lastPlayer = noPlayer;
  self.winner = noPlayer;
  self.board = board;
  self.lastMove = nullMove;
  self.movesDone = 0;
  return self;
}

Game *copyGame(Game *self)
{
  Game *copy = (Game *)malloc(sizeof(Game));
  *copy = *self;
  copy->board = copyBoard(&(self->board));
  return copy;
}

void destructGame(Game *self)
{
  destructBoard(&(self->board));
}

void startGame(Game *self)
{
  self->status = activeStatus;
  self->actualPlayer = firstPlayer;
}

static void finishGame(Game *self) {
  self->winner = self->vtable->winner(self);
  self->actualPlayer = noPlayer;
  self->status = finishedStatus;
}

static void updateGameStatus(Game *self) {
  if(self->vtable->over(self)) finishGame(self);
}

int genericGameMove(Game *self, GameMove move)
{
  if (!self->vtable->validMove(self, move)) return 0;
  self->lastMove = move;
  if(move.type == playMove) {
    setBoardCell(&self->board, move.coord, actualGamePlayerCell(self));
  } else if(move.type == resignMove) {
    self->winner = otherGamePlayer(self);
  }
  self->lastPlayer = self->actualPlayer;
  switchGamePlayer(self);
  updateGameStatus(self);
  self->movesDone++;
  return 1;
}

static void switchGamePlayer(Game *self)
{
  self->actualPlayer = otherGamePlayer(self);
}

PlayerId actualGamePlayer(Game *self)
{
  return self->actualPlayer;
}

PlayerId otherGamePlayer(Game *self)
{
  return (self->status != activeStatus)      ? noPlayer :
         (self->actualPlayer == firstPlayer) ? secondPlayer :
                                               firstPlayer;
}

int genericGameOver(Game *self)
{
  return self->winner != noPlayer
         || !boardHasEmptyCell(&self->board);
}

int validGameMove(Game *self, GameMove move)
{
  return    self->status != finishedStatus
         && (   move.type == resignMove
             || move.type == passMove
             || getBoardCell(&(self->board), move.coord) == emptyBoardCell);
}

int nextValidGameMove(Game *self, MoveIterator *iterator)
{
  int result;
  do {
    result = !moveIteratorFinished(iterator);
  } while(   (result == 1)
          && !self->vtable->validMove(self, iterator->move));
  return result;
}

int nextMoveWorthChecking(Game *self, MoveIterator *iterator)
{
  int result;
  do {
    result = nextValidGameMove(self,iterator);
  } while(   (result == 1)
          && (self->vtable->moveWorthChecking != 0)
          && !self->vtable->moveWorthChecking(self, iterator->move));
  return result;
}

void printGameStatus(Game *self)
{
  printBoard(&self->board);
  if(self->lastMove.type != invalidMove) {
    GameMoveString move = gameMoveToString(self->lastMove);
    printf("\nLast move: %s\n\n", move.chars);
  }
}

void printGameOverInfo(Game *self)
{
  PlayerId winner = self->vtable->winner(self);
  printBoard(&self->board);
  printf(winner == noPlayer
         ? "Nobody won!\n"
         : "%s (%s) won!\n",
           gamePlayerNames[winner],
           boardCellNames[gamePlayerCells[winner]]);
  if (self->vtable->score) {
    printf("Score: %lf\n", self->vtable->score(self));
  }
}
