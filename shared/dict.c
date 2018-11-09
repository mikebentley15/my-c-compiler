#include "dict.h"
#include "stringfunc.h"
#include "util.h"
#include "hash.h"

#include <stddef.h> // for NULL
#include <stdlib.h> // for malloc() and free()

#define DICT_MULT_FACTOR 4

int Dict_private_next_idx(int capacity, int idx) {
  // simply the next index, but in a circular buffer
  return (idx + 1) % capacity;
}

int Dict_private_find(struct Dict* d, const char* key) {
  int idx = djb2_hash(key) % d->capacity;
  int current_idx = Dict_private_next_idx(d->capacity, idx);
  if (d->keys[idx] == NULL) {
    // found idx
  } else if (strequal(d->keys[idx], key)) {
    // found idx
  } else {
    while (current_idx != idx) {
      d->collisions++;
      if (d->keys[current_idx] == NULL) {
        // found idx
        break;
      } else if (strequal(d->keys[current_idx], key)) {
        // found idx
        break;
      }
      current_idx = Dict_private_next_idx(d->capacity, current_idx);
    }
  }
  return idx;
}

// we would still delete the key when removed
// this is called if we already own key
void Dict_private_set_nocopy(struct Dict* d, const char* key, void* val) {
  int idx = Dict_private_find(d, key);
  if (d->key[idx] == NULL) {
    d->keys[idx] = key;
    d->size++;
  }
  d->vals[idx] = val;
}

void Dict_init(struct Dict* d, int capacity) {
  d->size = 0;
  d->collisions = 0;
  d->capacity = next_prime(DICT_MULT_FACTOR * capacity);
  d->keys = (const char**) malloc(d->capacity * sizeof(const char*));
  d->vals = (void**) malloc(d->capacity * sizeof(void*));
  Dict_clear(d);
}

void Dict_resize(struct Dict* d, int capacity) {
  struct Dict other;
  Dict_init(&other, capacity);
  int i = 0;
  while (i < d->capacity) {
    if (d->keys[i] != NULL) {
      // move ownership of keys over
      Dict_private_set_nocopy(&other, d->keys[i], d->vals[i]);
      d->keys[i] = NULL;
      d->vals[i] = NULL;
    }
    i++;
  }
  other.collisions = other.collisions + d->collisions;

  // free the old memory
  Dict_del(d);

  // copy the fields over
  d->keys       = other.keys;
  d->vals       = other.vals;
  d->capacity   = other.capacity;
  d->size       = other.size;
  d->collisions = other.collisions;
}

void Dict_del(struct Dict* d) {
  Dict_clear(d);
  free(d->keys);
  free(d->vals);
  d->keys = NULL;
  d->vals = NULL;
  d->capacity = 0;
  d->size = 0;
  d->collisions = 0;
}

void Dict_clear(struct Dict* d) {
  int i = 0;
  while (i < d->capacity) {
    if (d->keys[i] != NULL) {
      free(d->keys[i]);
      d->keys[i] = NULL;
      d->vals[i] = NULL;
    }
    i++;
  }
  d->size = 0;
}

void Dict_set(struct Dict* d, const char* key, void* val) {
  int idx = Dict_private_find(d, key);
  if (d->keys[idx] == NULL) {
    d->keys[idx] = strdup(key);
    d->size++;
  }
  d->vals[idx] = val;
}

bool Dict_contains(struct Dict* d, const char* key) {
  int idx = Dict_private_find(d, key);
  return d->keys[idx] != NULL;
}

void* Dict_at(struct Dict* d, const char* key) {
  int idx = Dict_private_find(d, key);
  return d->vals[idx];
}
