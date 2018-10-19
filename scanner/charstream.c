#include "charstream.h"

#include <stdio.h>

void CharStream_init(struct CharStream* stream, const char* filename) {
  if (stream->is_init) {
    CharStream_close(stream);
  }
  stream->type = CHAR_STREAM_TYPE_FILE;
  stream->is_init = true;
  stream->file = fopen(filename, "r");
  // TODO: check stream->file for NULL.  Maybe didn't open properly
  stream->contents = NULL;
}

void CharStream_init_string(struct CharStream* stream, const char* contents) {
  if (stream->is_init) {
    CharStream_close(stream);
  }
  stream->type = CHAR_STREAM_TYPE_STRING;
  stream->is_init = true;
  stream->file = NULL;
  stream->contents = contents;
}

void CharStream_close(struct CharStream* stream) {
  if (stream->type == CHAR_STREAM_TYPE_FILE) {
    fclose(stream->file);
  }
  stream->type = CHAR_STREAM_TYPE_NONE;
  stream->is_init = false;
  stream->file = NULL;
  stream->contents = NULL;
}

bool CharStream_is_eof(struct CharStream* stream) {
  bool is_eof = false;
  if (stream->type == CHAR_STREAM_TYPE_FILE) {
    is_eof = feof(stream->file);
  } else if (stream->type == CHAR_STREAM_TYPE_STRING) {
    is_eof = (stream->contents[0] == '\0');
  }
  return is_eof;
}

char CharStream_readchar(struct CharStream* stream) {
  char ch;
  if (stream->type == CHAR_STREAM_TYPE_FILE) {
    ch = (char) fgetc(stream->file);
  } else if (stream->type == CHAR_STREAM_TYPE_STRING) {
    ch = stream->contents[0];
    stream->contents = stream->contents + 1;
  }
  return ch;
}

