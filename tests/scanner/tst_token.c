#include "token.c"
#include "test_harness.c"

#include "linkedlist.c"
#include "stringfunc.c"

struct TS_Mock {
  int next_token_calls;
  int get_lineno_calls;
  int get_column_calls;
  int get_filepath_calls;
  int is_eof_calls;
  int close_calls;
};

struct Token TS_next_token_stub(void* arg) {
  struct TS_Mock *m = arg;
  struct Token tok;
  tok.type = TT_IDENTIFIER;
  tok.strval = "hello there";
  tok.intval = 1234;
  m->next_token_calls++;
  return tok;
}

int TS_get_lineno_stub(void* arg) {
  struct TS_Mock *m = arg;
  m->get_lineno_calls++;
  return m->get_lineno_calls;
}

int TS_get_column_stub(void* arg) {
  struct TS_Mock *m = arg;
  m->get_column_calls++;
  return m->get_column_calls;
}

const char* TS_get_filepath_stub(void* arg) {
  struct TS_Mock *m = arg;
  m->get_filepath_calls++;
  return "<TS_get_filepath_stub>";
}

bool TS_is_eof_stub(void* arg) {
  struct TS_Mock *m = arg;
  m->is_eof_calls++;
  return false;
}

void TS_close_stub(void* arg) {
  struct TS_Mock *m = arg;
  m->close_calls++;
}

struct TokenStream init_TS_Mock(struct TS_Mock *m) {
  struct TokenStream ts;

  m->next_token_calls = 0;
  m->get_lineno_calls = 0;
  m->get_column_calls = 0;
  m->get_filepath_calls = 0;
  m->is_eof_calls = 0;
  m->close_calls = 0;

  ts.arg = m;
  ts.next_token = &TS_next_token_stub;
  ts.get_lineno = &TS_get_lineno_stub;
  ts.get_column = &TS_get_column_stub;
  ts.get_filepath = &TS_get_filepath_stub;
  ts.is_eof = &TS_is_eof_stub;
  ts.close = &TS_close_stub;

  return ts;
}

void tst_TS_next_token() {
  struct TS_Mock m;
  struct TokenStream ts = init_TS_Mock(&m);

  struct Token tok = TS_next_token(&ts);

  test_assert_msg(tok.type == TT_IDENTIFIER,
      "tst_TS_next_token: type == TT_IDENTIFIER");
  test_assert_msg(strequal(tok.strval, "hello there"),
      "tst_TS_next_token: strval == 'hello there'");
  test_assert_msg(tok.intval == 1234, "tst_TS_next_token: intval == 1234");

  test_assert_msg(m.next_token_calls == 1,
      "tst_TS_next_token: next_token_calls == 1");
  test_assert_msg(m.get_lineno_calls == 0,
      "tst_TS_next_token: get_lineno_calls == 0");
  test_assert_msg(m.get_column_calls == 0,
      "tst_TS_next_token: get_column_calls == 0");
  test_assert_msg(m.get_filepath_calls == 0,
      "tst_TS_next_token: get_filepath_calls == 0");
  test_assert_msg(m.is_eof_calls == 0,
      "tst_TS_next_token: is_eof_calls == 0");
  test_assert_msg(m.close_calls == 0,
      "tst_TS_next_token: close_calls == 0");
}

void tst_TS_get_lineno() {
  struct TS_Mock m;
  struct TokenStream ts = init_TS_Mock(&m);
  int lineno;

  lineno = TS_get_lineno(&ts);
  test_assert_msg(lineno == 1, "tst_TS_get_lineno: lineno == 1");
  lineno = TS_get_lineno(&ts);
  test_assert_msg(lineno == 2, "tst_TS_get_lineno: lineno == 2");

  test_assert_msg(m.next_token_calls == 0,
      "tst_TS_get_lineno: next_token_calls == 0");
  test_assert_msg(m.get_lineno_calls == 2,
      "tst_TS_get_lineno: get_lineno_calls == 2");
  test_assert_msg(m.get_column_calls == 0,
      "tst_TS_get_lineno: get_column_calls == 0");
  test_assert_msg(m.get_filepath_calls == 0,
      "tst_TS_get_lineno: get_filepath_calls == 0");
  test_assert_msg(m.is_eof_calls == 0,
      "tst_TS_get_lineno: is_eof_calls == 0");
  test_assert_msg(m.close_calls == 0,
      "tst_TS_get_lineno: close_calls == 0");
}

