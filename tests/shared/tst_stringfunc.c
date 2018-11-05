#include "stringfunc.c"
#include "test_harness.c"

void tst_is_whitespace() {
  bool expected[128];
  int i = 0;
  while (i < 128) {
    expected[i] = false;
    i++;
  }
  expected[(int)' ']  = true;
  expected[(int)'\n'] = true;
  expected[(int)'\t'] = true;

  i = 0;
  while (i < 128) {
    test_assert_msg(is_whitespace((char) i) == expected[i],
        "tst_is_whitespace: i == expected[i]");
    i++;
  }
}

void tst_is_digit() {
  bool expected[128];
  int i = 0;
  while (i < 128) {
    expected[i] = false;
    i++;
  }
  i = (int)'0';
  while (i <= (int)'9') {
    expected[i] = true;
    i++;
  }

  i = 0;
  while (i < 128) {
    test_assert_msg(is_digit((char) i) == expected[i],
        "tst_is_digit: i == expected[i]");
    i++;
  }
}

void tst_is_letter() {
  bool expected[128];
  int i = 0;
  while (i < 128) {
    expected[i] = false;
    i++;
  }
  i = (int)'a';
  while (i <= (int)'z') {
    expected[i] = true;
    i++;
  }
  i = (int)'A';
  while (i <= (int)'Z') {
    expected[i] = true;
    i++;
  }

  i = 0;
  while (i < 128) {
    test_assert_msg(is_letter((char) i) == expected[i],
        "tst_is_digit: i == expected[i]");
    i++;
  }
}

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

void tst_memcopy() {
  char buf[50];
  memcopy(buf, "hello there!", 13);
  test_assert_msg(strequal(buf, "hello there!"),
      "tst_memcopy: buf == 'hello there!'");
}

void tst_memfill() {
  char buf[20];
  memfill(buf, 19, 'a');
  buf[19] = '\0';
  test_assert_msg(strequal(buf, "aaaaaaaaaaaaaaaaaaa"),
      "tst_memfill: buf == 'a'*19");
}

void tst_memzero() {
  int buf[20];
  memzero(buf, 20*sizeof(int));
  int i = 0;
  while (i < 20) {
    test_assert_msg(buf[i] == 0, "tst_memzero: buf[i] == 0");
    i++;
  }
}

int main(void) {
  tst_is_whitespace();
  tst_is_digit();
  tst_is_letter();
  tst_strequal();
  tst_strgreater();
  tst_strcompare();
  tst_to_lowercase();
  tst_to_uppercase();
  tst_memcopy();
  tst_memfill();
  tst_memzero();
  return testval;
}
