#include "preprocessor.h"


struct MacroVal {
  char* val;
  bool is_deleted;
};

struct MacroVal* PP_p_create_val(const char* val) {
  struct MacroVal *val = (struct MacroVal*) malloc(sizeof(struct MacroVal));
  val->val = strdup(val);
  val->is_deleted = false;
  return val;
}

void PP_p_del_val(struct MacroVal* val) {
  free(val->val);
  val->val = NULL;
  val->is_deleted = true;
  free(val);
}

bool PP_p_read_file(struct Preprocessor *pp, const char* file) {
  struct FileStream* fs = pp->opener->open_file(pp->opener->arg);
  struct CharStream* cs = NULL;
  bool success = (fs != NULL);
  if (success) {
    cs = (struct CharStream*) malloc(sizeof(struct CharStream));
    *cs = FS_to_CharStream(fs);
    CSS_push(&(pp->streams), cs);
    pp->top_fstream = cs;
  } else {
    error("Failed to include file, cannot continue");
  }
  return success;
}

void PP_p_read_string(struct Preprocessor *pp, const char* content) {
  struct StringStream *ss =
      (struct StringStream*) malloc(sizeof(struct StringStream));
  SS_init(ss, content);
  struct CharStream *cs =
      (struct CharStream*) malloc(sizeof(struct CharStream));
  *cs = SS_to_CharStream(ss);
  CSS_push(&(pp->streams), cs);
}

void PP_init(struct Preprocessor *pp,
             struct FileOpener *opener
             struct CharStream input)
{
  struct CharStream* cs =
      (struct CharStream*) malloc(sizeof(struct CharStream));
  *cs = input;
  CSS_init(&(pp->streams));
  CSS_push(&(pp->streams), cs);
  pp->opener = opener;
  SS_init(&(pp->last_tok), "");
  Scanner_init(&(pp->scanner), &(pp->streams.cs));
  KeywordMapper_init(&(pp->mapper), NULL);
  Dict_init(&(pp->macro_vars));
}

void PP_del(struct Preprocessor *pp) {
  struct CharStream *cs;
  int i = 0;

  CSS_close(&(pp->streams));

  // delete from the given input stream from PP_init()
  cs = (struct CharStream*) LL_pop_head(&(pp->streams.finished));
  free(cs);

  // delete all created streams from the preprocessor
  while (pp->streams.finished.size > 0) {
    cs = (struct CharStream*) LL_pop_head(&(pp->streams.finished));
    free(cs->arg);
    free(cs);
  }

  CSS_del(&(pp->streams));
  opener = NULL;
  SS_close(&(pp->last_tok));
  Scanner_close(&(pp->scanner));
  KeywordMapper_del(&(pp->mapper));

  i = 0;
  while (i < pp->macro_vars.size) {
    if (pp->macro_vars.vals[i] != NULL) {
      PP_p_del_val((struct MacroVal*) pp->macro_vars.vals[i]);
      pp->macro_vars.vals[i] = NULL;
    }
  }

  Dict_del(&(pp->macro_vars));
}

bool PP_set_macro(struct Preprocessor *pp, const char* name, const char* val) {
  struct MacroVal *node = (struct MacroVal*) Dict_at(&(pp->macro_vars), name);
  bool already_exists = false;

  if (node == NULL) {
    struct MacroVal *node = PP_p_create_val(val);
    Dict_set(&(pp->macro_vars), name, node);
  } else if (node->is_deleted) {
    node->is_deleted = false;
    free(node->val);
    node->val = strdup(val);
  } else {
    // TODO: print line, column, and filename
    already_exists = true;
    fputs("Warning: redefinition of macro variable \"", stderr);
    fputs(name, stderr);
    fputs("\", ignoring");
  }

  return !already_exists;
}

const char* PP_get_macro(struct Preprocessor *pp, const char* name) {
  struct MacroVal *node = (struct MacroVal*) Dict_at(&(pp->macro_vars), name);
  const char* val = NULL;
  if (node != NULL && !node->is_deleted) {
    val = node->val;
  }
  return val;
}

void PP_del_macro(struct Preprocessor *pp, const char* name) {
  // doesn't actually remove it from the Dict,
  // but instead marks it as deleted
  struct MacroVal *node = (struct MacroVal*) Dict_at(&(pp->macro_vars), name);
  if (node != NULL) {
    if (!node->is_deleted) {
      node->is_deleted = true;
    } else {
      // TODO: print line, column, and filename
      fputs("Warning: macro variable is already undefined: \"", stderr);
      fputs(name, stderr);
      fputs("\", ignoring", stderr);
    }
  } else {
    // TODO: print line, column, and filename
    fputs("Warning: cannot undefine a macro that doesn't exist: \"", stderr);
    fputs(name, stderr);
    fputs("\", ignoring", stderr);
  }
}

bool PP_is_eof(struct Preprocessor *pp) {
  return CSS_is_eof(&(pp->streams));
}

