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
  test_assert_msg(false, "tst_TokenEater_init: unimplemented");
  TokenEater_del(&te);
}

void tst_TokenEater_del() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &ts);
  test_assert_msg(false, "tst_TokenEater_init: unimplemented");
  TokenEater_del(&te);
}

void tst_TokenEater_add() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &ts);
  test_assert_msg(false, "tst_TokenEater_init: unimplemented");
  TokenEater_del(&te);
}

void tst_TokenEater_next() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &ts);
  test_assert_msg(false, "tst_TokenEater_init: unimplemented");
  TokenEater_del(&te);
}

void tst_TokenEater_to_TokenStream() {
  struct TokenEater te;
  struct TokenStreamQueue tsq;
  struct TokenStream ts;
  TSQ_init(&tsq);
  ts = TSQ_to_TS(&tsq);
  TokenEater_init(&te, &ts);
  test_assert_msg(false, "tst_TokenEater_init: unimplemented");
  TokenEater_del(&te);
}

int main(void) {
  tst_TokenEater_init();
  tst_TokenEater_del();
  tst_TokenEater_add();
  tst_TokenEater_next();
  tst_TokenEater_to_TokenStream();
  return testval;
}
