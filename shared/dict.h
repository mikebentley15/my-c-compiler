#ifndef DICT_H
#define DICT_H

#include "bootstrap.h"

struct Dict {
  const char **keys;  // keys   (NULL means empty slot)
  const char **vals;  // values (NULL means empty slot)
  int capacity;       // size of keys and vals
  int size;           // number of elements
  int collisions;     // read and write collision stat
};

// For both init and resize, capacity is how many elements you want to store
// inside, which is less than the actual capacity used for this hash map.
void Dict_init(struct Dict* d, int capacity);
void Dict_resize(struct Dict* d, int capacity);
void Dict_del(struct Dict* d);
void Dict_clear(struct Dict* d);
void Dict_set(struct Dict* d, const char* key, const char* val);
bool Dict_contains(struct Dict* d, const char* key);
const char* Dict_at(struct Dict* d, const char* key);

// Not yet found to be necessary
//void Dict_remove(struct Dict* d, const char* key);

#endif // DICT_H