char PP_get_char(struct Preprocessor *pp) {
  char ch = '\0';
  struct CharStream cs;
  struct Token tok;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  bool found_char;
  const char* macro;
  struct String buf;
  Str_init(&buf);

  // here is where preprocessing is done
  if (!SS_is_eof(&(pp->last_tok))) {
    ch = SS_get_char(&(pp->last_tok));
  } else {
    // Scan until I'm sure there is nothing to expand
    found_char = false;
    while (!found_char) {
      tok = Scanner_next(&(pp->scanner));
      if (tok.type == TT_POUND) {
        // prepare the queue for the keyword mapper
        TSQ_init(&tsq);
        TSQ_push(&tsq, tok);
        // skip whitespace
        tok.type = TT_WHITESPACE;
        while (tok.type == TT_WHITESPACE) {
          tok = Scanner_next(&(pp->scanner))
        }
        TSQ_push(&tsq, tok);
        ts = TSQ_to_TS(&tsq);
        pp->mapper.in = &ts;

        tok = KeywordMapper_next(&(pp->mapper));

        pp->mapper.in = NULL; // set to NULL just to be safe
      } else if (tok.type == TT_IDENTIFIER) {
        // TODO: see if it is in macro_vars
        macro = PP_get_macro(pp, tok.strval);
        if (macro == NULL) {
          found_char = true;
          SS_close(&(pp->last_tok));
          SS_init(&(pp->last_tok), tok.strval);
          ch = SS_get_char(&(pp->last_tok));
        } else {
          PP_p_read_string(pp, macro);
        }
      }

      // parse preprocessor directives
      if (tok.type == TT_PP_INCLUDE) {
        // skip whitespace
        tok.type = TT_WHITESPACE;
        while (tok.type == TT_WHITESPACE || tok.type == TT_LINE_CONTINUATION) {
          tok = Scanner_next(&(pp->scanner));
        }
        if (tok.type == TT_STRING_LITERAL) { // #include "..."
          PP_p_read_file(pp, tok.strval);
        } else if (tok.type == TT_LESS) {    // #include <...>
          while (tok.type != TT_GREATER && tok.type != TT_EOF) {
            tok = Scanner_next(&(pp->scanner));
            Str_extend(&buf, tok.strval);
          }
          if (tok.type == TT_GREATER) {
            PP_p_read_file(pp, buf.data);
          } else { // tok.type == TT_EOF
            // TODO: error case
          }
        } else {
          // TODO: error case
        }
      } else if (tok.type == TT_PP_IFDEF) {
        // go until the #else or #endif
        // TODO: need to nest here, so need to do recursive descent
      } else if (tok.type == TT_PP_IFNDEF) {
        // TODO: need to nest here, so need to do recursive descent
      } else if (tok.type == TT_PP_ELSE) {
        // TODO: error, not in an #ifdef or #ifndef
      } else if (tok.type == TT_PP_ENDIF) {
        // TODO: error, not in an #ifdef or #ifndef
      } else if (tok.type == TT_PP_DEFINE) {
        // TODO: finish
      } else if (tok.type == TT_PP_UNDEF) {
        // TODO: finish
      } else if (tok.type == TT_PP_PRAGMA) {
        // TODO: finish
      }
    }
        // TODO: finish
  }

        // TODO: finish
  struct String buf;
  Str_init(&buf);
  struct StringStream ss;
  ss->content = buf.data;
  ss->lineno = 1;
  ss->column = 0;
  ss->index = 0;

  

  return ch;
}

struct CharStream* PP_p_get_top_fstream(struct Preprocessor* pp) {
  struct CharStream* cs = NULL;
  int i = pp->streams.streams.size - 1;
  if (i >= 0) {
    while (i > 0) {
      cs = (struct CharStream*) V_at(&(pp->streams.streams), i);
      if (!strequal(CS_filepath(cs), "<StringStream>")) {
        break;
      }
      i--;
    }
    if (i == 0) {
      cs = (struct CharStream*) V_at(&(pp->streams.streams), i);
    }
  }
  return cs;
}

char PP_p_get_char(void* arg) {
  struct Preprocessor *pp = (struct Preprocessor*) arg;
  return PP_get_char(pp);
}

int  PP_p_get_lineno(void* arg) {
  struct Preprocessor *pp = (struct Preprocessor*) arg;
  struct CharStream* cs = PP_p_get_top_fstream(pp);
  return CS_get_lineno(cs);
}

int  PP_p_get_column(void* arg) {
  struct Preprocessor *pp = (struct Preprocessor*) arg;
  struct CharStream* cs = PP_p_get_top_fstream(pp);
  return CS_get_column(cs);
}

const char* PP_p_get_filepath(void* arg) {
  struct Preprocessor *pp = (struct Preprocessor*) arg;
  struct CharStream* cs = PP_p_get_top_fstream(pp);
  return CS_get_filepath(cs);
}

bool PP_p_is_eof(void* arg) {
  struct Preprocessor *pp = (struct Preprocessor*) arg;
  return PP_is_eof(pp);
}

void PP_p_close(void* arg) {
  struct Preprocessor *pp = (struct Preprocessor*) arg;
  PP_del(pp);
}

struct CharStream PP_to_CharStream(struct Preprocessor *pp) {
  struct CharStream cs;
  cs.arg          = pp;
  cs.get_char     = &PP_p_get_char;
  cs.get_lineno   = &PP_p_get_lineno;
  cs.get_column   = &PP_p_get_column;
  cs.get_filepath = &PP_p_get_filepath;
  cs.is_eof       = &PP_p_is_eof;
  cs.close        = &PP_p_close;
  return cs;
}
