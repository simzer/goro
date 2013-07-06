/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <assert.h>

#include "testboard.h"
#include "game.h"

static double testEvalPosition(Game *self);
static PlayerId testWinner(Game *self);
static int testMoveWorthChecking(Game *self, GameMove move);
static double testScore(Game *self);

GameVirtualTable testGameVirtualTable = {
  genericGameMove,
  validGameMove,
  testMoveWorthChecking,
  genericGameOver,
  testWinner,
  testEvalPosition,
  copyGame,
  testScore,
  printGameStatus
};
typedef Game TestGame;
static TestGame createTestGame(Board board) {
  TestGame game = createGame(board);
  game.vtable = &testGameVirtualTable;
  return game;
}

static double testScore(Game *self) {
  return 42;
}

static int testMoveWorthChecking(Game *self, GameMove move) {
  return (move.coord.col + move.coord.row) % 2;
}

static PlayerId testWinner(Game *self) {
  return (!boardHasEmptyCell(&self->board)) ? firstPlayer : noPlayer;
}

static double testEvalPosition(Game *self) {
  return 0;
}

static void createdGameHasGivenBoard(void) {
  Board board = createBoard(13,19);
  setBoardCell(&board, createBoardCoord(4,5), blackBoardCell);
  Game game = createGame(board);
  assert(getBoardCell(&game.board, createBoardCoord(4,5)) == blackBoardCell);
}

static Game exampleNotStartedGame(void) {
  Board *board = malloc(sizeof(board));
  *board = createBoard(13,19);
  return createTestGame(*board);
}

static Game exampleStartedGame(void) {
  Game game = exampleNotStartedGame();
  startGame(&game);
  return game;
}

static Game exampleStartedGameWithMove(GameMove move) {
  Game game = exampleStartedGame();
  genericGameMove(&game, move);
  return game;
}

static GameMove examplePlayMove(void) {
  return createPlayMove(createBoardCoord(1,2));
}

static void createdGameIsNotStartedHasNoMoveOrPlayer(void) {
  assert(exampleNotStartedGame().status == notStartedStatus);
  assert(exampleNotStartedGame().actualPlayer == noPlayer);
}

static void copiedGamesHasSameAttributes(void) {
  Game game = exampleStartedGame();
  Game *copy = copyGame(&game);
  assert(game.actualPlayer == copy->actualPlayer);
  assert(game.status == copy->status);
  assert(&game != copy);
}

static void copiedGameHasCopiedBoard(void) {
  Game game = exampleStartedGame();
  Game *copy = copyGame(&game);
  sameBoardsOnDifferentMemoryLocation(&copy->board, &game.board);
}

static void gameDestructionResultsNullBoardCells(void) {
  Game game = exampleStartedGame();
  destructGame(&game);
  assert(game.board.cells == 0);
}

static void gameStartSetsFirstPlayerAsActualPlayer(void) {
  assert(exampleStartedGame().actualPlayer == firstPlayer);
}

static void gameStartSetsActiveStatus(void) {
  assert(exampleStartedGame().status == activeStatus);
}

static void moveSwitchesPlayer(void) {
  assert(exampleStartedGameWithMove(createPassMove()).actualPlayer == secondPlayer);
}

static void playedMoveRegisteredOnBoard(void) {
  Game game = exampleStartedGameWithMove(examplePlayMove());
  assert(getBoardCell(&game.board, createBoardCoord(1,2)) == blackBoardCell);
}

static void specialMovesAreValid(void) {
  Game game = exampleStartedGame();
  assert(genericGameMove(&game, createPassMove()) == 1);
  assert(genericGameMove(&game, createResignMove()) == 1);
}

static void moveOnEmptyCellIsValid(void) {
  Game game = exampleStartedGame();
  assert(genericGameMove(&game, examplePlayMove()) == 1);
}

static void moveOnFilledCellIsInvalid(void) {
  Game game = exampleStartedGameWithMove(examplePlayMove());
  assert(genericGameMove(&game, examplePlayMove()) == 0);
}

static void resignMoveResultFinishedGame(void) {
  Game game = exampleStartedGameWithMove(createResignMove());
  assert(game.status == finishedStatus);
}

static void lastGameMoveIsStored(void) {
  Game game = exampleStartedGameWithMove(examplePlayMove());
  assert(gameMoveEqual(game.lastMove, examplePlayMove()));
}

static void gameMoveDoesNothingOnFinishedGame(void) {

}

