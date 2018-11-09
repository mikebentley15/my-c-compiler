#ifndef CHARSTREAMSTACK_H
#define CHARSTREAMSTACK_H

#include "linkedlist.h"
#include "vector.h"

// A stack of CharStream that itself makes a CharStream
struct CharStreamStack {
  struct Vector streams;  // list of CharStream*
  struct LinkedList finished; // put here after they are finished and closed
  struct CharStream cs;   // this as a character stream
};

void CSS_init(struct CharStreamStack *css);
void CSS_del(struct CharStreamStack *css);
void CSS_close(struct CharStreamStack *css);
void CSS_push(struct CharStreamStack *css, struct CharStream *cs);

bool CSS_is_eof(struct CharStreamStack *css);

struct CharStream CSS_to_CharStream(struct CharStreamStack *css);

#endif // CHARSTREAMSTACK_H
