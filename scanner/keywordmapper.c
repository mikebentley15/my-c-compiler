#include "keywordmapper.h"
#include "util.h"

bool p_isUnsupported(enum TokenType type) {
  return (type >= TT_KEYWORD_UNSUPPORTED_BEGIN &&
          type <= TT_KEYWORD_UNSUPPORETD_END) ||
         (type >= TT_PP_UNSUPPORTED_BEGIN &&
          type <= TT_PP_UNSUPPORTED_END);
}

void KeywordMapper_init(struct KeywordMapper *f, struct TokenStream *stream) {
  f->in     = stream;
  f->map    = (struct Dict*) malloc(sizeof(struct Dict));
  f->pp_map = (struct Dict*) malloc(sizeof(struct Dict));
  Dict_init(f->map, 38);
  Dict_init(f->pp_map, 8);

  // create the dictionary of all keywords
  Dict_set(f->map, "_Packed" , (void*) TT_PACKED  );
  Dict_set(f->map, "and"     , (void*) TT_AND     );
  Dict_set(f->map, "auto"    , (void*) TT_AUTO    );
  Dict_set(f->map, "bool"    , (void*) TT_BOOL    );
  Dict_set(f->map, "break"   , (void*) TT_BREAK   );
  Dict_set(f->map, "byte"    , (void*) TT_BYTE    );
  Dict_set(f->map, "case"    , (void*) TT_CASE    );
  Dict_set(f->map, "char"    , (void*) TT_CHAR    );
  Dict_set(f->map, "const"   , (void*) TT_CONST   );
  Dict_set(f->map, "continue", (void*) TT_CONTINUE);
  Dict_set(f->map, "default" , (void*) TT_DEFAULT );
  Dict_set(f->map, "do"      , (void*) TT_DO      );
  Dict_set(f->map, "double"  , (void*) TT_DOUBLE  );
  Dict_set(f->map, "else"    , (void*) TT_ELSE    );
  Dict_set(f->map, "enum"    , (void*) TT_ENUM    );
  Dict_set(f->map, "extern"  , (void*) TT_EXTERN  );
  Dict_set(f->map, "false"   , (void*) TT_FALSE   );
  Dict_set(f->map, "float"   , (void*) TT_FLOAT   );
  Dict_set(f->map, "for"     , (void*) TT_FOR     );
  Dict_set(f->map, "goto"    , (void*) TT_GOTO    );
  Dict_set(f->map, "if"      , (void*) TT_IF      );
  Dict_set(f->map, "int"     , (void*) TT_INT     );
  Dict_set(f->map, "long"    , (void*) TT_LONG    );
  Dict_set(f->map, "or"      , (void*) TT_OR      );
  Dict_set(f->map, "register", (void*) TT_REGISTER);
  Dict_set(f->map, "return"  , (void*) TT_RETURN  );
  Dict_set(f->map, "short"   , (void*) TT_SHORT   );
  Dict_set(f->map, "signed"  , (void*) TT_SIGNED  );
  Dict_set(f->map, "sizeof"  , (void*) TT_SIZEOF  );
  Dict_set(f->map, "static"  , (void*) TT_STATIC  );
  Dict_set(f->map, "struct"  , (void*) TT_STRUCT  );
  Dict_set(f->map, "switch"  , (void*) TT_SWITCH  );
  Dict_set(f->map, "true"    , (void*) TT_TRUE    );
  Dict_set(f->map, "typedef" , (void*) TT_TYPEDEF );
  Dict_set(f->map, "union"   , (void*) TT_UNION   );
  Dict_set(f->map, "void"    , (void*) TT_VOID    );
  Dict_set(f->map, "volatile", (void*) TT_VOLATILE);
  Dict_set(f->map, "while"   , (void*) TT_WHILE   );

  // create the dictionary of all preprocessing directives
  Dict_set(f->pp_map, "include", (void*) TT_PP_INCLUDE);
  Dict_set(f->pp_map, "ifdef"  , (void*) TT_PP_IFDEF  );
  Dict_set(f->pp_map, "ifndef" , (void*) TT_PP_IFNDEF );
  Dict_set(f->pp_map, "else"   , (void*) TT_PP_ELSE   );
  Dict_set(f->pp_map, "endif"  , (void*) TT_PP_ENDIF  );
  Dict_set(f->pp_map, "define" , (void*) TT_PP_DEFINE );
  Dict_set(f->pp_map, "pragma" , (void*) TT_PP_PRAGMA );
  Dict_set(f->pp_map, "undef"  , (void*) TT_PP_UNDEF  );
}

