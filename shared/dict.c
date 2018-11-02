#include "dict.h"
#include "stringfunc.h"
#include "util.h"
#include "hash.h"

#include <stddef.h> // for NULL
#include <stdlib.h> // for malloc() and free()

#define DICT_MULT_FACTOR 4

void Dict_init(struct Dict* d, int capacity) {
  d->size = 0;
  d->collisions = 0;
  d->capacity = next_prime(DICT_MULT_FACTOR * capacity);
  d->keys = (const char**) malloc(d->capacity * sizeof(const char*));
  d->vals = (const char**) malloc(d->capacity * sizeof(const char*));
  Dict_clear(d);
}

void Dict_resize(struct Dict* d, int capacity) {
  struct Dict other;
  Dict_init(&other, capacity);
  int i = 0;
  while (i < d->capacity) {
    if (d->keys[i] != NULL) {
      Dict_set(&other, d->keys[i], d->vals[i]);
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
    d->keys[i] = NULL;
    d->vals[i] = NULL;
    i++;
  }
  d->size = 0;
}

int Dict_private_next_idx(int capacity, int idx) {
  // simply the next index, but in a circular buffer
  return (idx + 1) % capacity;
}

void Dict_set(struct Dict* d, const char* key, const char* val) {
  int idx = djb2_hash(key) % d->capacity;
  int current_idx = Dict_private_next_idx(d->capacity, idx);
  if (d->keys[idx] == NULL) {
    d->keys[idx] = key;
    d->vals[idx] = val;
    d->size++;
  } else if (strequal(d->keys[idx], key)) {
    d->vals[idx] = val;
  } else {
    while (current_idx != idx) {
      d->collisions++;
      if (d->keys[current_idx] == NULL) {
        d->keys[current_idx] = key;
        d->vals[current_idx] = val;
        d->size++;
        break;
      } else if (strequal(d->keys[current_idx], key)) {
        d->vals[current_idx] = val;
        break;
      }
      current_idx = Dict_private_next_idx(d->capacity, current_idx);
    }
  }
}

bool Dict_contains(struct Dict* d, const char* key) {
  return Dict_at(d, key) != NULL;
}

const char* Dict_at(struct Dict* d, const char* key) {
  int idx = djb2_hash(key) % d->capacity;
  const char* value;
  int current_idx = Dict_private_next_idx(d->capacity, idx);
  if (d->keys[idx] == NULL) {
    value = NULL;
  } else if (strequal(d->keys[idx], key)) {
    value = d->vals[idx];
  } else {
    while (current_idx != idx) {
      d->collisions++;
      if (d->keys[current_idx] == NULL) {
        value = NULL;
        break;
      } else if (strequal(d->keys[current_idx], key)) {
        value = d->vals[current_idx];
        break;
      }
      current_idx = Dict_private_next_idx(d->capacity, current_idx);
    }
  }
  return value;
}
