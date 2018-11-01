#include "filestream.h"

#include <stdio.h>

void  FS_init(struct FileStream* stream, const char* filepath) {
  if (stream->is_init) {
    FS_close(stream);
  }
  stream->filepath = filepath;
  stream->file = fopen(filepath, "r");
  stream->lineno = 1;
  stream->is_init = true;
}

void  FS_close(struct FileStream* stream) {
  fclose(stream->file);
  stream->file = NULL;
  stream->filepath = NULL;
  stream->lineno = 0;
  stream->is_init = false;
}

bool  FS_is_eof(struct FileStream* stream) {
  return feof(stream->file);
}

char  FS_get_char(struct FileStream* stream) {
  char ch = (char) fgetc(stream->file);
  if (ch == '\n') {
    stream->lineno++;
  }
  return ch;
}

char        FS_private_CS_get_char(void* stream) {
  struct FileStream* fs = (struct FileStream*) stream;
  return FS_get_char(fs);
}

int         FS_private_CS_get_lineno(void* stream) {
  struct FileStream* fs = (struct FileStream*) stream;
  return fs->lineno;
}

const char* FS_private_CS_get_filepath(void* stream) {
  struct FileStream* fs = (struct FileStream*) stream;
  return fs->filepath;
}

bool        FS_private_CS_is_eof(void* stream) {
  struct FileStream* fs = (struct FileStream*) stream;
  return FS_is_eof(fs);
}

void        FS_private_CS_close(void* stream) {
  struct FileStream* fs = (struct FileStream*) stream;
  FS_close(fs);
}

CharStream FS_to_CharStream(struct FileStream* stream) {
  CharStream cs;
  cs.arg          = stream;
  cs.get_char     = &FS_private_CS_get_char;
  cs.get_lineno   = &FS_private_CS_get_lineno;
  cs.get_filepath = &FS_private_CS_get_filepath;
  cs.is_eof       = &FS_private_CS_is_eof;
  cs.close        = &FS_private_CS_close;
  cs.is_init      = true;
  return cs;
}

