#include "token.h"

struct Token TS_next_token(struct TokenStream* ts) {
  return ts->next_token(ts->arg);
}

int TS_get_lineno(struct TokenStream* ts) {
  return ts->get_lineno(ts->arg);
}

int TS_get_column(struct TokenStream* ts) {
  return ts->get_column(ts->arg);
}

const char* TS_get_filepath(struct TokenStream* ts) {
  return ts->get_filepath(ts->arg);
}

bool TS_is_eof(struct TokenStream* ts) {
  return ts->is_eof(ts->arg);
}

void TS_close(struct TokenStream* ts) {
  return ts->close(ts->arg);
}