void tst_TS_get_column() {
  struct TS_Mock m;
  struct TokenStream ts = init_TS_Mock(&m);
  int column;

  column = TS_get_column(&ts);
  test_assert_msg(column == 1, "tst_TS_get_column: column == 1");
  column = TS_get_column(&ts);
  test_assert_msg(column == 2, "tst_TS_get_column: column == 2");

  test_assert_msg(m.next_token_calls == 0,
      "tst_TS_get_column: next_token_calls == 0");
  test_assert_msg(m.get_lineno_calls == 0,
      "tst_TS_get_column: get_lineno_calls == 0");
  test_assert_msg(m.get_column_calls == 2,
      "tst_TS_get_column: get_column_calls == 2");
  test_assert_msg(m.get_filepath_calls == 0,
      "tst_TS_get_column: get_filepath_calls == 0");
  test_assert_msg(m.is_eof_calls == 0,
      "tst_TS_get_column: is_eof_calls == 0");
  test_assert_msg(m.close_calls == 0,
      "tst_TS_get_column: close_calls == 0");
}

void tst_TS_get_filepath() {
  struct TS_Mock m;
  struct TokenStream ts = init_TS_Mock(&m);
  const char* filepath = TS_get_filepath(&ts);

  test_assert_msg(strequal(filepath, "<TS_get_filepath_stub>"),
      "tst_TS_get_filepath: filepath == '<TS_get_filepath_stub>'");

  test_assert_msg(m.next_token_calls == 0,
      "tst_TS_get_filepath: next_token_calls == 0");
  test_assert_msg(m.get_lineno_calls == 0,
      "tst_TS_get_filepath: get_lineno_calls == 0");
  test_assert_msg(m.get_column_calls == 0,
      "tst_TS_get_filepath: get_column_calls == 0");
  test_assert_msg(m.get_filepath_calls == 1,
      "tst_TS_get_filepath: get_filepath_calls == 1");
  test_assert_msg(m.is_eof_calls == 0,
      "tst_TS_get_filepath: is_eof_calls == 0");
  test_assert_msg(m.close_calls == 0,
      "tst_TS_get_filepath: close_calls == 0");
}

void tst_TS_is_eof() {
  struct TS_Mock m;
  struct TokenStream ts = init_TS_Mock(&m);
  bool is_eof = TS_is_eof(&ts);

  test_assert_msg(!is_eof, "tst_TS_is_eof: is_eof == false");

  test_assert_msg(m.next_token_calls == 0,
      "tst_TS_is_eof: next_token_calls == 0");
  test_assert_msg(m.get_lineno_calls == 0,
      "tst_TS_is_eof: get_lineno_calls == 0");
  test_assert_msg(m.get_column_calls == 0,
      "tst_TS_is_eof: get_column_calls == 0");
  test_assert_msg(m.get_filepath_calls == 0,
      "tst_TS_is_eof: get_filepath_calls == 0");
  test_assert_msg(m.is_eof_calls == 1,
      "tst_TS_is_eof: is_eof_calls == 1");
  test_assert_msg(m.close_calls == 0,
      "tst_TS_is_eof: close_calls == 0");
}

void tst_TS_close() {
  struct TS_Mock m;
  struct TokenStream ts = init_TS_Mock(&m);

  TS_close(&ts);

  test_assert_msg(m.next_token_calls == 0,
      "tst_TS_close: next_token_calls == 0");
  test_assert_msg(m.get_lineno_calls == 0,
      "tst_TS_close: get_lineno_calls == 0");
  test_assert_msg(m.get_column_calls == 0,
      "tst_TS_close: get_column_calls == 0");
  test_assert_msg(m.get_filepath_calls == 0,
      "tst_TS_close: get_filepath_calls == 0");
  test_assert_msg(m.is_eof_calls == 0,
      "tst_TS_close: is_eof_calls == 0");
  test_assert_msg(m.close_calls == 1,
      "tst_TS_close: close_calls == 1");
}

void tst_TSQ_init() {
  struct TokenStreamQueue tsq;
  TSQ_init(&tsq);

  test_assert_msg(tsq.q.size == 0, "tst_TSQ_init: q.size == 0");
  test_assert_msg(tsq.lineno == 1, "tst_TSQ_init: lineno == 1");
  test_assert_msg(tsq.column == 0, "tst_TSQ_init: column == 0");

  TSQ_close(&tsq);
}

