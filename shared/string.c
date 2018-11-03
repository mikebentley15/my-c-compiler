#include "string.h"

#include <stddef.h> // for NULL
#include <stdlib.h> // for malloc(), free() and realloc()

void Str_init(struct String* s) {
  s->capacity = 32;
  s->size = 0;
  s->data = (char*) malloc(sizeof(char) * s->capacity);
  s->data[0] = '\0';
}

void Str_del(struct String* s) {
  free(s->data);
  s->data = NULL;
  s->capacity = 0;
  s->size = 0;
}

void Str_clear(struct String* s) {
  // lazy clear, doesn't actually remove anything
  s->size = 0;
  s->data[0] = '\0';
}

void Str_private_expand(struct String* s, int capacity) {
  s->capacity = capacity;
  s->data = (char*) realloc(s->data, sizeof(char) * capacity);
}

void Str_append(struct String* s, char val) {
  if (s->size == s->capacity - 1) {
    Str_private_expand(s, s->capacity * 2);
  }
  s->data[s->size] = val;
  s->size++;
  s->data[s->size] = '\0';
}

void Str_extend(struct String* s, const char *str) {
  int i = 0;
  while (str[i] != '\0') {
    Str_append(s, str[i]);
    i++;
  }
}

char Str_pop(struct String* s) {
  s->size--;
  char value = s->data[s->size];
  s->data[s->size] = '\0';
  return value;
}

char Str_at(struct String* s, int idx) {
  // TODO: bounds checking (what to do for an error?)
  return s->data[idx];
}

void Str_set(struct String* s, int idx, char val) {
  // TODO: bounds checking (what to do for an error?)
  s->data[idx] = val;
}

