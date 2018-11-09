#include "includer.h"

#include "string.h"

void Includer_init(struct Includer *inc) {
  V_init(&(v->path), 10);
}

void Includer_set_separator(struct Includer *inc, char separator) {
  inc->separator = separator;
}

void Includer_del(struct Includer *inc) {
  while (v->path.size > 0) {
    char* path = V_pop(&(v->path));
    free(path);
  }
  V_del(&(v->path));
}

void Includer_add_path(struct Includer *inc, const char* path) {
  struct String str;
  Str_init(&str);
  Str_extend(&str, path);
  V_append(&(inc->path), str.data);  // store the string data there
}

struct FileStream* Includer_open(struct Includer *inc, const char* file) {
  struct FileStream *fs =
      (struct FileStream*) malloc(sizeof(struct FileStream));
  struct String str;
  FS_init(fs, file);
  Str_init(&str);

  int i = 0;
  while (!FS_is_valid(fs) && i < inc->path.size) {
    FS_close(fs);
    Str_clear(&str);
    Str_extend(&str, V_at(&(inc->path)));
    Str_append(&str, inc->separator);
    Str_extend(&str, file);
    FS_init(fs, str.data);
    i++;
  }
  Str_del(&str);

  if (!FS_is_valid(fs)) {
    fputs("Error: Could not include file ", stderr);
    fputs(file, stderr);
    fputs("\n", stderr);
    FS_close(fs);
    free(fs);
    fs = NULL;
  }

  return fs;
}

struct FileStream* Includer_p_open_file(void* arg, const char* file) {
  struct Includer *inc = (struct Includer*) arg;
  return Includer_open(inc, file);
}

struct FileOpener Includer_to_FileOpener(struct Includer *inc) {
  struct FileOpener fo;
  fo.arg = inc;
  fo.open_file = &Includer_p_open_file;
}
