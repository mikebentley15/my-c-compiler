#include "string.c"
#include "test_harness.c"

#include "stringfunc.c"

void tst_Str_init() {
  struct String s;
  Str_init(&s);

  test_assert_msg(s.data != NULL, "tst_Str_init: data != NULL");
  test_assert_msg(s.capacity > 0, "tst_Str_init: capacity > 0");
  test_assert_msg(s.size == 0, "tst_Str_init: capacity > 0");
  test_assert_msg(strequal(s.data, ""), "tst_Str_init: data == ''");

  Str_del(&s);
}

void tst_Str_del() {
  struct String s;
  Str_init(&s);
  Str_del(&s);

  test_assert_msg(s.data == NULL, "tst_Str_del 01: data == NULL");
  test_assert_msg(s.capacity == 0, "tst_Str_del 01: capacity == 0");
  test_assert_msg(s.size == 0, "tst_Str_del 01: size == 0");

  Str_init(&s);
  Str_extend(&s, "hello");
  Str_del(&s);

  test_assert_msg(s.data == NULL, "tst_Str_del 02: data == NULL");
  test_assert_msg(s.capacity == 0, "tst_Str_del 02: capacity == 0");
  test_assert_msg(s.size == 0, "tst_Str_del 02: size == 0");
}

void tst_Str_clear() {
  struct String s;
  Str_init(&s);
  int capacity = s.capacity;

  Str_clear(&s);

  test_assert_msg(s.data != NULL, "tst_Str_clear 01: data != NULL");
  test_assert_msg(s.capacity == capacity,
      "tst_Str_clear 01: capacity did not change");
  test_assert_msg(s.size == 0, "tst_Str_clear 01: size == 0");
  test_assert_msg(strequal(s.data, ""), "tst_Str_clear 01: data == ''");

  Str_append(&s, 'h');
  Str_append(&s, 'i');
  capacity = s.capacity;
  Str_clear(&s);

  test_assert_msg(s.data != NULL, "tst_Str_clear 02: data != NULL");
  test_assert_msg(s.capacity == capacity,
      "tst_Str_clear 02: capacity did not change");
  test_assert_msg(s.size == 0, "tst_Str_clear 02: size == 0");
  test_assert_msg(strequal(s.data, ""), "tst_Str_clear 02: data == ''");

  Str_del(&s);
}

void tst_Str_append() {
  struct String s;
  Str_init(&s);

  Str_append(&s, 'h');
  test_assert_msg(s.size == 1, "tst_Str_append 01: size == 1");
  test_assert_msg(strequal(s.data, "h"),
      "tst_Str_append 01: data == 'h'");

  Str_append(&s, 'e');
  test_assert_msg(s.size == 2, "tst_Str_append 02: size == 2");
  test_assert_msg(strequal(s.data, "he"),
      "tst_Str_append 02: data == 'he'");

  Str_append(&s, 'l');
  test_assert_msg(s.size == 3, "tst_Str_append 03: size == 3");
  test_assert_msg(strequal(s.data, "hel"),
      "tst_Str_append 03: data == 'hel'");

  Str_append(&s, 'l');
  test_assert_msg(s.size == 4, "tst_Str_append 04: size == 4");
  test_assert_msg(strequal(s.data, "hell"),
      "tst_Str_append 04: data == 'hell'");

  Str_append(&s, 'o');
  test_assert_msg(s.size == 5, "tst_Str_append 05: size == 5");
  test_assert_msg(strequal(s.data, "hello"),
      "tst_Str_append 05: data == 'hello'");

  Str_del(&s);
}

void tst_Str_extend() {
  struct String s;
  Str_init(&s);

  Str_extend(&s, "hello");

  test_assert_msg(s.size ==  5, "tst_Str_extend 01: size == 5");
  test_assert_msg(strequal(s.data, "hello"),
      "tst_Str_extend 01: data = 'hello'");

  Str_extend(&s, " ");

  test_assert_msg(s.size ==  6, "tst_Str_extend 02: size == 6");
  test_assert_msg(strequal(s.data, "hello "),
      "tst_Str_extend 02: data = 'hello '");

  Str_extend(&s, "world!");

  test_assert_msg(s.size == 12, "tst_Str_extend 03: size == 12");
  test_assert_msg(strequal(s.data, "hello world!"),
      "tst_Str_extend 03: data = 'hello world!'");

  Str_extend(&s, "");

  test_assert_msg(s.size == 12, "tst_Str_extend 04: size == 12");
  test_assert_msg(strequal(s.data, "hello world!"),
      "tst_Str_extend 04: data = 'hello world!'");

  int capacity = s.capacity;
  while (s.size <= capacity) {
    Str_extend(&s, "abc");
  }

  test_assert_msg(s.size > capacity, "tst_Str_extend 05: size > capacity");
  test_assert_msg(s.size <= s.capacity, "tst_Str_extend 05: size <= s.capacity");

  Str_del(&s);
}

