#include "scanner.c"
#include "test_harness.c"

#include "charstream.c"
#include "string.c"
#include "stringfunc.c"
#include "stringstream.c"
#include "token.c"
#include "util.c"

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

void tst_Scanner_next() {
  struct Scanner s;
  struct StringStream in;

  // TODO: test all sorts of tokens including erroneous tokens
  const char* input = "hello there my friend;";

  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);

  // TODO: finish testing this function.  It is a beast.
  // TODO: Maybe even split it up a bit.
  test_assert_msg(false, "tst_Scanner_next: unimplemented");

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
