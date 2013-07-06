/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <string.h>
#include <assert.h>

#include "boardcoord.h"

static void boardCoardStringEqual(char* input, char* reference) {
  BoardCoordString coordString = createBoardCoordString(input);
  assert(strcmp(coordString.chars, reference) == 0);
}

static void boardCoordsOperationResults(BoardCoord coord, int row, int col) {
  assert(coord.col == col);
  assert(coord.row == row);
}

static void boardCoordsConvertedToString(int row, int col, char *string) {
  BoardCoordString coordString =
      boardCoordToString(createBoardCoord(row, col));
  assert(strcmp(coordString.chars, string) == 0);
}

static void stringConvertedToBoardCoord(char *string, int row, int col)
{
  BoardCoord coord1 = createBoardCoord(row, col);
  BoardCoord coord2 = stringToBoardCoord(createBoardCoordString(string));
  assert(boardCoordsEqual(coord1, coord2));
}

static void stringShouldConvertedToNullCoord(char *string)
{
  BoardCoord coord = stringToBoardCoord(createBoardCoordString(string));
  assert(boardCoordsEqual(coord, nullBoardCoord));
}

static void validStringCopiedToBoardCoordString(void)
{
  boardCoardStringEqual("f5","f5");
}

static void longStringChompedInBoardCoordString(void)
{
  boardCoardStringEqual("abcd","abc");
}

static void bothCasedCharsConvertedToColumnIndex(void)
{
  assert(charToBoardColumn('a') == 0);
  assert(charToBoardColumn('t') == 18);
  assert(charToBoardColumn('A') == 0);
  assert(charToBoardColumn('T') == 18);
}

static void invalidCharConvertedToInvalidColumnIndex(void)
{
  assert(charToBoardColumn('?') == -1);
}

static void validColumnIndicesConvertedToUpperCaseChars(void)
{
  assert(boardColumnToChar(0) == 'A');
  assert(boardColumnToChar(18) == 'T');
}

static void invalidColumnIndicesConvertedToQuestionMark(void)
{
  assert(boardColumnToChar(19) == '?');
  assert(boardColumnToChar(-1) == '?');
}

static void boardCoordGetsCoordinates(void)
{
  boardCoordsOperationResults(createBoardCoord(5, 6), 5, 6);
}

static void boardCoordAdditionWorksInRange(void)
{
  boardCoordsOperationResults(addBoardCoords(createBoardCoord(5, 6),
                                            createBoardCoord(7, 9)),
                             5 + 7, 6 + 9);
}

static void boardCoordSubtractionWorksInRange(void)
{
  boardCoordsOperationResults(subBoardCoords(createBoardCoord(7, 9),
                                            createBoardCoord(5, 6)),
                             7 - 5, 9 - 6);
}

static void boardCoordMultiplicationWorkInRange(void)
{
  boardCoordsOperationResults(mulBoardCoord(3, createBoardCoord(5, 6)),
                             3 * 5, 3 * 6);
}

static void sameBoardCoordsAreEqual(void)
{
  BoardCoord coord1 = createBoardCoord(5, 6);
  assert(boardCoordsEqual(coord1, coord1));
}

static void differentBoardCoordAreNotEqual(void)
{
  BoardCoord coord1 = createBoardCoord(5, 6);
  BoardCoord coord2 = createBoardCoord(5, 9);
  BoardCoord coord3 = createBoardCoord(7, 6);
  assert(!boardCoordsEqual(coord1, coord2));
  assert(!boardCoordsEqual(coord1, coord3));
}

static void validStringsConvertedToBoardCoord(void)
{
  stringConvertedToBoardCoord("g6", 5, 6);
  stringConvertedToBoardCoord("G6", 5, 6);
  stringConvertedToBoardCoord("K16", 15, 9);
}

static void invalidStringsConvertedToNullCoord(void)
{
  stringShouldConvertedToNullCoord("45");
  stringShouldConvertedToNullCoord("ff");
  stringShouldConvertedToNullCoord("X16");
}

static void validCoordinatesConvertedToCoordString(void)
{
  boardCoordsConvertedToString(5, 6, "G6");
  boardCoordsConvertedToString(15, 9, "K16");
}

static void invalidCoordinatesConvertedToNanString(void)
{
  boardCoordsConvertedToString(-1, 9, "nan");
  boardCoordsConvertedToString(5, -1, "nan");
}

static void halfNeighbourhoodGivesHalfEndIndex(void)
{
  assert(halfNeighbourhood(&fourNeighbourhood).startIndex == 0);
  assert(halfNeighbourhood(&fourNeighbourhood).endIndex == 1);
  assert(halfNeighbourhood(&eightNeighbourhood).startIndex == 0);
  assert(halfNeighbourhood(&eightNeighbourhood).endIndex == 3);
}

static void testBoardCoordString(void) {
  validStringCopiedToBoardCoordString();
  longStringChompedInBoardCoordString();
  validStringsConvertedToBoardCoord();
  invalidStringsConvertedToNullCoord();
  validCoordinatesConvertedToCoordString();
  invalidCoordinatesConvertedToNanString();
}

static void testBoardColumn(void) {
  bothCasedCharsConvertedToColumnIndex();
  invalidCharConvertedToInvalidColumnIndex();
  validColumnIndicesConvertedToUpperCaseChars();
  invalidColumnIndicesConvertedToQuestionMark();
}

static void testBoardCoord(void) {
  boardCoordGetsCoordinates();
  boardCoordAdditionWorksInRange();
  boardCoordSubtractionWorksInRange();
  boardCoordMultiplicationWorkInRange();
  sameBoardCoordsAreEqual();
  differentBoardCoordAreNotEqual();
}

static void testNeighbourhood(void) {
  halfNeighbourhoodGivesHalfEndIndex();
}

void testboardcoord(void)
{
  testBoardColumn();
  testBoardCoordString();
  testBoardCoord();
  testNeighbourhood();
}
