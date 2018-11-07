#include "tokeneater.c"
#include "test_harness.c"

#include "set.c"
#include "token.c"
#include "util.c"
#include "stringfunc.c"
#include "hash.c"
#include "linkedlist.c"

void tst_TokenEater_init() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &ts);

  test_assert_msg(te.in == &ts, "tst_TokenEater_init: in == &ts");
  test_assert_msg(te.types.data != NULL,
      "tst_TokenEater_init: types.data != NULL");

  TokenEater_del(&te);
  TS_close(&ts);
}

void tst_TokenEater_del() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &ts);

  TokenEater_del(&te);
  TS_close(&ts);

  test_assert_msg(te.in == NULL, "tst_TokenEater_del: in == NULL");
  test_assert_msg(te.types.data == NULL,
      "tst_TokenEater_del: types.data == NULL");
}

void tst_TokenEater_add() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &ts);

  TokenEater_add(&te, TT_COMMA);

  test_assert_msg(te.types.size == 1, "tst_TokenEater_add: types.size == 1");
  test_assert_msg(Set_contains(&(te.types), (void*) TT_COMMA),
      "tst_TokenEater_add: TT_COMMA in te.types");

  TokenEater_del(&te);
  TS_close(&ts);
}

void tst_TokenEater_next() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  struct Token tok;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &ts);

  tok.type = TT_WHITESPACE;
  tok.strval = "  \t";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_IDENTIFIER;
  tok.strval = "hello_there";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_WHITESPACE;
  tok.strval = " ";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_INT_LITERAL;
  tok.strval = "1234";
  tok.intval = 1234;
  TSQ_push(&tsq, tok);

  tok.type = TT_NEWLINE;
  tok.strval = "\n";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok = TokenEater_next(&te);

  test_assert_msg(tok.type == TT_WHITESPACE,
      "tst_TokenEater_next 01: type = TT_WHITESPACE");
  test_assert_msg(strequal(tok.strval, "  \t"),
      "tst_TokenEater_next 01: strval == '  \\t'");

  TokenEater_add(&te, TT_WHITESPACE);
  TokenEater_add(&te, TT_INT_LITERAL);

  tok = TokenEater_next(&te);

  test_assert_msg(tok.type == TT_IDENTIFIER,
      "tst_TokenEater_next 02: type == TT_IDENTIFIER");

  tok = TokenEater_next(&te);

  test_assert_msg(tok.type == TT_NEWLINE,
      "tst_TokenEater_next 03: type == TT_NEWLINE");

  tok = TokenEater_next(&te);

  test_assert_msg(tok.type == TT_EOF,
      "tst_TokenEater_next 04: type == TT_EOF");

  TokenEater_del(&te);
  TS_close(&ts);
}

void tst_TokenEater_to_TokenStream() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream in;
  struct TokenStream out;
  struct Token tok;
  TSQ_init(&tsq);
  in = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &in);
  out = TokenEater_to_TokenStream(&te);

  tsq.lineno = 123;
  tsq.column = 987;

  tok.type = TT_POUND;
  tok.strval = "#";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_IDENTIFIER;
  tok.strval = "ifndef";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  TokenEater_add(&te, TT_POUND);

  test_assert_msg(TS_get_lineno(&out) == 123,
      "tst_TokenEater_to_TokenStream: lineno == 123");
  test_assert_msg(TS_get_column(&out) == 987,
      "tst_TokenEater_to_TokenStream: column == 987");
  test_assert_msg(strequal(TS_get_filepath(&out), "<TokenStreamQueue>"),
      "tst_TokenEater_to_TokenStream: filepath == '<TokenStreamQueue>'");
  test_assert_msg(!TS_is_eof(&out),
      "tst_TokenEater_to_TokenStream: is_eof == false");

  tok = TS_next_token(&out);

  test_assert_msg(tok.type == TT_IDENTIFIER,
      "tst_TokenEater_to_TokenStream: tok.type == TT_IDENTIFIER");
  test_assert_msg(strequal(tok.strval, "ifndef"),
      "tst_TokenEater_to_TokenStream: tok.strval == 'ifndef'");

  tok = TS_next_token(&out);

  test_assert_msg(tok.type == TT_EOF,
      "tst_TokenEater_to_TokenStream after: tok.type == TT_EOF");
  test_assert_msg(TS_is_eof(&out),
      "tst_TokenEater_to_TokenStream after: is_eof == true");

  TS_close(&out);
  TokenEater_del(&te);

  test_assert_msg(tsq.lineno == 1,
      "tst_TokenEater_to_TokenStream close: lineno == 1");
  test_assert_msg(tsq.column == 0,
      "tst_TokenEater_to_TokenStream close: column == 0");
  test_assert_msg(tsq.q.size == 0,
      "tst_TokenEater_to_TokenStream close: q.size == 0");
}

int main(void) {
  tst_TokenEater_init();
  tst_TokenEater_del();
  tst_TokenEater_add();
  tst_TokenEater_next();
  tst_TokenEater_to_TokenStream();
  return testval;
}