void tst_TSQ_close() {
  struct TokenStreamQueue tsq;
  struct Token tok;
  TSQ_init(&tsq);

  TSQ_push(&tsq, tok);
  TSQ_push(&tsq, tok);
  TSQ_push(&tsq, tok);
  TSQ_push(&tsq, tok);
  tsq.lineno = 123;
  tsq.column = 456;

  TSQ_close(&tsq);

  test_assert_msg(tsq.q.size == 0, "tst_TSQ_close: q.size == 0");
  test_assert_msg(tsq.lineno == 1, "tst_TSQ_close: lineno == 1");
  test_assert_msg(tsq.column == 0, "tst_TSQ_close: column == 0");
}

void tst_TSQ_clear() {
  struct TokenStreamQueue tsq;
  struct Token tok;
  TSQ_init(&tsq);

  TSQ_push(&tsq, tok);
  TSQ_push(&tsq, tok);
  TSQ_push(&tsq, tok);
  TSQ_push(&tsq, tok);
  tsq.lineno = 123;
  tsq.column = 456;

  TSQ_clear(&tsq);

  test_assert_msg(tsq.q.size == 0, "tst_TSQ_clear: q.size == 0");
  test_assert_msg(tsq.lineno == 123, "tst_TSQ_clear: lineno == 123");
  test_assert_msg(tsq.column == 456, "tst_TSQ_clear: column == 456");

  TSQ_close(&tsq);
}

void tst_TSQ_push() {
  struct TokenStreamQueue tsq;
  struct Token tok;
  struct Token tok1;
  struct Token tok2;
  struct Token tok3;
  struct Token tokeof;
  TSQ_init(&tsq);

  tok1.type = TT_ERROR;
  tok2.type = TT_COMMA;
  tok3.type = TT_INCREMENT;
  tokeof.type = TT_EOF;

  tok1.strval = "error";
  tok2.strval = ",";
  tok3.strval = "++";
  tokeof.strval = "";

  tok1.intval = 1;
  tok2.intval = 2;
  tok3.intval = 3;
  tokeof.intval = 0;

  TSQ_push(&tsq, tok1);
  TSQ_push(&tsq, tok2);
  TSQ_push(&tsq, tok1);
  TSQ_push(&tsq, tok3);

  test_assert_msg(tsq.q.size == 4, "tst_TSQ_push: q.size == 4");

  tok = TSQ_pop(&tsq);
  test_assert_msg(tsq.q.size == 3, "tst_TSQ_push 01: q.size == 3");
  test_assert_msg(tok.type == tok1.type,
      "tst_TSQ_push 01: tok == tok1 (type)");
  test_assert_msg(strequal(tok.strval, tok1.strval),
      "tst_TSQ_push 01: tok == tok1 (strval)");
  test_assert_msg(tok.intval == tok1.intval,
      "tst_TSQ_push 01: tok == tok1 (intval)");

  tok = TSQ_pop(&tsq);
  test_assert_msg(tsq.q.size == 2, "tst_TSQ_push 02: q.size == 2");
  test_assert_msg(tok.type == tok2.type,
      "tst_TSQ_push 02: tok == tok2 (type)");
  test_assert_msg(strequal(tok.strval, tok2.strval),
      "tst_TSQ_push 02: tok == tok2 (strval)");
  test_assert_msg(tok.intval == tok2.intval,
      "tst_TSQ_push 02: tok == tok2 (intval)");

  tok = TSQ_pop(&tsq);
  test_assert_msg(tsq.q.size == 1, "tst_TSQ_push 03: q.size == 1");
  test_assert_msg(tok.type == tok1.type,
      "tst_TSQ_push 03: tok == tok1 (type)");
  test_assert_msg(strequal(tok.strval, tok1.strval),
      "tst_TSQ_push 03: tok == tok1 (strval)");
  test_assert_msg(tok.intval == tok1.intval,
      "tst_TSQ_push 03: tok == tok1 (intval)");

  tok = TSQ_pop(&tsq);
  test_assert_msg(tsq.q.size == 0, "tst_TSQ_push 04: q.size == 0");
  test_assert_msg(tok.type == tok3.type,
      "tst_TSQ_push 04: tok == tok3 (type)");
  test_assert_msg(strequal(tok.strval, tok3.strval),
      "tst_TSQ_push 04: tok == tok3 (strval)");
  test_assert_msg(tok.intval == tok3.intval,
      "tst_TSQ_push 04: tok == tok3 (intval)");

  tok = TSQ_pop(&tsq);
  test_assert_msg(tsq.q.size == 0, "tst_TSQ_push 05: q.size == 0");
  test_assert_msg(tok.type == tokeof.type,
      "tst_TSQ_push 05: tok == tokeof (type)");
  test_assert_msg(strequal(tok.strval, tokeof.strval),
      "tst_TSQ_push 05: tok == tokeof (strval)");
  test_assert_msg(tok.intval == tokeof.intval,
      "tst_TSQ_push 05: tok == tokeof (intval)");

  tok = TSQ_pop(&tsq);
  test_assert_msg(tsq.q.size == 0, "tst_TSQ_push 06: q.size == 0");
  test_assert_msg(tok.type == tokeof.type,
      "tst_TSQ_push 06: tok == tokeof (type)");
  test_assert_msg(strequal(tok.strval, tokeof.strval),
      "tst_TSQ_push 06: tok == tokeof (strval)");
  test_assert_msg(tok.intval == tokeof.intval,
      "tst_TSQ_push 06: tok == tokeof (intval)");

  TSQ_close(&tsq);
}

