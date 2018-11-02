#include "hash.c"
#include "test_harness.c"

void tst_djb2_hash() {
  // just test that it compiles and runs.  Not that it produces any meaningful
  // answer.
  unsigned long val = djb2_hash("hello there");
  test_assert_msg(val >= 0, "djb2_hash had a weird error...");
}

int main(void) {
  tst_djb2_hash();
  return testval;
}