void KeywordMapper_del(struct KeywordMapper *f) {
  Dict_del(f->map);
  Dict_del(f->pp_map);
  free(f->map);
  free(f->pp_map);
}

struct Token KeywordMapper_next(struct KeywordMapper *f) {
  struct Token tok = f->in->next_token(f->in->arg);
  enum TokenType type = TT_ERROR;
  if (tok->type == TT_IDENTIFIER) {
    type = (enum TokenType) Dict_at(f->map, tok->strval);
    if (type != TT_ERROR) {
      tok->type = type;
      if (p_isUnsupported(type)) {
        fputs("Error: unsupported keyword \"", stderr);
        fputs(tok->strval, stderr);
        fputs("\"\n", stderr);
      }
    }
  } else if (tok->type == TT_POUND) {
    tok = f->in->next_token(f->in->arg);
    if (tok->type != TT_IDENTIFIER) {
      fputs("Error: unidentified preprocessing directive \"", stderr);
      fputs(tok->strval, stderr);
      fputs("\"\n", stderr);
    } else {
      type = (enum TokenType) Dict_at(f->pp_map, tok->strval);
      if (type == TT_ERROR) {
        fputs("Error: unidentified preprocessing directive \"", stderr);
        fputs(tok->strval, stderr);
        fputs("\"\n", stderr);
      } else {
        tok->type = type;
      }
      if (p_isUnsupported(type)) {
        fputs("Error: unsupported preprocessing directive \"", stderr);
        fputs(tok->strval, stderr);
        fputs("\"\n", stderr);
      }
    }
  }
  return tok;
}

struct Token KeywordMapper_p_next_token(void *filter) {
  struct KeywordMapper *f = (struct KeywordMapper*) filter;
  return KeywordMapper_next(f);
}

int KeywordMapper_p_get_lineno(void *filter) {
  struct KeywordMapper *f = (struct KeywordMapper*) filter;
  return f->in->get_lineno(f->in->arg);
}

int KeywordMapper_p_get_column(void *filter) {
  struct KeywordMapper *f = (struct KeywordMapper*) filter;
  return f->in->get_column(f->in->arg);
}

const char* KeywordMapper_p_get_filepath(void *filter) {
  struct KeywordMapper *f = (struct KeywordMapper*) filter;
  return f->in->get_filepath(f->in->arg);
}

bool KeywordMapper_p_is_eof(void *filter) {
  struct KeywordMapper *f = (struct KeywordMapper*) filter;
  return f->in->is_eof(f->in->arg);
}

void KeywordMapper_p_close(void *filter) {
  struct KeywordMapper *f = (struct KeywordMapper*) filter;
  return f->in->close(f->in->arg);
}

struct TokenStream KeywordMapper_to_TokenStream(struct KeywordMapper *f) {
  struct TokenStream stream;

  stream->arg          = f;
  stream->next_token   = &KeywordMapper_p_next_token;
  stream->get_lineno   = &KeywordMapper_p_get_lineno;
  stream->get_column   = &KeywordMapper_p_get_column;
  stream->get_filepath = &KeywordMapper_p_get_filepath;
  stream->is_eof       = &KeywordMapper_p_is_eof;
  stream->close        = &KeywordMapper_p_close;

  return stream;
}
