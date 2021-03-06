#include "charstream.h"

void  CS_close(struct CharStream* stream) {
  stream->close(stream->arg);
  stream->arg = NULL;
  stream->get_char = NULL;
  stream->get_lineno = NULL;
  stream->get_column = NULL;
  stream->get_filepath = NULL;
  stream->is_eof = NULL;
  stream->close = NULL;
  stream->is_init = false;
}

char  CS_get_char(struct CharStream* stream) {
  return stream->get_char(stream->arg);
}

int   CS_get_lineno(struct CharStream* stream) {
  return stream->get_lineno(stream->arg);
}

int   CS_get_column(struct CharStream* stream) {
  return stream->get_column(stream->arg);
}

const char* CS_get_filepath(struct CharStream* stream) {
  return stream->get_filepath(stream->arg);
}

bool  CS_is_eof(struct CharStream* stream) {
  return stream->is_eof(stream->arg);
}
