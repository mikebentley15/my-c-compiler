#include <stdio.h>

int fputs_stub(const char* str, FILE *f) { return 1; }
int fputc_stub(int ch, FILE *f) { return ch; }

#define fputs fputs_stub
#define fputc fputc_stub

#include "scanner.c"

#include "charstream.c"
#include "string.c"
#include "stringfunc.c"
#include "stringstream.c"
#include "token.c"
#include "util.c"

#undef fputs
#undef fputc

#include "test_harness.c"

void tst_Scanner_init() {
  struct Scanner s;
  struct StringStream in;
  const char* input = "hello there my friend;";
  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);

  test_assert_msg(s.in == &cs, "tst_Scanner_init: in == &cs");
  test_assert_msg(!s.is_eof, "tst_Scanner_init: is_eof == false");
  test_assert_msg(s.next == 'h', "tst_Scanner_init: next == 'h'");
  test_assert_msg(s.tok_buf.size == 0, "tst_Scanner_init: tok_buf == ''");

  Scanner_close(&s);
  SS_close(&in);
}

void tst_Scanner_close() {
  struct Scanner s;
  struct StringStream in;
  const char* input = "hello there my friend;";
  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);

  Scanner_close(&s);
  SS_close(&in);

  test_assert_msg(s.in == NULL, "tst_Scanner_close: in == NULL");
  test_assert_msg(s.is_eof == true, "tst_Scanner_close: is_eof == true");
  test_assert_msg(s.next == '\0', "tst_Scanner_close: next == '\\0'");
  test_assert_msg(s.tok_buf.data == NULL,
      "tst_Scanner_close: tok_buf == NULL");
}

void checktok(struct Scanner *s, enum TokenType type, char* strval, int intval,
              char* msg)
{
  struct Token tok = Scanner_next(s);
  struct String s_msg;
  Str_init(&s_msg);

  Str_extend(&s_msg, msg);
  Str_extend(&s_msg, " type check");
  test_assert_msg(tok.type == type, s_msg.data);

  Str_clear(&s_msg);
  Str_extend(&s_msg, msg);
  Str_extend(&s_msg, " strval check - '");
  Str_extend(&s_msg, tok.strval);
  Str_append(&s_msg, '\'');
  test_assert_msg(strequal(tok.strval, strval), s_msg.data);

  Str_clear(&s_msg);
  Str_extend(&s_msg, msg);
  Str_extend(&s_msg, " intval check");
  test_assert_msg(tok.intval == intval, s_msg.data);
}

