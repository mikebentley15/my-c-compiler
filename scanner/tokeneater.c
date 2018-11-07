#include "tokeneater.h"

#include <stddef.h> // for NULL

void TokenEater_init(struct TokenEater *f, struct TokenStream *in) {
  f->in = in;
  Set_init(&(f->types), 200); // 200 should be plenty for this
}

void TokenEater_del(struct TokenEater *f) {
  f->in = NULL;
  Set_del(&(f->types));
}

void TokenEater_add(struct TokenEater *f, enum TokenType type) {
  Set_add(&(f->types), (void*) type);
}

struct Token TokenEater_next(struct TokenEater *f) {
  struct Token tok;
  tok = TS_next_token(f->in);
  while (Set_contains(&(f->types), (void*) tok.type)) {
    tok = TS_next_token(f->in);
  }
  return tok;
}

struct Token TokenEater_p_next_token(void* arg) {
  struct TokenEater *eater;
  return TokenEater_next(eater);
}

int TokenEater_p_get_lineno(void* arg) {
  struct TokenEater *eater;
  return TS_get_lineno(eater->in);
}

int TokenEater_p_get_column(void* arg) {
  struct TokenEater *eater;
  return TS_get_column(eater->in);
}

const char* TokenEater_p_get_filepath(void* arg) {
  struct TokenEater *eater;
  return TS_get_filepath(eater->in);
}

bool TokenEater_p_is_eof(void* arg) {
  struct TokenEater *eater;
  return TS_is_eof(eater->in);
}

void TokenEater_p_close(void* arg) {
  struct TokenEater *eater;
  return TS_close(eater->in);
}

struct TokenStream TokenEater_to_TokenStream(struct TokenEater *f) {
  struct TokenStream ts;
  ts.arg = f;
  ts.next_token   = &TokenEater_p_next_token;
  ts.get_lineno   = &TokenEater_p_get_lineno;
  ts.get_column   = &TokenEater_p_get_column;
  ts.get_filepath = &TokenEater_p_get_filepath;
  ts.is_eof       = &TokenEater_p_is_eof;
  ts.close        = &TokenEater_p_close;
  return ts;
}

