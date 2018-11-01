#include "stringstream.h"

#include <stddef.h> // for NULL

void SS_init(struct StringStream* stream, const char* content) {
  if (stream->is_init) {
    SS_close(stream);
  }
  stream->content = content;
  stream->lineno  = 1;
  stream->column  = 0;
  stream->index   = 0;
  stream->is_init = true;
}

void SS_close(struct StringStream* stream) {
  stream->content = NULL;
  stream->lineno  = 0;
  stream->column  = 0;
  stream->index   = 0;
  stream->is_init = false;
}

bool SS_is_eof(struct StringStream* stream) {
  return stream->content[stream->index] == '\0';
}

char SS_get_char(struct StringStream* stream) {
  char ch = stream->content[stream->index];
  if (!SS_is_eof(stream)) {
    stream->index++;
    if (ch == '\n') {
      stream->lineno++;
      stream->column = 0;
    } else {
      stream->column++;
    }
  }
  return ch;
}

char        SS_private_CS_get_char(void* stream) {
  struct StringStream* ss = (struct StringStream*) stream;
  return SS_get_char(ss);
}

int         SS_private_CS_get_lineno(void* stream) {
  struct StringStream* ss = (struct StringStream*) stream;
  return ss->lineno;
}

int         SS_private_CS_get_column(void* stream) {
  struct StringStream* ss = (struct StringStream*) stream;
  return ss->column;
}

const char* SS_private_CS_get_filepath(void* stream) {
  return "<content>";
}

bool        SS_private_CS_is_eof(void* stream) {
  struct StringStream* ss = (struct StringStream*) stream;
  return SS_is_eof(ss);
}

void        SS_private_CS_close(void* stream) {
  struct StringStream* ss = (struct StringStream*) stream;
  SS_close(ss);
}

struct CharStream SS_to_CharStream(struct StringStream* stream) {
  struct CharStream cs;
  cs.arg          = stream;
  cs.get_char     = &SS_private_CS_get_char;
  cs.get_lineno   = &SS_private_CS_get_lineno;
  cs.get_column   = &SS_private_CS_get_column;
  cs.get_filepath = &SS_private_CS_get_filepath;
  cs.is_eof       = &SS_private_CS_is_eof;
  cs.close        = &SS_private_CS_close;
  cs.is_init      = true;
  return cs;
}

