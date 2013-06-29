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

extern int groupLiberties(Group *self);
extern void removeGroup(Group *self);

typedef struct GroupIterator {
  Board *board;
  Group *group;
} GroupIterator;

extern GroupIterator createGroupIterator(Board *board);
extern int groupIteratorFinished(GroupIterator *self);

#endif
