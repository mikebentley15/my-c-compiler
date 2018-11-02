#ifndef SET_H
#define SET_H

struct StringSet {
  const char **data; // the set itself
  int size;          // how many elements are stored
  int capacity;      // the size of data
  int collisions;    // stat on how many collisions were found, on both reads
                     // and writes
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
