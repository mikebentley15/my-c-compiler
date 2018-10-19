#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "charstream.h"

struct Scanner {
  struct CharStream* in;
  bool is_init;
  bool is_eof;
  char next;
  char[500] tok_buf;
};

void Scanner_init(struct Scanner* scanner, struct CharStream* stream);
void Scanner_close(struct Scanner* scanner); // doesn't close scanner->in
bool Scanner_is_eof(struct Scanner* scanner);
struct Token Scanner_next(struct Scanner* scanner);

#endif // SCANNER_H
