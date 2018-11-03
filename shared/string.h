#ifndef STRING_H
#define STRING_H

struct String {
  char* data;
  int capacity;
  int size;
};

void Str_init(struct String* v);
void Str_del(struct String* v);
void Str_clear(struct String* v);
void Str_append(struct String* v, char val);
void Str_extend(struct String* v, const char *str);
char Str_pop(struct String* v);
char Str_at(struct String* v, int idx);
void Str_set(struct String* v, int idx, char val);

#endif // STRING_H
