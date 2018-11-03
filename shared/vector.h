#ifndef VECTOR_H
#define VECTOR_H

struct Vector {
  void** data;
  int capacity;
  int size;
};

void  V_init(struct Vector* v, int capacity);
void  V_del(struct Vector* v);
void  V_clear(struct Vector* v);
void  V_append(struct Vector* v, void* val);
void* V_pop(struct Vector* v);
void* V_at(struct Vector* v, int idx);
void  V_set(struct Vector* v, int idx, void* val);

#endif // VECTOR_H
