#include "hash.h"
#include "set.h"
#include "stringfunc.h"
#include "util.h"

#include <stdlib.h> // for malloc() and free()

#define SSET_MULT_FACTOR 4

void SSet_init(struct StringSet* set, int capacity) {
  set->size = 0;
  set->collisions = 0;
  set->capacity = next_prime(SSET_MULT_FACTOR * capacity);
  set->data = (const char**) malloc(set->capacity * sizeof(const char*));
  // TODO: check for set->data == NULL
  int i = 0;
  while (i < set->capacity) {
    set->data[i] = NULL;
    i++;
  }
}

void SSet_del(struct StringSet* set) {
  set->size = 0;
  set->capacity = 0;
  set->collisions = 0;
  free(set->data);
  set->data = NULL;
}

void SSet_clear(struct StringSet* set) {
  set->size = 0;
  int i = 0;
  while (i < set->capacity) {
    set->data[i] = NULL;
    i++;
  }
}

int SSet_private_next_idx(int capacity, int idx) {
  // simply the next index, but in a circular buffer
  return (idx + 1) % capacity;
}

bool SSet_add(struct StringSet* set, const char* elem) {
  int idx = djb2_hash(elem) % set->capacity;
  bool is_contained = false;
  int current_idx = SSet_private_next_idx(set->capacity, idx);
  if (set->data[idx] == NULL) {
    is_contained = false;
    set->data[idx] = elem;
  } else if (strequal(set->data[idx], elem)) {
    is_contained = true;
  } else {
    while (current_idx != idx) {
      set->collisions++;
      if (set->data[current_idx] == NULL) {
        set->data[current_idx] = elem;
        is_contained = false;
        break;
      } else if (strequal(set->data[current_idx], elem)) {
        is_contained = true;
        break;
      }
      current_idx = SSet_private_next_idx(set->capacity, current_idx);
    }
  }
  if (!is_contained) {
    set->size++;
  }
  return !is_contained;
}

bool SSet_contains(struct StringSet* set, const char* elem) {
  int idx = djb2_hash(elem) % set->capacity;
  bool is_contained = false;
  int current_idx = SSet_private_next_idx(set->capacity, idx);
  if (set->data[idx] == NULL) {
    is_contained = false;
  } else if (strequal(set->data[idx], elem)) {
    is_contained = true;
  } else {
    while (current_idx != idx) {
      set->collisions++;
      if (set->data[current_idx] == NULL) {
        is_contained = false;
        break;
      } else if (strequal(set->data[current_idx], elem)) {
        is_contained = true;
        break;
      }
      current_idx = SSet_private_next_idx(set->capacity, current_idx);
    }
  }
  return is_contained;
}
