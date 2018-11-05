#include "scanner.c"
#include "test_harness.c"

#include "charstream.c"
#include "string.c"
#include "stringfunc.c"
#include "stringstream.c"
#include "util.c"

void tst_Scanner_init() {
  struct Scanner s;
  struct StringStream in;
  const char* input = "hello there my friend;";
  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);
  test_assert_msg(false, "tst_Scanner_init: unimplemented");
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
  test_assert_msg(false, "tst_Scanner_close: unimplemented");
  Scanner_close(&s);
  SS_close(&in);
}

void tst_Scanner_is_eof() {
  struct Scanner s;
  struct StringStream in;
  const char* input = "hello there my friend;";
  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);
  test_assert_msg(false, "tst_Scanner_is_eof: unimplemented");
  Scanner_close(&s);
  SS_close(&in);
}

void tst_Scanner_next() {
  struct Scanner s;
  struct StringStream in;
  const char* input = "hello there my friend;";
  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);
  test_assert_msg(false, "tst_Scanner_next: unimplemented");
  Scanner_close(&s);
  SS_close(&in);
}

void tst_Scanner_to_TokenStream() {
  struct Scanner s;
  struct StringStream in;
  const char* input = "hello there my friend;";
  SS_init(&in, input);
  struct CharStream cs = SS_to_CharStream(&in);
  Scanner_init(&s, &cs);
  test_assert_msg(false, "tst_Scanner_to_TokenStream: unimplemented");
  Scanner_close(&s);
  SS_close(&in);
}

int main(void) {
  tst_Scanner_init();
  tst_Scanner_close();
  tst_Scanner_is_eof();
  tst_Scanner_next();
  tst_Scanner_to_TokenStream();
  return testval;
}
