#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "charstream.h"

struct Scanner {
  CharStream* in;
  bool is_init;
  bool is_eof;
  char next;
  char[500] tok_buf;
};

void Scanner_init(Scanner* scanner, CharStream* stream);
void Scanner_close(Scanner* scanner); // doesn't close scanner->in
bool Scanner_is_eof(Scanner* scanner);
Token Scanner_next(Scanner* scanner);

#endif // SCANNER_H
