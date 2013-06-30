/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#include <stdlib.h>
#include <assert.h>

#include "board.h"
#include "boarditerator.h"
#include "group.h"

static Group collectNextGroup(Board *board);
static void collectGroup(Group *self, BoardCoord coord);

Group createGroup(Board *board)
{
  Group self;
  self.board = board;
  self.cells = 0;
  self.size = 0;
  return self;
}

void destructGroup(Group *self)
{
  free(self->cells);
  self->cells = 0;
  self->size = 0;
}

void addCoordToGroup(Group *self, BoardCoord coord)
{
  self->size++;
  self->cells = realloc(self->cells, self->size * sizeof(BoardCoord));
  self->cells[self->size-1] = coord;
}

int groupLiberties(Group *self)
{
  int liberties = 0;
  BoardCell cell = getBoardCell(self->board, self->cells[0]);
  Board board = copyBoard(self->board);
  int i;
  for (i = 0; i < self->size; i++) {
    NeighbourIterator iterator =
        createNeighbourIterator(self->cells[i],fourNeighbourhood);
    while(getNeighbours(&iterator)) {
      if (getBoardCell(&board, iterator.neighbour) == emptyBoardCell) {
        liberties++;
        setBoardCell(&board, iterator.neighbour, cell);
      }
    }
  }
  destructBoard(&board);
  return liberties;
}

void removeGroup(Group *self)
{
  int i;
  for (i = 0; i < self->size; i++) {
    clearBoardCell(self->board, self->cells[i]);
  }
  self->size = 0;
  free(self->cells);
  self->cells = 0;
}

void printGroup(Group *self)
{
  int i;
  printf("[");
  for (i = 0; i < self->size; i++) {
    BoardCoordString string = boardCoordToString(self->cells[i]);
    printf("%s,", string.chars);
  }
  printf("]\n");
}

BoardCell territoryOwner(Group *self)
{
  BoardCell owner = emptyBoardCell;
  BoardCell firstNeighbour = emptyBoardCell;
  int i;
  for (i = 0; i < self->size; i++) {
    NeighbourIterator iterator =
        createNeighbourIterator(self->cells[i],fourNeighbourhood);
    while(getNeighbours(&iterator)) {
      BoardCell neighbour = getBoardCell(self->board, iterator.neighbour);
      if (boardCellFilled(neighbour)) {
        if(firstNeighbour == emptyBoardCell) {
          firstNeighbour = neighbour;
          owner = neighbour;
        }
        if(firstNeighbour != neighbour) owner = emptyBoardCell;
      }
    }
  }
  return owner;
}

GroupList createGroupList(Board *board)
{
  GroupList self;
  Group group;
  Board tempBoard = copyBoard(board);
  self.groupNumber = 0;
  self.groups = malloc(0);
  while(group = collectNextGroup(&tempBoard),
        group.size != 0)
  {
    group.board = board;
    self.groupNumber++;
    self.groups = realloc(self.groups, self.groupNumber * sizeof(Group));
    self.groups[self.groupNumber-1] = group;
  }
  return self;
}

static Group collectNextGroup(Board *board)
{
  Group group = createGroup(board);
  BoardIterator iterator = createBoardIterator(board);
  while(!boardIteratorFinished(&iterator)) {
    if(getBoardCell(board, iterator.coord) != invalidBoardCell) {
      collectGroup(&group, iterator.coord);
      break;
    }
  }
  return group;
}

static void collectGroup(Group *self, BoardCoord coord)
{
  NeighbourIterator iterator;
  BoardCell referenceCell = getBoardCell(self->board, coord);
  addCoordToGroup(self, coord);
  setBoardCell(self->board, coord, invalidBoardCell);
  iterator = createNeighbourIterator(coord,fourNeighbourhood);
  while(getNeighbours(&iterator)) {
    if(   getBoardCell(self->board, iterator.neighbour)
       == referenceCell )
    {
      collectGroup(self, iterator.neighbour);
    }
  }
}

void destructGroupList(GroupList *self)
{
  int i;
  for (i = 0; i < self->groupNumber; i++) {
    destructGroup(&self->groups[i]);
  }
  free(self->groups);
  self->groups = 0;
}

void printGroupList(GroupList *self)
{
  int i;
  printf("(\n");
  for (i = 0; i < self->groupNumber; i++) {
    printf("group[%d] = ", i);
    printGroup(&self->groups[i]);
    printf(",");
  };
  printf(")\n");
}

GroupIterator createGroupIterator(GroupList *groups)
{
  GroupIterator self;
  self.groups = groups;
  self.index = -1;
  self.group = 0;
  return self;
}

int getGroups(GroupIterator *self)
{
  self->index++;
  if(self->index < self->groups->groupNumber) {
    self->group = &(self->groups->groups[self->index]);
    return 1;
  } else {
    self->index = -1;
    self->group = 0;
    return 0;
  }
}

int getGroupsByColor(GroupIterator *self, BoardCell cell)
{
  int running;
  do {
    running = getGroups(self);
  } while(running
          && getBoardCell(self->group->board, self->group->cells[0])
             != cell);
  return running;
}

int getTerritories(GroupIterator *self)
{
  return getGroupsByColor(self, emptyBoardCell);
}


