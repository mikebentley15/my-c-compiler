#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include "charstream.h"
#include "bootstrap.h"

struct StringStream {
  bool is_init;
  const char* content;
  int lineno;
  int column;
  int index;
};

void SS_init(struct StringStream* stream, const char* content);
void SS_close(struct StringStream* stream);
bool SS_is_eof(struct StringStream* stream);
char SS_get_char(struct StringStream* stream);

struct CharStream SS_to_CharStream(struct StringStream* stream);

#endif // STRINGSTREAM_H
