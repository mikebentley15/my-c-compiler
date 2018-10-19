#ifndef CHARSTREAM_H
#define CHARSTREAM_H

enum CharStreamType {
  CHAR_STREAM_TYPE_NONE = 0,
  CHAR_STREAM_TYPE_FILE = 1,
  CHAR_STREAM_TYPE_STRING = 2,
};

// Intended to be a stream made from a file or other sources
// For example, from a static string
// This is helpful at least for testing purposes
struct CharStream {
  CharStreamType type;
  bool is_init;
  FILE* file;
  const char* contents;
};

void CharStream_init(CharStream* stream, const char* filename);
void CharStream_init_string(CharStream* stream, const char* contents);
void CharStream_close(CharStream* stream);
bool CharStream_is_eof(CharStream* stream);
char CharStream_readchar(CharStream* stream);

#endif // CHARSTREAM_H
