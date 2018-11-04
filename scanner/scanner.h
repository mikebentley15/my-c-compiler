#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "charstream.h"
#include "string.h"

struct Scanner {
  struct CharStream* in;
  bool is_init;
  bool is_eof;
  char next;
  struct String tok_buf;
  int buflen;
};

void Scanner_init(struct Scanner* scanner, struct CharStream* stream);
void Scanner_close(struct Scanner* scanner); // doesn't close scanner->in
bool Scanner_is_eof(struct Scanner* scanner);
struct Token Scanner_next(struct Scanner* scanner);

struct TokenStream Scanner_to_TokenStream(struct Scanner* scanner);

#endif // SCANNER_H
