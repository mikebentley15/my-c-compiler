#ifndef KEYWORD_MAPPER_H
#define KEYWORD_MAPPER_H

#include "token.h"
#include "dict.h"

// Simply filters Identifier tokens into separated TokenTypes
struct KeywordMapper {
  struct TokenStream *in;   // input stream of tokens
  struct Dict *map;         // all keywords -> enum TokenType
  struct Dict *pp_map;      // preprocess keywords -> enum TokenType
};

void KeywordMapper_init(struct KeywordMapper *f, struct TokenStream *stream);
void KeywordMapper_del(struct KeywordMapper *f);
struct Token KeywordMapper_next(struct KeywordMapper *f, struct Token tok);

struct TokenStream KeywordMapper_to_TokenStream(struct KeywordMapper *f);

#endif // KEYWORD_MAPPER_H
