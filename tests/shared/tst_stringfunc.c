#include "stringfunc.c"
#include "test_harness.c"

void tst_strequal() {
  test_assert_msg(strequal("hello", "hello"),
      "tst_strequal: 'hello' == 'hello'");
  test_assert_msg(!strequal("he", "hello"),
      "tst_strequal: 'he' != 'hello'");
  test_assert_msg(!strequal("hello", "he"),
      "tst_strequal: 'hello' != 'he'");
  test_assert_msg(strequal("he\0llo", "he"),
      "tst_strequal: 'he\\0llo' == 'he'");
  test_assert_msg(!strequal("he\nllo", "he"),
      "tst_strequal: 'he\\nllo' != 'he'");
  test_assert_msg(strequal("he\nllo", "he\nllo"),
      "tst_strequal: 'he\\nllo' == 'he\nllo'");
}

int main(void) {
  return testval;
}
