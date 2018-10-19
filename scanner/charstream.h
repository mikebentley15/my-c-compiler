#ifndef CHARSTREAM_H
#define CHARSTREAM_H

#include <stdio.h>

enum CharStreamType {
  CHAR_STREAM_TYPE_NONE = 0,
  CHAR_STREAM_TYPE_FILE = 1,
  CHAR_STREAM_TYPE_STRING = 2,
};

// Intended to be a stream made from a file or other sources
// For example, from a static string
// This is helpful at least for testing purposes
struct CharStream {
  enum CharStreamType type;
  bool is_init;
  FILE* file;
  const char* contents;
};

void CharStream_init(struct CharStream* stream, const char* filename);
void CharStream_init_string(struct CharStream* stream, const char* contents);
void CharStream_close(struct CharStream* stream);
bool CharStream_is_eof(struct CharStream* stream);
char CharStream_readchar(struct CharStream* stream);

#endif // CHARSTREAM_H