void tst_Scanner_next() {
  struct Scanner s;
  struct StringStream in;

  // TODO: test all sorts of tokens including erroneous tokens
  const char* input = "\t\n\"\\\\\"\\\n // comment\n /* another \n \t */\n#include+-*/%()--++ = == , != < > <= >= && || ! >> << ^ & | ~ . -> : ? += -= *= /= %= <<= >>= &= |= ^= 0 123480 '\\n' true false";

  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);

  checktok(&s, TT_WHITESPACE,            "\t",                  0, "tst_Scanner_next 01:");
  checktok(&s, TT_NEWLINE,               "\n",                  0, "tst_Scanner_next 02:");
  checktok(&s, TT_STRING_LITERAL,        "\\",                  0, "tst_Scanner_next 03:");
  checktok(&s, TT_LINE_CONTINUATION,     "\\\n",                0, "tst_Scanner_next 04:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 05:");
  checktok(&s, TT_LINE_COMMENT,          "// comment",          0, "tst_Scanner_next 06:");
  checktok(&s, TT_NEWLINE,               "\n",                  0, "tst_Scanner_next 07:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 08:");
  checktok(&s, TT_MULTILINE_COMMENT,     "/* another \n \t */", 0, "tst_Scanner_next 09:");
  checktok(&s, TT_NEWLINE,               "\n",                  0, "tst_Scanner_next 10:");
  checktok(&s, TT_POUND,                 "#",                   0, "tst_Scanner_next 11:");
  checktok(&s, TT_IDENTIFIER,            "include",             0, "tst_Scanner_next 12:");
  checktok(&s, TT_PLUS,                  "+",                   0, "tst_Scanner_next 13:");
  checktok(&s, TT_MINUS,                 "-",                   0, "tst_Scanner_next 14:");
  checktok(&s, TT_MULTIPLY,              "*",                   0, "tst_Scanner_next 15:");
  checktok(&s, TT_DIVIDE,                "/",                   0, "tst_Scanner_next 16:");
  checktok(&s, TT_MODULUS,               "%",                   0, "tst_Scanner_next 17:");
  checktok(&s, TT_LPAREN,                "(",                   0, "tst_Scanner_next 18:");
  checktok(&s, TT_RPAREN,                ")",                   0, "tst_Scanner_next 19:");
  checktok(&s, TT_DECREMENT,             "--",                  0, "tst_Scanner_next 20:");
  checktok(&s, TT_INCREMENT,             "++",                  0, "tst_Scanner_next 21:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 22:");
  checktok(&s, TT_EQUALS,                "=",                   0, "tst_Scanner_next 23:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 24:");
  checktok(&s, TT_EQUALITY,              "==",                  0, "tst_Scanner_next 25:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 26:");
  checktok(&s, TT_COMMA,                 ",",                   0, "tst_Scanner_next 27:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 28:");
  checktok(&s, TT_NOT_EQUAL,             "!=",                  0, "tst_Scanner_next 29:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 30:");
  checktok(&s, TT_LESS,                  "<",                   0, "tst_Scanner_next 31:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 32:");
  checktok(&s, TT_GREATER,               ">",                   0, "tst_Scanner_next 33:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 34:");
  checktok(&s, TT_LESS_EQUAL,            "<=",                  0, "tst_Scanner_next 35:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 36:");
  checktok(&s, TT_GREATER_EQUAL,         ">=",                  0, "tst_Scanner_next 37:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 38:");
  checktok(&s, TT_BOOLEAN_AND,           "&&",                  0, "tst_Scanner_next 39:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 40:");
  checktok(&s, TT_BOOLEAN_OR,            "||",                  0, "tst_Scanner_next 41:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 42:");
  checktok(&s, TT_BOOLEAN_NOT,           "!",                   0, "tst_Scanner_next 43:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 44:");
  checktok(&s, TT_BITSHIFT_RIGHT,        ">>",                  0, "tst_Scanner_next 45:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 46:");
  checktok(&s, TT_BITSHIFT_LEFT,         "<<",                  0, "tst_Scanner_next 47:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 48:");
  checktok(&s, TT_BITWISE_XOR,           "^",                   0, "tst_Scanner_next 49:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 50:");
  checktok(&s, TT_BITWISE_AND,           "&",                   0, "tst_Scanner_next 51:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 52:");
  checktok(&s, TT_BITWISE_OR,            "|",                   0, "tst_Scanner_next 53:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 54:");
  checktok(&s, TT_BITWISE_INVERT,        "~",                   0, "tst_Scanner_next 55:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 56:");
  checktok(&s, TT_DOT,                   ".",                   0, "tst_Scanner_next 57:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 58:");
  checktok(&s, TT_ARROW,                 "->",                  0, "tst_Scanner_next 59:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 60:");
  checktok(&s, TT_COLON,                 ":",                   0, "tst_Scanner_next 61:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 62:");
  checktok(&s, TT_QUESTION,              "?",                   0, "tst_Scanner_next 63:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 64:");
  checktok(&s, TT_PLUS_EQUALS,           "+=",                  0, "tst_Scanner_next 65:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 66:");
  checktok(&s, TT_MINUS_EQUALS,          "-=",                  0, "tst_Scanner_next 67:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 68:");
  checktok(&s, TT_MULTIPLY_EQUALS,       "*=",                  0, "tst_Scanner_next 69:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 70:");
  checktok(&s, TT_DIVIDE_EQUALS,         "/=",                  0, "tst_Scanner_next 71:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 72:");
  checktok(&s, TT_MODULUS_EQUALS,        "%=",                  0, "tst_Scanner_next 73:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 74:");
  checktok(&s, TT_BITSHIFT_LEFT_EQUALS,  "<<=",                 0, "tst_Scanner_next 75:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 76:");
  checktok(&s, TT_BITSHIFT_RIGHT_EQUALS, ">>=",                 0, "tst_Scanner_next 77:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 78:");
  checktok(&s, TT_BITWISE_AND_EQUALS,    "&=",                  0, "tst_Scanner_next 79:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 80:");
  checktok(&s, TT_BITWISE_OR_EQUALS,     "|=",                  0, "tst_Scanner_next 81:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 82:");
  checktok(&s, TT_BITWISE_XOR_EQUALS,    "^=",                  0, "tst_Scanner_next 83:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 84:");
  checktok(&s, TT_INT_LITERAL,           "0",                   0, "tst_Scanner_next 85:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 86:");
  checktok(&s, TT_INT_LITERAL,           "123480",         123480, "tst_Scanner_next 87:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 88:");
  checktok(&s, TT_CHARACTER_LITERAL,     "\n",          (int)'\n', "tst_Scanner_next 89:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 90:");
  checktok(&s, TT_IDENTIFIER,            "true",                0, "tst_Scanner_next 91:");
  checktok(&s, TT_WHITESPACE,            " ",                   0, "tst_Scanner_next 92:");
  checktok(&s, TT_IDENTIFIER,            "false",               0, "tst_Scanner_next 93:");
  checktok(&s, TT_EOF,                   "",                    0, "tst_Scanner_next 94:");

  Scanner_close(&s);
  SS_close(&in);
}

