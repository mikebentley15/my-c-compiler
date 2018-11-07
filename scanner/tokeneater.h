#ifndef TOKENEATER_H
#define TOKENEATER_H

#include "token.h"
#include "set.h"

struct TokenEater {
  struct TokenStream *in;
  struct Set types;
};

void TokenEater_init(struct TokenEater *f, struct TokenStream *in);
void TokenEater_del(struct TokenEater *f);
void TokenEater_add(struct TokenEater *f, enum TokenType type);
struct Token TokenEater_next(struct TokenEater *f);

struct TokenStream TokenEater_to_TokenStream(struct TokenEater *f);

#endif // TOKENEATER_H
