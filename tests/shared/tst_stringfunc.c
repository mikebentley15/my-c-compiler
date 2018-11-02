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

void tst_strgreater() {
  test_assert_msg(!strgreater("hello", "hello"),
      "tst_strequal: !'hello' > 'hello'");
  test_assert_msg(!strgreater("he", "hello"),
      "tst_strequal: !'he' > 'hello'");
  test_assert_msg(strgreater("hello", "he"),
      "tst_strequal: 'hello' > 'he'");
  test_assert_msg(!strgreater("he\0llo", "he"),
      "tst_strequal: !'he\\0llo' > 'he'");
  test_assert_msg(strgreater("he\nllo", "he"),
      "tst_strequal: 'he\\nllo' > 'he'");
  test_assert_msg(!strgreater("he\nllo", "he\nllo"),
      "tst_strequal: !'he\\nllo' > 'he\nllo'");
}

void tst_strcompare() {
  test_assert_msg(strcompare("hello", "hello") == 0,
      "tst_strequal: 'hello' == 'hello'");
  test_assert_msg(strcompare("he", "hello") < 0,
      "tst_strequal: 'he' < 'hello'");
  test_assert_msg(strcompare("hello", "he") > 0,
      "tst_strequal: 'hello' > 'he'");
  test_assert_msg(strcompare("he\0llo", "he") == 0,
      "tst_strequal: 'he\\0llo' == 'he'");
  test_assert_msg(strcompare("he\nllo", "he") > 0,
      "tst_strequal: 'he\\nllo' > 'he'");
  test_assert_msg(strcompare("he\nllo", "he\nllo") == 0,
      "tst_strequal: 'he\\nllo' == 'he\nllo'");
}

void tst_to_lowercase() {
  char buf[11];
  buf[0] = 'A';
  buf[1] = 'c';
  buf[2] = 'E';
  buf[3] = ' ';
  buf[4] = 'p';
  buf[5] = 'I';
  buf[6] = 'l';
  buf[7] = 'O';
  buf[8] = 't';
  buf[9] = '!';
  buf[10] = '\0';
  test_assert_msg(strequal(to_lowercase(buf), "ace pilot!"),
      "tst_to_lowercase: lowercase('AcE pIlOt!') == 'ace pilot!'");
}

void tst_to_uppercase() {
  char buf[11];
  buf[0] = 'A';
  buf[1] = 'c';
  buf[2] = 'E';
  buf[3] = ' ';
  buf[4] = 'p';
  buf[5] = 'I';
  buf[6] = 'l';
  buf[7] = 'O';
  buf[8] = 't';
  buf[9] = '!';
  buf[10] = '\0';
  test_assert_msg(strequal(to_uppercase(buf), "ACE PILOT!"),
      "tst_to_lowercase: lowercase('AcE pIlOt!') == 'ACE PILOT!'");
}

int main(void) {
  tst_strequal();
  tst_strgreater();
  tst_strcompare();
  tst_to_lowercase();
  tst_to_uppercase();
  return testval;
}
