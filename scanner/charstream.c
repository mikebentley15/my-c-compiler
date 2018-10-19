#include "charstream.h"

#include <stdio.h>

void CharStream_init(CharStream* stream, const char* filename) {
  if (stream->is_init) {
    CharStream_close(stream);
  }
  stream->streamtype = CHAR_STREAM_TYPE_FILE;
  stream->is_init = true;
  stream->file = fopen(filename, "r");
  // TODO: check stream->file for NULL.  Maybe didn't open properly
  stream->contents = NULL;
}

void CharStream_init_string(CharStream* stream, const char* contents) {
  if (stream->is_init) {
    CharStream_close(stream);
  }
  stream->streamtype = CHAR_STREAM_TYPE_STRING;
  stream->is_init = true;
  stream->file = NULL;
  stream->contents = contents;
}

void CharStream_close(CharStream* stream) {
  if (stream->streamtype == CHAR_STREAM_TYPE_FILE) {
    fclose(stream->file);
  }
  stream->streamtype = CHAR_STREAM_TYPE_NONE;
  stream->is_init = false;
  stream->file = NULL;
  stream->contents = NULL;
}

bool CharStream_is_eof(CharStream* stream) {
  bool is_eof = false;
  if (stream->type == CHAR_STREAM_TYPE_FILE) {
    is_eof = feof(stream->file);
  } else if (stream->type == CHAR_STREAM_TYPE_STRING) {
    is_eof = (*contents == '\0');
  }
  return is_eof;
}

char CharStream_readchar(CharStream* stream) {
  char ch;
  if (stream->type == CHAR_STREAM_TYPE_FILE) {
    ch = (char) fgetc(stream->file);
  } else if (stream->type == CHAR_STREAM_TYPE_STRING) {
    ch = *contents;
    contents = contents + 1;
  }
}