void tst_TSQ_pop() {
  // tested in tst_TSQ_push()
}

void tst_TSQ_to_TS() {
  struct TokenStreamQueue tsq;
  struct Token tok;
  TSQ_init(&tsq);
  struct TokenStream ts = TSQ_to_TS(&tsq);

  test_assert_msg(TS_is_eof(&ts), "tst_TSQ_to_TS 01: is_eof == true");

  tok.type = TT_WHITESPACE;
  tok.strval = "  \t   \t     \t"; // 13 characters
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_NEWLINE;
  tok.strval = "\n";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_IDENTIFIER;
  tok.strval = "hello34_my_friend";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_WHITESPACE;
  tok.strval = "  ";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_INT_LITERAL;
  tok.strval = "9876";
  tok.intval = 9876;
  TSQ_push(&tsq, tok);

  tok.type = TT_SEMICOLON;
  tok.strval = ";";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  tok.type = TT_NEWLINE;
  tok.strval = "\n";
  tok.intval = 0;
  TSQ_push(&tsq, tok);

  test_assert_msg(TS_get_lineno(&ts) == 1, "tst_TSQ_to_TS 02: lineno == 1");
  test_assert_msg(TS_get_column(&ts) == 0, "tst_TSQ_to_TS 02: column == 0");
  test_assert_msg(strequal(TS_get_filepath(&ts), "<TokenStreamQueue>"),
      "tst_TSQ_to_TS 02: filepath == '<TokenStreamQueue>'");
  test_assert_msg(!TS_is_eof(&ts), "tst_TSQ_to_TS 02: is_eof == false");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_WHITESPACE,
      "tst_TSQ_to_TS 03: tok.type == TT_WHITESPACE");
  test_assert_msg(strequal(tok.strval, "  \t   \t     \t"),
      "tst_TSQ_to_TS 03: tok.strval = '  \\t   \\t     \\t'");
  test_assert_msg(tok.intval == 0, "tst_TSQ_to_TS 03: tok.intval = 0");
  test_assert_msg(TS_get_lineno(&ts) == 1, "tst_TSQ_to_TS 03: lineno == 1");
  test_assert_msg(TS_get_column(&ts) == 13, "tst_TSQ_to_TS 03: column == 13");
  test_assert_msg(!TS_is_eof(&ts), "tst_TSQ_to_TS 03: is_eof == false");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_NEWLINE,
      "tst_TSQ_to_TS 04: tok.type == TT_NEWLINE");
  test_assert_msg(strequal(tok.strval, "\n"),
      "tst_TSQ_to_TS 04: tok.strval = '\\n'");
  test_assert_msg(tok.intval == 0, "tst_TSQ_to_TS 04: tok.intval = 0");
  test_assert_msg(TS_get_lineno(&ts) == 2, "tst_TSQ_to_TS 04: lineno == 2");
  test_assert_msg(TS_get_column(&ts) == 0, "tst_TSQ_to_TS 04: column == 0");
  test_assert_msg(!TS_is_eof(&ts), "tst_TSQ_to_TS 04: is_eof == false");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_IDENTIFIER,
      "tst_TSQ_to_TS 05: tok.type == TT_IDENTIFIER");
  test_assert_msg(strequal(tok.strval, "hello34_my_friend"),
      "tst_TSQ_to_TS 05: tok.strval = 'hello34_my_friend'");
  test_assert_msg(tok.intval == 0, "tst_TSQ_to_TS 05: tok.intval = 0");
  test_assert_msg(TS_get_lineno(&ts) == 2, "tst_TSQ_to_TS 05: lineno == 2");
  test_assert_msg(TS_get_column(&ts) == 17, "tst_TSQ_to_TS 05: column == 17");
  test_assert_msg(!TS_is_eof(&ts), "tst_TSQ_to_TS 05: is_eof == false");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_WHITESPACE,
      "tst_TSQ_to_TS 06: tok.type == TT_WHITESPACE");
  test_assert_msg(strequal(tok.strval, "  "),
      "tst_TSQ_to_TS 06: tok.strval = '  '");
  test_assert_msg(tok.intval == 0, "tst_TSQ_to_TS 06: tok.intval = 0");
  test_assert_msg(TS_get_lineno(&ts) == 2, "tst_TSQ_to_TS 06: lineno == 2");
  test_assert_msg(TS_get_column(&ts) == 19, "tst_TSQ_to_TS 06: column == 19");
  test_assert_msg(!TS_is_eof(&ts), "tst_TSQ_to_TS 06: is_eof == false");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_INT_LITERAL,
      "tst_TSQ_to_TS 07: tok.type == TT_INT_LITERAL");
  test_assert_msg(strequal(tok.strval, "9876"),
      "tst_TSQ_to_TS 07: tok.strval = '9876'");
  test_assert_msg(tok.intval == 9876, "tst_TSQ_to_TS 07: tok.intval = 9876");
  test_assert_msg(TS_get_lineno(&ts) == 2, "tst_TSQ_to_TS 07: lineno == 2");
  test_assert_msg(TS_get_column(&ts) == 23, "tst_TSQ_to_TS 07: column == 23");
  test_assert_msg(!TS_is_eof(&ts), "tst_TSQ_to_TS 07: is_eof == false");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_SEMICOLON,
      "tst_TSQ_to_TS 08: tok.type == TT_SEMICOLON");
  test_assert_msg(strequal(tok.strval, ";"),
      "tst_TSQ_to_TS 08: tok.strval = ';'");
  test_assert_msg(tok.intval == 0, "tst_TSQ_to_TS 08: tok.intval = 0");
  test_assert_msg(TS_get_lineno(&ts) == 2, "tst_TSQ_to_TS 08: lineno == 2");
  test_assert_msg(TS_get_column(&ts) == 24, "tst_TSQ_to_TS 08: column == 24");
  test_assert_msg(!TS_is_eof(&ts), "tst_TSQ_to_TS 08: is_eof == false");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_NEWLINE,
      "tst_TSQ_to_TS 09: tok.type == TT_NEWLINE");
  test_assert_msg(strequal(tok.strval, "\n"),
      "tst_TSQ_to_TS 09: tok.strval = '\\n'");
  test_assert_msg(tok.intval == 0, "tst_TSQ_to_TS 09: tok.intval = 0");
  test_assert_msg(TS_get_lineno(&ts) == 3, "tst_TSQ_to_TS 09: lineno == 3");
  test_assert_msg(TS_get_column(&ts) == 0, "tst_TSQ_to_TS 09: column == 0");
  test_assert_msg(TS_is_eof(&ts), "tst_TSQ_to_TS 09: is_eof == false");

  tok = TS_next_token(&ts);
  test_assert_msg(tok.type == TT_EOF,
      "tst_TSQ_to_TS 10: tok.type == TT_EOF");
  test_assert_msg(strequal(tok.strval, ""),
      "tst_TSQ_to_TS 10: tok.strval = ''");
  test_assert_msg(tok.intval == 0, "tst_TSQ_to_TS 09: tok.intval = 0");
  test_assert_msg(TS_get_lineno(&ts) == 3, "tst_TSQ_to_TS 09: lineno == 3");
  test_assert_msg(TS_get_column(&ts) == 0, "tst_TSQ_to_TS 09: column == 0");
  test_assert_msg(TS_is_eof(&ts), "tst_TSQ_to_TS 09: is_eof == true");

  TS_close(&ts);

  test_assert_msg(tsq.q.size == 0, "tst_TSQ_to_TS end: q.size == 0");
  test_assert_msg(tsq.lineno == 1, "tst_TSQ_to_TS end: lineno == 1");
  test_assert_msg(tsq.column == 0, "tst_TSQ_to_TS end: column == 0");
}

int main(void) {
  tst_TS_next_token();
  tst_TS_get_lineno();
  tst_TS_get_column();
  tst_TS_get_filepath();
  tst_TS_is_eof();
  tst_TS_close();
  tst_TSQ_init();
  tst_TSQ_close();
  tst_TSQ_clear();
  tst_TSQ_push();
  tst_TSQ_pop();
  tst_TSQ_to_TS();
  return testval;
}
