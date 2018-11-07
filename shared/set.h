#ifndef SET_H
#define SET_H

#include "bootstrap.h"

struct Set {
  void **data;                  // the set itself
  int size;                     // number of elements in the set
  int capacity;                 // size of data
  int collisions;               // stat of read/write collisions
  bool (*equal)(void*, void*);  // compare function (user can set this)
  unsigned long (*hash)(void*); // hash function (user can set this)
};

// capacity is how many elements you think you will insert, not necessarily the
// size of the set.  That is determined internally.
void Set_init(struct Set* set, int capacity);
void Set_del(struct Set* set);
void Set_set_compare(struct Set* set, bool (*equal)(void*, void*));
void Set_set_hash(struct Set* set, unsigned long (*hash)(void*));
void Set_clear(struct Set* set);
// returns true if the element was not already there, i.e. it was successfully
// added.
bool Set_add(struct Set* set, void* elem);
bool Set_contains(struct Set* set, void* elem);

struct StringSet {
  struct Set s;      // underlying set
};

// capacity is how many elements you think you will insert, not necessarily the
// size of the set.  That is determined internally.
void SSet_init(struct StringSet* set, int capacity);
void SSet_del(struct StringSet* set);
void SSet_clear(struct StringSet* set);
// returns true if the element was not already there, i.e. it was successfully
// added.
bool SSet_add(struct StringSet* set, const char* elem);
bool SSet_contains(struct StringSet* set, const char* elem);

// Not yet found to be necessary
//void SSet_remove(struct StringSet* set, const char* elem);
//void SSet_resize(struct StringSet* set, int capacity);

#endif // SET_H