void tst_Scanner_to_TokenStream() {
  struct Scanner s;
  struct StringStream in;
  const char* input = "hello   there my friend;";
  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);

  struct TokenStream ts = Scanner_to_TokenStream(&s);
  struct Token tok;

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_IDENTIFIER,
      "tst_Scanner_to_TokenStream 01: type == IDENTIFIER");
  test_assert_msg(strequal(tok.strval, "hello"),
      "tst_Scanner_to_TokenStream 01: strval == 'hello'");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_WHITESPACE,
      "tst_Scanner_to_TokenStream 02: type == WHITESPACE");
  test_assert_msg(strequal(tok.strval, "   "),
      "tst_Scanner_to_TokenStream 02: strval == '   '");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_IDENTIFIER,
      "tst_Scanner_to_TokenStream 03: type == IDENTIFIER");
  test_assert_msg(strequal(tok.strval, "there"),
      "tst_Scanner_to_TokenStream 03: strval == 'there'");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_WHITESPACE,
      "tst_Scanner_to_TokenStream 04: type == WHITESPACE");
  test_assert_msg(strequal(tok.strval, " "),
      "tst_Scanner_to_TokenStream 04: strval == ' '");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_IDENTIFIER,
      "tst_Scanner_to_TokenStream 05: type == IDENTIFIER");
  test_assert_msg(strequal(tok.strval, "my"),
      "tst_Scanner_to_TokenStream 05: strval == 'my'");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_WHITESPACE,
      "tst_Scanner_to_TokenStream 06: type == WHITESPACE");
  test_assert_msg(strequal(tok.strval, " "),
      "tst_Scanner_to_TokenStream 06: strval == ' '");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_IDENTIFIER,
      "tst_Scanner_to_TokenStream 07: type == IDENTIFIER");
  test_assert_msg(strequal(tok.strval, "friend"),
      "tst_Scanner_to_TokenStream 07: strval == 'friend'");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_SEMICOLON,
      "tst_Scanner_to_TokenStream 08: type == SEMICOLON");
  test_assert_msg(strequal(tok.strval, ";"),
      "tst_Scanner_to_TokenStream 08: strval == ';'");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_EOF,
      "tst_Scanner_to_TokenStream 09: type == EOF");
  test_assert_msg(strequal(tok.strval, ""),
      "tst_Scanner_to_TokenStream 09: strval == ''");

  Scanner_close(&s);
  SS_close(&in);
}

int main(void) {
  tst_Scanner_init();
  tst_Scanner_close();
  tst_Scanner_next();
  tst_Scanner_to_TokenStream();
  return testval;
}
