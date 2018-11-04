#ifndef KEYWORD_FILTER_H
#define KEYWORD_FILTER_H

#include "token.h"
#include "dict.h"

// Simply filters Identifier tokens into separated TokenTypes
struct KeywordFilter {
  struct TokenStream *in;   // input stream of tokens
  struct Dict *map;         // all keywords -> enum TokenType
  struct Dict *pp_map;      // preprocess keywords -> enum TokenType
};

void KeywordFilter_init(struct KeywordFilter *f, struct TokenStream *stream);
void KeywordFilter_del(struct KeywordFilter *f);
struct Token KeywordFilter_next(struct KeywordFilter *f, struct Token tok);

struct TokenStream KeywordFilter_to_TokenStream(struct KeywordFilter *f);

#endif // KEYWORD_FILTER_H