void tst_Str_pop() {
  struct String s;
  Str_init(&s);

  Str_extend(&s, "hello there");

  test_assert_msg(Str_pop(&s) == 'e', "tst_Str_pop 01: s.pop() == 'e'");
  test_assert_msg(strequal(s.data, "hello ther"),
      "tst_Str_pop 01: data = 'hello ther'");
  test_assert_msg(s.size == 10, "tst_Str_pop 01: size == 10");

  test_assert_msg(Str_pop(&s) == 'r', "tst_Str_pop 02: s.pop() == 'r'");
  test_assert_msg(strequal(s.data, "hello the"),
      "tst_Str_pop 02: data = 'hello the'");
  test_assert_msg(s.size ==  9, "tst_Str_pop 02: size ==  9");

  test_assert_msg(Str_pop(&s) == 'e', "tst_Str_pop 03: s.pop() == 'e'");
  test_assert_msg(strequal(s.data, "hello th"),
      "tst_Str_pop 03: data = 'hello th'");
  test_assert_msg(s.size ==  8, "tst_Str_pop 03: size ==  8");

  test_assert_msg(Str_pop(&s) == 'h', "tst_Str_pop 04: s.pop() == 'h'");
  test_assert_msg(strequal(s.data, "hello t"),
      "tst_Str_pop 04: data = 'hello t'");
  test_assert_msg(s.size ==  7, "tst_Str_pop 04: size ==  7");

  test_assert_msg(Str_pop(&s) == 't', "tst_Str_pop 05: s.pop() == 't'");
  test_assert_msg(strequal(s.data, "hello "),
      "tst_Str_pop 05: data = 'hello '");
  test_assert_msg(s.size ==  6, "tst_Str_pop 05: size ==  6");

  test_assert_msg(Str_pop(&s) == ' ', "tst_Str_pop 06: s.pop() == ' '");
  test_assert_msg(strequal(s.data, "hello"),
      "tst_Str_pop 06: data = 'hello'");
  test_assert_msg(s.size ==  5, "tst_Str_pop 06: size ==  5");

  test_assert_msg(Str_pop(&s) == 'o', "tst_Str_pop 07: s.pop() == 'o'");
  test_assert_msg(strequal(s.data, "hell"),
      "tst_Str_pop 07: data = 'hell'");
  test_assert_msg(s.size ==  4, "tst_Str_pop 07: size ==  4");

  test_assert_msg(Str_pop(&s) == 'l', "tst_Str_pop 08: s.pop() == 'l'");
  test_assert_msg(strequal(s.data, "hel"),
      "tst_Str_pop 08: data = 'hel'");
  test_assert_msg(s.size ==  3, "tst_Str_pop 08: size ==  3");

  test_assert_msg(Str_pop(&s) == 'l', "tst_Str_pop 09: s.pop() == 'l'");
  test_assert_msg(strequal(s.data, "he"),
      "tst_Str_pop 09: data = 'he'");
  test_assert_msg(s.size ==  2, "tst_Str_pop 09: size ==  2");

  test_assert_msg(Str_pop(&s) == 'e', "tst_Str_pop 10: s.pop() == 'e'");
  test_assert_msg(strequal(s.data, "h"),
      "tst_Str_pop 10: data = 'h'");
  test_assert_msg(s.size ==  1, "tst_Str_pop 10: size ==  1");

  test_assert_msg(Str_pop(&s) == 'h', "tst_Str_pop 11: s.pop() == 'h'");
  test_assert_msg(strequal(s.data, ""),
      "tst_Str_pop 11: data = ''");
  test_assert_msg(s.size ==  0, "tst_Str_pop 11: size ==  0");

  Str_del(&s);
}

void tst_Str_at() {
  struct String s;
  Str_init(&s);

  Str_extend(&s, "hello");

  test_assert_msg(Str_at(&s, 0) == 'h', "tst_Str_at: s[0] == 'h'");
  test_assert_msg(Str_at(&s, 1) == 'e', "tst_Str_at: s[1] == 'e'");
  test_assert_msg(Str_at(&s, 2) == 'l', "tst_Str_at: s[2] == 'l'");
  test_assert_msg(Str_at(&s, 3) == 'l', "tst_Str_at: s[3] == 'l'");
  test_assert_msg(Str_at(&s, 4) == 'o', "tst_Str_at: s[4] == 'o'");

  test_assert_msg(s.size == 5, "tst_Str_at: size == 5");

  Str_del(&s);
}

void tst_Str_set() {
  struct String s;
  Str_init(&s);

  Str_extend(&s, "hello");
  Str_set(&s, 2, 'p');
  Str_set(&s, 3, 'p');
  Str_set(&s, 1, 'i');

  test_assert_msg(strequal(s.data, "hippo"), "tst_Str_set: data == 'hippo'");
  test_assert_msg(s.size == 5, "tst_Str_set: size == 5");

  Str_del(&s);
}


int main(void) {
  tst_Str_init();
  tst_Str_del();
  tst_Str_clear();
  tst_Str_append();
  tst_Str_extend();
  tst_Str_pop();
  tst_Str_at();
  tst_Str_set();
  return testval;
}
