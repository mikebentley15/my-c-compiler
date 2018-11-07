#include <stdio.h> // for the real fputs()

int fputs_stub(const char *str, FILE *f) { return 0; }

#define fputs fputs_stub
#include "keywordmapper.c"
#undef fputs

#include "test_harness.c"

#include "dict.c"
#include "hash.c"
#include "linkedlist.c"
#include "string.c"
#include "stringfunc.c"
#include "token.c"
#include "util.c"

void tst_KeywordMapper_init() {
  struct KeywordMapper km;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  KeywordMapper_init(&km, &ts);

  test_assert_msg(km.in == &ts, "tst_KeywordMapper_init: in == &ts");
  test_assert_msg(km.map != NULL, "tst_KeywordMapper_init: map != NULL");
  test_assert_msg(km.pp_map != NULL, "tst_KeywordMapper_init: pp_map != NULL");

  KeywordMapper_del(&km);
  TS_close(&ts);
}

void tst_KeywordMapper_del() {
  struct KeywordMapper km;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  KeywordMapper_init(&km, &ts);

  KeywordMapper_del(&km);
  TS_close(&ts);

  test_assert_msg(km.in == NULL, "tst_KeywordMapper_del: in == NULL");
  test_assert_msg(km.map == NULL, "tst_KeywordMapper_del: map == NULL");
  test_assert_msg(km.pp_map == NULL, "tst_KeywordMapper_del: pp_map == NULL");
}

void chk_map(
    struct TokenStreamQueue *tsq,
    struct KeywordMapper *km,
    enum TokenType type,
    const char* strval,
    int intval,
    enum TokenType final_type,
    const char* msg)
{
  struct Token tok;
  struct String str;
  Str_init(&str);

  tok.type = type;
  tok.strval = strval;
  tok.intval = intval;
  TSQ_push(tsq, tok);

  tok = KeywordMapper_next(km);

  Str_clear(&str);
  Str_extend(&str, msg);
  Str_extend(&str, " (type)");
  test_assert_msg(tok.type == final_type, str.data);

  Str_clear(&str);
  Str_extend(&str, msg);
  Str_extend(&str, " (strval)");
  test_assert_msg(strequal(tok.strval, strval), str.data);

  Str_clear(&str);
  Str_extend(&str, msg);
  Str_extend(&str, " (intval)");
  test_assert_msg(tok.intval == intval, str.data);
}

void chk_nonmap(
    struct TokenStreamQueue *tsq,
    struct KeywordMapper *km,
    enum TokenType type,
    const char* msg)
{
  struct String str;
  Str_init(&str);
  Str_extend(&str, "tst_KeywordMapper_next ");
  Str_extend(&str, msg);
  Str_append(&str, ':');
  chk_map(tsq, km, type, "", 0, type, str.data);
}

void chk_ident(
    struct TokenStreamQueue *tsq,
    struct KeywordMapper *km,
    const char* ident,
    enum TokenType type)
{
  struct String str;
  Str_init(&str);
  Str_extend(&str, "tst_KeywordMapper_next ");
  Str_extend(&str, ident);
  Str_extend(&str, ":");
  chk_map(tsq, km, TT_IDENTIFIER, ident, 0, type, str.data);
}

void chk_pp( // preprocessor identifiers
    struct TokenStreamQueue *tsq,
    struct KeywordMapper *km,
    const char* ident,
    enum TokenType type)
{
  // line continuations and whitespace need to be removed first
  // before the KeywordMapper can be expected to identify correctly
  struct Token pound;
  pound.type = TT_POUND;
  pound.strval = "#";
  pound.intval = 0;
  TSQ_push(tsq, pound);

  struct String str;
  Str_init(&str);
  Str_extend(&str, "tst_KeywordMapper_next #");
  Str_extend(&str, ident);
  Str_extend(&str, ":");
  chk_map(tsq, km, TT_IDENTIFIER, ident, 0, type, str.data);
}


