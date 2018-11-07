#include "hash.h"
#include "set.h"
#include "stringfunc.h"
#include "util.h"

#include <stdlib.h> // for malloc() and free()

#define SET_MULT_FACTOR 4

bool Set_p_default_equal(void* a, void* b) {
  return a == b;
}

unsigned long Set_p_default_hash(void* elem) {
  return (unsigned long) elem / sizeof(void*);
}

void Set_init(struct Set* set, int capacity) {
  set->collisions = 0;
  set->capacity = next_prime(SET_MULT_FACTOR * capacity);
  set->equal = &Set_p_default_equal;
  set->hash = &Set_p_default_hash;
  set->data = (void**) malloc(set->capacity * sizeof(void*));
  Set_clear(set);
}

void Set_del(struct Set* set) {
  set->size = 0;
  set->capacity = 0;
  set->collisions = 0;
  free(set->data);
  set->data = NULL;
}

void Set_set_compare(struct Set* set, bool (*equal)(void*, void*)) {
  set->equal = equal;
  // TODO: rehash everything
}

void Set_set_hash(struct Set* set, unsigned long (*hash)(void*)) {
  set->hash = hash;
  // TODO: rehash everything
}

void Set_clear(struct Set* set) {
  set->size = 0;
  int i = 0;
  while (i < set->capacity) {
    set->data[i] = NULL;
    i++;
  }
}

int Set_private_next_idx(int capacity, int idx) {
  // simply the next index, but in a circular buffer
  return (idx + 1) % capacity;
}

bool Set_add(struct Set* set, void* elem) {
  int idx = set->hash(elem) % set->capacity;
  bool is_contained = false;
  int current_idx = Set_private_next_idx(set->capacity, idx);
  if (set->data[idx] == NULL) {
    is_contained = false;
    set->data[idx] = elem;
  } else if (set->equal(set->data[idx], elem)) {
    is_contained = true;
  } else {
    while (current_idx != idx) {
      set->collisions++;
      if (set->data[current_idx] == NULL) {
        set->data[current_idx] = elem;
        is_contained = false;
        break;
      } else if (set->equal(set->data[current_idx], elem)) {
        is_contained = true;
        break;
      }
      current_idx = Set_private_next_idx(set->capacity, current_idx);
    }
  }
  if (!is_contained) {
    set->size++;
  }
  return !is_contained;
}

bool Set_contains(struct Set* set, void* elem) {
  int idx = set->hash(elem) % set->capacity;
  bool is_contained = false;
  int current_idx = Set_private_next_idx(set->capacity, idx);
  if (set->data[idx] == NULL) {
    is_contained = false;
  } else if (set->equal(set->data[idx], elem)) {
    is_contained = true;
  } else {
    while (current_idx != idx) {
      set->collisions++;
      if (set->data[current_idx] == NULL) {
        is_contained = false;
        break;
      } else if (set->equal(set->data[current_idx], elem)) {
        is_contained = true;
        break;
      }
      current_idx = Set_private_next_idx(set->capacity, current_idx);
    }
  }
  return is_contained;
}

bool SSet_p_strequal(void* a, void* b) {
  const char* str_a = (const char*) a;
  const char* str_b = (const char*) b;
  return strequal(str_a, str_b);
}

unsigned long SSet_p_hash(void* a) {
  const char* str = (const char*) a;
  return djb2_hash(str);
}

void SSet_init(struct StringSet* set, int capacity) {
  Set_init(&(set->s), capacity);
  Set_set_compare(&(set->s), &SSet_p_strequal);
  Set_set_hash(&(set->s), &SSet_p_hash);
}

void SSet_del(struct StringSet* set) {
  Set_del(&(set->s));
}

void SSet_clear(struct StringSet* set) {
  Set_clear(&(set->s));
}

bool SSet_add(struct StringSet* set, const char* elem) {
  return Set_add(&(set->s), (void*)elem);
}

bool SSet_contains(struct StringSet* set, const char* elem) {
  return Set_contains(&(set->s), (void*)elem);
}
