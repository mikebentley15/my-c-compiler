#ifndef CHARSTREAM_H
#define CHARSTREAM_H

#include "bootstrap.h"

// Define an input stream in C
// This is one way to do polymorphism in C
// The arg will be the struct, and the three functions are callbacks using that
// struct.
struct CharStream {
  void*         arg;
  char        (*get_char)(void*);
  int         (*get_lineno)(void*);
  int         (*get_column)(void*);
  const char* (*get_filepath)(void*);
  bool        (*is_eof)(void*);
  void        (*close)(void*);
};

void        CS_close(struct CharStream* stream);
char        CS_get_char(struct CharStream* stream);
int         CS_get_lineno(struct CharStream* stream);
int         CS_get_column(struct CharStream* stream);
const char* CS_get_filepath(struct CharStream* stream);
bool        CS_is_eof(struct CharStream* stream);

#endif // CHARSTREAM_H