void tst_KeywordMapper_next() {
  struct KeywordMapper km;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  struct Token tok;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  KeywordMapper_init(&km, &ts);

  chk_nonmap(&tsq, &km, TT_ERROR                , "TT_ERROR:");
  chk_nonmap(&tsq, &km, TT_BOOL                 , "TT_BOOL:");
  chk_nonmap(&tsq, &km, TT_CHAR                 , "TT_CHAR:");
  chk_nonmap(&tsq, &km, TT_INT                  , "TT_INT:");
  chk_nonmap(&tsq, &km, TT_VOID                 , "TT_VOID:");
  chk_nonmap(&tsq, &km, TT_ENUM                 , "TT_ENUM:");
  chk_nonmap(&tsq, &km, TT_STRUCT               , "TT_STRUCT:");
  chk_nonmap(&tsq, &km, TT_BREAK                , "TT_BREAK:");
  chk_nonmap(&tsq, &km, TT_CONST                , "TT_CONST:");
  chk_nonmap(&tsq, &km, TT_CONTINUE             , "TT_CONTINUE:");
  chk_nonmap(&tsq, &km, TT_ELSE                 , "TT_ELSE:");
  chk_nonmap(&tsq, &km, TT_IF                   , "TT_IF:");
  chk_nonmap(&tsq, &km, TT_RETURN               , "TT_RETURN:");
  chk_nonmap(&tsq, &km, TT_SIZEOF               , "TT_SIZEOF:");
  chk_nonmap(&tsq, &km, TT_TYPEDEF              , "TT_TYPEDEF:");
  chk_nonmap(&tsq, &km, TT_WHILE                , "TT_WHILE:");
  chk_nonmap(&tsq, &km, TT_PACKED               , "TT_PACKED:");
  chk_nonmap(&tsq, &km, TT_AND                  , "TT_AND:");
  chk_nonmap(&tsq, &km, TT_AUTO                 , "TT_AUTO:");
  chk_nonmap(&tsq, &km, TT_BYTE                 , "TT_BYTE:");
  chk_nonmap(&tsq, &km, TT_CASE                 , "TT_CASE:");
  chk_nonmap(&tsq, &km, TT_DEFAULT              , "TT_DEFAULT:");
  chk_nonmap(&tsq, &km, TT_DO                   , "TT_DO:");
  chk_nonmap(&tsq, &km, TT_DOUBLE               , "TT_DOUBLE:");
  chk_nonmap(&tsq, &km, TT_EXTERN               , "TT_EXTERN:");
  chk_nonmap(&tsq, &km, TT_FLOAT                , "TT_FLOAT:");
  chk_nonmap(&tsq, &km, TT_FOR                  , "TT_FOR:");
  chk_nonmap(&tsq, &km, TT_GOTO                 , "TT_GOTO:");
  chk_nonmap(&tsq, &km, TT_LONG                 , "TT_LONG:");
  chk_nonmap(&tsq, &km, TT_OR                   , "TT_OR:");
  chk_nonmap(&tsq, &km, TT_REGISTER             , "TT_REGISTER:");
  chk_nonmap(&tsq, &km, TT_SHORT                , "TT_SHORT:");
  chk_nonmap(&tsq, &km, TT_SIGNED               , "TT_SIGNED:");
  chk_nonmap(&tsq, &km, TT_STATIC               , "TT_STATIC:");
  chk_nonmap(&tsq, &km, TT_SWITCH               , "TT_SWITCH:");
  chk_nonmap(&tsq, &km, TT_UNION                , "TT_UNION:");
  chk_nonmap(&tsq, &km, TT_UNSIGNED             , "TT_UNSIGNED:");
  chk_nonmap(&tsq, &km, TT_VOLATILE             , "TT_VOLATILE:");
  chk_nonmap(&tsq, &km, TT_PP_INCLUDE           , "TT_PP_INCLUDE:");
  chk_nonmap(&tsq, &km, TT_PP_IFDEF             , "TT_PP_IFDEF:");
  chk_nonmap(&tsq, &km, TT_PP_IFNDEF            , "TT_PP_IFNDEF:");
  chk_nonmap(&tsq, &km, TT_PP_ELSE              , "TT_PP_ELSE:");
  chk_nonmap(&tsq, &km, TT_PP_ENDIF             , "TT_PP_ENDIF:");
  chk_nonmap(&tsq, &km, TT_PP_DEFINE            , "TT_PP_DEFINE:");
  chk_nonmap(&tsq, &km, TT_PP_PRAGMA            , "TT_PP_PRAGMA:");
  chk_nonmap(&tsq, &km, TT_PP_UNDEF             , "TT_PP_UNDEF:");
  chk_nonmap(&tsq, &km, TT_PLUS                 , "TT_PLUS:");
  chk_nonmap(&tsq, &km, TT_MINUS                , "TT_MINUS:");
  chk_nonmap(&tsq, &km, TT_MULTIPLY             , "TT_MULTIPLY:");
  chk_nonmap(&tsq, &km, TT_DEREFERENCE          , "TT_DEREFERENCE:");
  chk_nonmap(&tsq, &km, TT_DIVIDE               , "TT_DIVIDE:");
  chk_nonmap(&tsq, &km, TT_MODULUS              , "TT_MODULUS:");
  chk_nonmap(&tsq, &km, TT_LPAREN               , "TT_LPAREN:");
  chk_nonmap(&tsq, &km, TT_RPAREN               , "TT_RPAREN:");
  chk_nonmap(&tsq, &km, TT_DECREMENT            , "TT_DECREMENT:");
  chk_nonmap(&tsq, &km, TT_INCREMENT            , "TT_INCREMENT:");
  chk_nonmap(&tsq, &km, TT_EQUALS               , "TT_EQUALS:");
  chk_nonmap(&tsq, &km, TT_EQUALITY             , "TT_EQUALITY:");
  chk_nonmap(&tsq, &km, TT_NOT_EQUAL            , "TT_NOT_EQUAL:");
  chk_nonmap(&tsq, &km, TT_LESS                 , "TT_LESS:");
  chk_nonmap(&tsq, &km, TT_GREATER              , "TT_GREATER:");
  chk_nonmap(&tsq, &km, TT_LESS_EQUAL           , "TT_LESS_EQUAL:");
  chk_nonmap(&tsq, &km, TT_GREATER_EQUAL        , "TT_GREATER_EQUAL:");
  chk_nonmap(&tsq, &km, TT_BOOLEAN_AND          , "TT_BOOLEAN_AND:");
  chk_nonmap(&tsq, &km, TT_BOOLEAN_OR           , "TT_BOOLEAN_OR:");
  chk_nonmap(&tsq, &km, TT_BOOLEAN_NOT          , "TT_BOOLEAN_NOT:");
  chk_nonmap(&tsq, &km, TT_BITSHIFT_RIGHT       , "TT_BITSHIFT_RIGHT:");
  chk_nonmap(&tsq, &km, TT_BITSHIFT_LEFT        , "TT_BITSHIFT_LEFT:");
  chk_nonmap(&tsq, &km, TT_BITWISE_XOR          , "TT_BITWISE_XOR:");
  chk_nonmap(&tsq, &km, TT_BITWISE_AND          , "TT_BITWISE_AND:");
  chk_nonmap(&tsq, &km, TT_ADDRESS_OF           , "TT_ADDRESS_OF:");
  chk_nonmap(&tsq, &km, TT_BITWISE_OR           , "TT_BITWISE_OR:");
  chk_nonmap(&tsq, &km, TT_BITWISE_INVERT       , "TT_BITWISE_INVERT:");
  chk_nonmap(&tsq, &km, TT_DOT                  , "TT_DOT:");
  chk_nonmap(&tsq, &km, TT_ARROW                , "TT_ARROW:");
  chk_nonmap(&tsq, &km, TT_COLON                , "TT_COLON:");
  chk_nonmap(&tsq, &km, TT_QUESTION             , "TT_QUESTION:");
  chk_nonmap(&tsq, &km, TT_PLUS_EQUALS          , "TT_PLUS_EQUALS:");
  chk_nonmap(&tsq, &km, TT_MINUS_EQUALS         , "TT_MINUS_EQUALS:");
  chk_nonmap(&tsq, &km, TT_MULTIPLY_EQUALS      , "TT_MULTIPLY_EQUALS:");
  chk_nonmap(&tsq, &km, TT_DIVIDE_EQUALS        , "TT_DIVIDE_EQUALS:");
  chk_nonmap(&tsq, &km, TT_MODULUS_EQUALS       , "TT_MODULUS_EQUALS:");
  chk_nonmap(&tsq, &km, TT_BITSHIFT_LEFT_EQUALS , "TT_BITSHIFT_LEFT_EQUALS:");
  chk_nonmap(&tsq, &km, TT_BITSHIFT_RIGHT_EQUALS, "TT_BITSHIFT_RIGHT_EQUALS:");
  chk_nonmap(&tsq, &km, TT_BITWISE_AND_EQUALS   , "TT_BITWISE_AND_EQUALS:");
  chk_nonmap(&tsq, &km, TT_BITWISE_OR_EQUALS    , "TT_BITWISE_OR_EQUALS:");
  chk_nonmap(&tsq, &km, TT_BITWISE_XOR_EQUALS   , "TT_BITWISE_XOR_EQUALS:");
  chk_nonmap(&tsq, &km, TT_INT_LITERAL          , "TT_INT_LITERAL:");
  chk_nonmap(&tsq, &km, TT_STRING_LITERAL       , "TT_STRING_LITERAL:");
  chk_nonmap(&tsq, &km, TT_CHARACTER_LITERAL    , "TT_CHARACTER_LITERAL:");
  chk_nonmap(&tsq, &km, TT_TRUE_LITERAL         , "TT_TRUE_LITERAL:");
  chk_nonmap(&tsq, &km, TT_FALSE_LITERAL        , "TT_FALSE_LITERAL:");
  chk_nonmap(&tsq, &km, TT_LINE_COMMENT         , "TT_LINE_COMMENT:");
  chk_nonmap(&tsq, &km, TT_MULTILINE_COMMENT    , "TT_MULTILINE_COMMENT:");
  chk_nonmap(&tsq, &km, TT_WHITESPACE           , "TT_WHITESPACE:");
  chk_nonmap(&tsq, &km, TT_NEWLINE              , "TT_NEWLINE:");
  chk_nonmap(&tsq, &km, TT_EOF                  , "TT_EOF:");
  chk_nonmap(&tsq, &km, TT_LINE_CONTINUATION    , "TT_LINE_CONTINUATION:");
  chk_nonmap(&tsq, &km, TT_SEMICOLON            , "TT_SEMICOLON:");
  chk_nonmap(&tsq, &km, TT_COMMA                , "TT_COMMA:");

  chk_ident(&tsq, &km, "bool",     TT_BOOL         );
  chk_ident(&tsq, &km, "char",     TT_CHAR         );
  chk_ident(&tsq, &km, "int",      TT_INT          );
  chk_ident(&tsq, &km, "void",     TT_VOID         );
  chk_ident(&tsq, &km, "enum",     TT_ENUM         );
  chk_ident(&tsq, &km, "struct",   TT_STRUCT       );
  chk_ident(&tsq, &km, "break",    TT_BREAK        );
  chk_ident(&tsq, &km, "const",    TT_CONST        );
  chk_ident(&tsq, &km, "continue", TT_CONTINUE     );
  chk_ident(&tsq, &km, "else",     TT_ELSE         );
  chk_ident(&tsq, &km, "if",       TT_IF           );
  chk_ident(&tsq, &km, "return",   TT_RETURN       );
  chk_ident(&tsq, &km, "sizeof",   TT_SIZEOF       );
  chk_ident(&tsq, &km, "typedef",  TT_TYPEDEF      );
  chk_ident(&tsq, &km, "while",    TT_WHILE        );
  chk_ident(&tsq, &km, "_Packed",  TT_PACKED       );
  chk_ident(&tsq, &km, "and",      TT_AND          );
  chk_ident(&tsq, &km, "auto",     TT_AUTO         );
  chk_ident(&tsq, &km, "byte",     TT_BYTE         );
  chk_ident(&tsq, &km, "case",     TT_CASE         );
  chk_ident(&tsq, &km, "default",  TT_DEFAULT      );
  chk_ident(&tsq, &km, "do",       TT_DO           );
  chk_ident(&tsq, &km, "double",   TT_DOUBLE       );
  chk_ident(&tsq, &km, "extern",   TT_EXTERN       );
  chk_ident(&tsq, &km, "float",    TT_FLOAT        );
  chk_ident(&tsq, &km, "for",      TT_FOR          );
  chk_ident(&tsq, &km, "goto",     TT_GOTO         );
  chk_ident(&tsq, &km, "long",     TT_LONG         );
  chk_ident(&tsq, &km, "or",       TT_OR           );
  chk_ident(&tsq, &km, "register", TT_REGISTER     );
  chk_ident(&tsq, &km, "short",    TT_SHORT        );
  chk_ident(&tsq, &km, "signed",   TT_SIGNED       );
  chk_ident(&tsq, &km, "static",   TT_STATIC       );
  chk_ident(&tsq, &km, "switch",   TT_SWITCH       );
  chk_ident(&tsq, &km, "union",    TT_UNION        );
  chk_ident(&tsq, &km, "unsigned", TT_UNSIGNED     );
  chk_ident(&tsq, &km, "volatile", TT_VOLATILE     );
  chk_ident(&tsq, &km, "true",     TT_TRUE_LITERAL );
  chk_ident(&tsq, &km, "false",    TT_FALSE_LITERAL);
  chk_ident(&tsq, &km, "other_23", TT_IDENTIFIER   );
  chk_ident(&tsq, &km, "__abcd9A", TT_IDENTIFIER   );

  chk_pp(&tsq, &km, "include", TT_PP_INCLUDE);
  chk_pp(&tsq, &km, "ifdef",   TT_PP_IFDEF);
  chk_pp(&tsq, &km, "ifndef",  TT_PP_IFNDEF);
  chk_pp(&tsq, &km, "else",    TT_PP_ELSE);
  chk_pp(&tsq, &km, "endif",   TT_PP_ENDIF);
  chk_pp(&tsq, &km, "define",  TT_PP_DEFINE);
  chk_pp(&tsq, &km, "pragma",  TT_PP_PRAGMA);
  chk_pp(&tsq, &km, "undef",   TT_PP_UNDEF);

  KeywordMapper_del(&km);
  TS_close(&ts);
}

