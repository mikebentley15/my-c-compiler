#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "charstream.h"
#include "bootstrap.h"

struct FileStream {
  bool  is_init;
  FILE* file;
  const char* filepath;
  int   lineno;
  int   column;
};

void  FS_init(struct FileStream* stream, const char* filepath);
void  FS_close(struct FileStream* stream);
bool  FS_is_eof(struct FileStream* stream);
char  FS_get_char(struct FileStream* stream);

CharStream FS_to_CharStream(struct FileStream* stream);

#endif // FILESTREAM_H
