/* This file is part of Goro. Goro is licensed under the terms of the
   GNU General Public License version 3. See <http://www.gnu.org/licenses/>.

   Copyright (C) 2013 Goro Team <https://github.com/goro-dev?tab=members> */

#ifndef __GROUP_H__
#define __GROUP_H__

typedef struct Group {
  Board *board;
  BoardCoord *cells;
  int size;
} Group;
extern Group createGroup(Board *board);
extern void addCoordToGroup(Group *self, BoardCoord coord);
extern void destructGroup(Group *self);
extern int groupLiberties(Group *self);
extern void removeGroup(Group *self);
extern BoardCell territoryOwner(Group *self);


typedef struct Groups {
  Group *groups;
  int groupNumber;
} Groups;
extern Groups createGroups(Board *board);
extern void destructGroups(Groups *self);


typedef struct GroupIterator {
  Groups *groups;
  int index;
  Group *group;
} GroupIterator;

extern GroupIterator createGroupIterator(Groups *groups);
extern int getGroups(GroupIterator *self);
extern int getGroupsByColor(GroupIterator *self, BoardCell cell);
extern int getTerritories(GroupIterator *self);

#endif