void tst_KeywordMapper_to_TokenStream() {
  struct KeywordMapper km;
  struct TokenStreamQueue tsq;
  struct TokenStream in;
  struct TokenStream out;
  struct Token tok;
  TSQ_init(&tsq);
  in = TSQ_to_TS(&tsq);
  KeywordMapper_init(&km, &in);
  out = KeywordMapper_to_TokenStream(&km);

  tsq.lineno = 42;
  tsq.column = 84;

  test_assert_msg(TS_get_lineno(&out) == 42,
      "tst_KeywordMapper_to_TokenStream 01: lineno == 42");
  test_assert_msg(TS_get_column(&out) == 84,
      "tst_KeywordMapper_to_TokenStream 01: column == 84");
  test_assert_msg(strequal(TS_get_filepath(&out), "<TokenStreamQueue>"),
      "tst_KeywordMapper_to_TokenStream 01: filepath == <TokenStreamQueue>");
  test_assert_msg(TS_is_eof(&out),
      "tst_KeywordMapper_to_TokenStream 01: is_eof == true");

  tok.type = TT_IDENTIFIER;
  tok.strval = "continue";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_COMMA;
  tok.strval = ",";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_SEMICOLON;
  tok.strval = ";";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  test_assert_msg(!TS_is_eof(&out),
      "tst_KeywordMapper_to_TokenStream 02: is_eof == false");

  tok = TS_next_token(&out);

  test_assert_msg(tok.type == TT_CONTINUE,
      "tst_KeywordMapper_to_TokenStream 03: type == TT_CONTINUE");
  test_assert_msg(strequal(tok.strval, "continue"),
      "tst_KeywordMapper_to_TokenStream 03: strval == 'continue'");
  test_assert_msg(tok.intval == 0,
      "tst_KeywordMapper_to_TokenStream 03: intval == 0");

  tok = TS_next_token(&out);

  test_assert_msg(tok.type == TT_COMMA,
      "tst_KeywordMapper_to_TokenStream 04: type == TT_COMMA");
  test_assert_msg(strequal(tok.strval, ","),
      "tst_KeywordMapper_to_TokenStream 04: strval == ','");
  test_assert_msg(tok.intval == 0,
      "tst_KeywordMapper_to_TokenStream 04: intval == 0");

  TS_close(&out);
  KeywordMapper_del(&km);

  test_assert_msg(tsq.q.size == 0,
      "tst_KeywordMapper_to_TokenStream close: q.size == 0");
  test_assert_msg(tsq.lineno == 1,
      "tst_KeywordMapper_to_TokenStream close: lineno == 1");
  test_assert_msg(tsq.column == 0,
      "tst_KeywordMapper_to_TokenStream close: column == 0");
}

int main(void) {
  tst_KeywordMapper_init();
  tst_KeywordMapper_del();
  tst_KeywordMapper_next();
  tst_KeywordMapper_to_TokenStream();
  return testval;
}
