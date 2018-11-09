#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "bootstrap.h"
#include "charstreamstack.h"
#include "includer.h"
#include "stringstream.h"
#include "scanner.h"
#include "keywordmapper.h"
#include "dict.h"

struct Preprocessor {
  struct CharStreamStack streams; // all streams
  struct FileOpener *opener;      // how we #include
  struct StringStream last_tok;   // last scanned token as a stream
  struct Scanner scanner;         // scans for preprocess tokens
  struct KeywordMapper mapper;    // identifies preproc. directives
  struct Dict macro_vars;         // from #define
};

void PP_init(struct Preprocessor *pp, struct FileOpener *opener,
             struct CharStream input);
void PP_del(struct Preprocessor *pp);

// copies the incoming strings
void PP_set_macro(struct Preprocessor *pp, const char* name, const char* val);
const char* PP_get_macro(struct Preprocessor *pp, const char* name);
void PP_del_macro(struct Preprocessor *pp, const char* name);

bool PP_is_eof(struct Preprocessor *pp);
char PP_get_char(struct Preprocessor *pp);

struct CharStream PP_to_CharStream(struct Preprocessor *pp);

#endif // PREPROCESS_H
