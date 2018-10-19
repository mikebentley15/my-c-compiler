#include "scanner.h"

void Scanner_init(struct Scanner* scanner, struct CharStream* stream) {
  if (scanner->is_init) {
    Scanner_close(scanner);
  }
  scanner->in = stream;
  scanner->is_init = true;
  scanner->is_eof = CharStream_is_eof(scanner->in);
  if (!scanner->is_eof) {
    scanner->next = CharStream_readchar(scanner->in);
  }
  scanner->tok_buf[0] = '\0';
}

void Scanner_close(struct Scanner* scanner) {
  scanner->in = NULL;
  scanner->is_init = false;
  scanner->is_eof = false;
  scanner->next = '\0';
  scanner->tok_buf[0] = '\0';
}

bool Scanner_is_eof(struct Scanner* scanner) {
  return scanner->is_eof;
}

struct Token Scanner_next(struct Scanner* scanner) {
  struct Token tok;
  tok.type = TOKEN_TYPE_ERROR;
  tok.value = scanner->tok_buf;
  // TODO: use a preprocessor here maybe
  // TODO: implement
  return tok;
}

