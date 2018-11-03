#include "vector.h"

#include <stddef.h> // for NULL
#include <stdlib.h> // for malloc(), free(), and realloc()

void  V_init(struct Vector* v, int capacity) {
  if (capacity < 1) {
    capacity = 1;
  }
  v->capacity = capacity;
  v->size = 0;
  v->data = (void**) malloc(sizeof(void*) * capacity);
}

void  V_del(struct Vector* v) {
  free(v->data);
  v->data = NULL;
  v->capacity = 0;
  v->size = 0;
}

void  V_clear(struct Vector* v) {
  int i = 0;
  while (i < v->size) {
    v->data[i] = NULL;
    i++;
  }
  v->size = 0;
}

void  V_private_expand(struct Vector* v, int capacity) {
  v->capacity = capacity;
  v->data = (void**) realloc(v->data, sizeof(void*) * capacity);
}

void  V_append(struct Vector* v, void* val) {
  if (v->size == v->capacity) {
    V_private_expand(v, v->capacity * 2);
  }
  v->data[v->size] = val;
  v->size++;
}

void* V_pop(struct Vector* v) {
  v->size--;
  void* value = v->data[v->size];
  v->data[v->size] = NULL;
  return value;
}

void* V_at(struct Vector* v, int idx) {
  // TODO: bounds checking (what to do for an error?)
  return v->data[idx];
}

void  V_set(struct Vector* v, int idx, void* val) {
  // TODO: bounds checking (what to do for an error?)
  v->data[idx] = val;
}

