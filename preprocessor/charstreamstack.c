#include "charstreamstack.h"

struct CharStream* CSS_p_tail(struct CharStreamStack *css) {
  struct CharStream* cs = NULL;
  if (css->streams.size > 0) {
    cs = (struct CharStream*) V_at(&(css->streams), css->streams.size - 1);
  }
  return cs;
}

char CSS_p_get_char(void* arg) {
  struct CharStreamStack *css = (struct CharStreamStack*) arg;
  char ch;
  struct CharStream *cs = CSS_p_tail(css);

  if (cs != NULL) {
    ch = CS_get_char(cs);
    while (cs != NULL && CS_is_eof(cs)) {
      CS_close(cs);
      LL_push_head(&(css->finished), cs);
      V_pop(&(css->streams));
      cs = CSS_p_tail(css);
    }
  } else {
    ch = '\0'
  }

  return ch;
}

int CSS_p_get_lineno(void* arg) {
  struct CharStreamStack *css = (struct CharStreamStack*) arg;
  struct CharStream *cs = CSS_p_tail(css);
  int lineno = 0;
  if (cs != NULL) {
    lineno = CS_get_lineno(cs);
  }
  return lineno;
}

int CSS_p_get_column(void* arg) {
  struct CharStreamStack *css = (struct CharStreamStack*) arg;
  struct CharStream *cs = CSS_p_tail(css);
  int column = 0;
  if (cs != NULL) {
    column = CS_get_column(cs);
  }
  return column;
}

const char* CSS_p_get_filepath(void* arg) {
  struct CharStreamStack *css = (struct CharStreamStack*) arg;
  struct CharStream *cs = CSS_p_tail(css);
  const char* filepath = NULL;
  if (cs != NULL) {
    filepath = CS_get_filepath(cs);
  }
  return filepath;
}

bool CSS_p_is_eof(void* arg) {
  struct CharStreamStack *css = (struct CharStreamStack*) arg;
  return CSS_is_eof(css);
}

void CSS_p_close(void* arg) {
  struct CharStreamStack *css = (struct CharStreamStack*) arg;
  CSS_close(css);
}

void CSS_init(struct CharStreamStack *css) {
  V_init(&css->streams);
  LL_init(&css->finished);
  css->cs.arg          = css;
  css->cs.get_char     = &CSS_p_get_char;
  css->cs.get_lineno   = &CSS_p_get_lineno;
  css->cs.get_column   = &CSS_p_get_column;
  css->cs.get_filepath = &CSS_p_get_filepath;
  css->cs.is_eof       = &CSS_p_is_eof;
  css->cs.close        = &CSS_p_close;
}

void CSS_del(struct CharStreamStack *css) {
  V_del(&(css->streams));
  LL_clear(&(css->finished));
}

void CSS_close(struct CharStreamStack *css) {
  struct CharStream *cs = CSS_p_tail(css);
  while (cs != NULL) {
    CS_close(cs);
    LL_push_head(&(css->finished), cs);
    V_pop(&(css->streams));
    cs = CSS_p_tail(css);
  }
}

void CSS_push(struct CharStreamStack *css, struct CharStream *cs) {
  V_push(&(css->streams), cs);
}

bool CSS_is_eof(struct CharStreamStack *css) {
  return css->streams.size > 0;
}

struct CharStream CSS_to_CharStream(struct CharStreamStack *css) {
  return css->cs;
}
