#ifndef INCLUDER_H
#define INCLUDER_H

#include "vector.h"

struct FileOpener {
  void* arg;
  struct FileStream* (*open_file)(void*, const char*);
};

struct Includer {
  struct Vector path;     // search paths (char*)
  char separator;         // path separator
};

void Includer_init(struct Includer *inc);
void Includer_set_separator(struct Includer *inc, char separator);
void Includer_del(struct Includer *inc);
void Includer_add_path(struct Includer *inc, const char* path); // will copy it

// returns NULL on failure
// caller must close and free the FileStream if not NULL
struct FileStream* Includer_open(struct Includer *inc, const char* file);

struct FileOpener Includer_to_FileOpener(struct Includer *inc);

#endif // INCLUDER_H