static void actualAndOtherPlayerCanBeQueried(void) {
  Game game = exampleStartedGameWithMove(examplePlayMove());
  assert(actualGamePlayer(&game) == secondPlayer);
  assert(otherGamePlayer(&game) == firstPlayer);
}

static void playerQueryGivesNoPlayerOnNotActiveGame(void) {
  Game game = exampleNotStartedGame();
  assert(actualGamePlayer(&game) == noPlayer);
  assert(otherGamePlayer(&game) == noPlayer);
}

static void actualGameCellIsNullForInactiveGames(void) {
  Game game = exampleNotStartedGame();
  assert(actualGamePlayerCell(&game) == invalidBoardCell);
}

static void actualGameCellCorrespondsToActualPlayer(void) {
  Game game = exampleStartedGameWithMove(examplePlayMove());
  assert(actualGamePlayerCell(&game) == whiteBoardCell);
}

static void gameOverDetectedIfBoardIsFull(void) {
  Game game = createTestGame(full19x19Board());
  assert(genericGameOver(&game) == 1);
}

static void nextWorthMoveDoesNotGiveWorthlessMoves(void) {
  Game game = exampleNotStartedGame();
  MoveIterator iterator = createMoveIterator(&game.board);
  while(nextMoveWorthChecking(&game, &iterator)) {
    assert(testMoveWorthChecking(&game, iterator.move));
  }
}

static Game exampleGameWithEveryMovePlayedFor(
    int next(Game *self, MoveIterator *iterator))
{
  Game game = exampleStartedGame();
  MoveIterator iterator = createMoveIterator(&game.board);
  while(next(&game, &iterator)) genericGameMove(&game, iterator.move);
  return game;
}

static Game exampleGameWithEveryWorthMovePlayed() {
  return exampleGameWithEveryMovePlayedFor(nextMoveWorthChecking);
}

static Game exampleGameWithEveryValidMovePlayed() {
  return exampleGameWithEveryMovePlayedFor(nextValidGameMove);
}

static void testOnlyWorthMovePlayed(Game *game, BoardCoord coord) {
  if(testMoveWorthChecking(game,  createPlayMove(coord))) {
    assert(getBoardCell(&game->board, coord) != emptyBoardCell);
  } else {
    assert(getBoardCell(&game->board, coord) == emptyBoardCell);
  }
}

static void nextWorthMoveGivesAllMovesWorthCheckingOnce(void) {
  Game game = exampleGameWithEveryWorthMovePlayed();
  BoardIterator iterator = createBoardIterator(&game.board);
  while(!boardIteratorFinished(&iterator))
    testOnlyWorthMovePlayed(&game, iterator.coord);
}

static int movesPlayedByIterator(int next(Game *self, MoveIterator *iterator))
{
  Game game = exampleStartedGame();
  MoveIterator iterator = createMoveIterator(&game.board);
  int moves = 0;
  while(next(&game, &iterator)) moves++;
  return moves;
}

static void nextWorthMoveStopsAtLastWorthMove(void) {
  assert(movesPlayedByIterator(nextMoveWorthChecking) == (19*13)/2);
}

static void nextValidMoveGivesAllValidMovesOnce(void) {
  assert(movesPlayedByIterator(nextValidGameMove) == 19*13+2);
}

void testgame(void)
{
  createdGameHasGivenBoard();
  createdGameIsNotStartedHasNoMoveOrPlayer();
  copiedGamesHasSameAttributes();
  copiedGameHasCopiedBoard();
  gameDestructionResultsNullBoardCells();
  gameStartSetsFirstPlayerAsActualPlayer();
  gameStartSetsActiveStatus();
  moveSwitchesPlayer();
  specialMovesAreValid();
  playedMoveRegisteredOnBoard();
  moveOnEmptyCellIsValid();
  moveOnFilledCellIsInvalid();
  resignMoveResultFinishedGame();
  lastGameMoveIsStored();
  gameMoveDoesNothingOnFinishedGame();
  actualAndOtherPlayerCanBeQueried();
  playerQueryGivesNoPlayerOnNotActiveGame();
  actualGameCellIsNullForInactiveGames();
  actualGameCellCorrespondsToActualPlayer();
  nextWorthMoveDoesNotGiveWorthlessMoves();
  nextWorthMoveGivesAllMovesWorthCheckingOnce();
  nextWorthMoveStopsAtLastWorthMove();
  nextValidMoveGivesAllValidMovesOnce();
  gameOverDetectedIfBoardIsFull();
}
