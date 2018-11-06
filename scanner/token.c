#include "token.h"

#include "stdlib.h" // for malloc() and free()

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

void TSQ_init(struct TokenStreamQueue* tsq) {
  LL_init(&(tsq->q));
  tsq->lineno = 1;
  tsq->column = 0;
}

void TSQ_close(struct TokenStreamQueue* tsq) {
  TSQ_clear(tsq);
  tsq->lineno = 1;
  tsq->column = 0;
}

void TSQ_clear(struct TokenStreamQueue* tsq) {
  struct Token* tok;
  while (tsq->q.size > 0) {
    tok = (struct Token*) LL_pop_tail(&(tsq->q));
    free(tok);
  }
}

void TSQ_push(struct TokenStreamQueue* tsq, struct Token tok) {
  struct Token *htok = (struct Token*) malloc(sizeof(struct Token));
  htok->type = tok.type;
  htok->strval = tok.strval;
  htok->intval = tok.intval;
  LL_push_head(&(tsq->q), htok);
}

struct Token TSQ_pop(struct TokenStreamQueue* tsq) {
  struct Token *htok;
  struct Token tok;
  int i;
  if (tsq->q.size > 0) {
    htok = LL_pop_tail(&(tsq->q));
    tok.type = htok->type;
    tok.strval = htok->strval;
    tok.intval = htok->intval;
    free(htok);
  } else {
    tok.type = TT_EOF;
    tok.strval = "";
    tok.intval = 0;
  }

  // capture line and column information
  if (tok.type == TT_NEWLINE) {
    tsq->lineno++;
    tsq->column = 0;
  } else {
    i = 0;
    while (tok.strval[i] != '\0') {
      i++;
    }
    tsq->column = tsq->column + i;
  }

  return tok;
}

struct Token TSQ_p_next_token(void* arg) {
  struct TokenStreamQueue *tsq = (struct TokenStreamQueue*) arg;
  return TSQ_pop(tsq);
}

int TSQ_p_get_lineno(void* arg) {
  struct TokenStreamQueue *tsq = (struct TokenStreamQueue*) arg;
  return tsq->lineno;
}

int TSQ_p_get_column(void* arg) {
  struct TokenStreamQueue *tsq = (struct TokenStreamQueue*) arg;
  return tsq->column;
}

const char* TSQ_p_get_filepath(void* arg) {
  return "<TokenStreamQueue>";
}

bool TSQ_p_is_eof(void* arg) {
  struct TokenStreamQueue *tsq = (struct TokenStreamQueue*) arg;
  return tsq->q.size == 0;
}

void TSQ_p_close(void* arg) {
  struct TokenStreamQueue *tsq = (struct TokenStreamQueue*) arg;
  TSQ_close(tsq);
}

struct TokenStream TSQ_to_TS(struct TokenStreamQueue* tsq) {
  struct TokenStream ts;
  ts.arg          = tsq;
  ts.next_token   = &TSQ_p_next_token;
  ts.get_lineno   = &TSQ_p_get_lineno;
  ts.get_column   = &TSQ_p_get_column;
  ts.get_filepath = &TSQ_p_get_filepath;
  ts.is_eof       = &TSQ_p_is_eof;
  ts.close        = &TSQ_p_close;
  return ts;
}

