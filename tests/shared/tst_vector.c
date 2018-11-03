#include "vector.c"
#include "test_harness.c"

void tst_V_init() {
  struct Vector v;
  V_init(&v, 10);

  test_assert_msg(v.data != NULL, "tst_V_init 01: data != NULL");
  test_assert_msg(v.capacity == 10, "tst_V_init 01: capacity == 10");
  test_assert_msg(v.size == 0, "tst_V_init 01: size == 0");

  V_del(&v);
  V_init(&v, 0);

  test_assert_msg(v.data != NULL, "tst_V_init 02: data != NULL");
  test_assert_msg(v.capacity == 1, "tst_V_init 02: capacity == 1");
  test_assert_msg(v.size == 0, "tst_V_init 02: size == 0");

  V_del(&v);
}

void tst_V_del() {
  struct Vector v;
  V_init(&v, 10);

  V_del(&v);

  test_assert_msg(v.data == NULL, "tst_V_del 01: data == NULL");
  test_assert_msg(v.capacity == 0, "tst_V_del 01: capacity == 0");
  test_assert_msg(v.size == 0, "tst_V_del 01: size == 0");

  V_init(&v, 10);
  V_append(&v, &tst_V_del);
  V_del(&v);

  test_assert_msg(v.data == NULL, "tst_V_del 02: data == NULL");
  test_assert_msg(v.capacity == 0, "tst_V_del 02: capacity == 0");
  test_assert_msg(v.size == 0, "tst_V_del 02: size == 0");
}

void tst_V_clear() {
  struct Vector v;
  V_init(&v, 10);

  V_clear(&v);

  test_assert_msg(v.data != NULL, "tst_V_clear 01: data != NULL");
  test_assert_msg(v.capacity == 10, "tst_V_clear 01: capacity == 10");
  test_assert_msg(v.size == 0, "tst_V_clear 01: size == 0");

  V_append(&v, &tst_V_clear);
  V_clear(&v);

  test_assert_msg(v.data != NULL, "tst_V_clear 02: data != NULL");
  test_assert_msg(v.capacity == 10, "tst_V_clear 02: capacity == 10");
  test_assert_msg(v.size == 0, "tst_V_clear 02: size == 0");

  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_clear(&v);

  test_assert_msg(v.data != NULL, "tst_V_clear 03: data != NULL");
  test_assert_msg(v.capacity == 10, "tst_V_clear 03: capacity == 10");
  test_assert_msg(v.size == 0, "tst_V_clear 03: size == 0");

  // add more than 10 things to trigger a resize
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_append(&v, &tst_V_clear);
  V_clear(&v);

  test_assert_msg(v.data != NULL, "tst_V_clear 04: data != NULL");
  test_assert_msg(v.capacity == 20, "tst_V_clear 04: capacity == 20");
  test_assert_msg(v.size == 0, "tst_V_clear 04: size == 0");

  V_del(&v);
}

void tst_V_append() {
  struct Vector v;
  V_init(&v, 10);

  V_append(&v, &tst_V_append);
  V_append(&v, &V_init);
  V_append(&v, &V_del);
  V_append(&v, &V_clear);
  V_append(&v, &V_append);
  V_append(&v, &V_init);
  V_append(&v, &V_del);
  V_append(&v, &V_clear);
  V_append(&v, &V_append);
  V_append(&v, &V_init);
  V_append(&v, &V_del);
  V_append(&v, &V_clear);
  V_append(&v, &V_append);

  test_assert_msg(v.capacity == 20, "tst_V_append: capacity == 20");
  test_assert_msg(v.size == 13, "tst_V_append: size == 13");
  test_assert_msg(V_at(&v,  0) == &tst_V_append,
      "tst_V_append: v[ 0] == &tst_V_append");
  test_assert_msg(V_at(&v,  1) == &V_init,
      "tst_V_append: v[ 1] == &V_init");
  test_assert_msg(V_at(&v,  2) == &V_del,
      "tst_V_append: v[ 2] == &V_del");
  test_assert_msg(V_at(&v,  3) == &V_clear,
      "tst_V_append: v[ 3] == &V_clear");
  test_assert_msg(V_at(&v,  4) == &V_append,
      "tst_V_append: v[ 4] == &V_append");
  test_assert_msg(V_at(&v,  5) == &V_init,
      "tst_V_append: v[ 5] == &V_init");
  test_assert_msg(V_at(&v,  6) == &V_del,
      "tst_V_append: v[ 6] == &V_del");
  test_assert_msg(V_at(&v,  7) == &V_clear,
      "tst_V_append: v[ 7] == &V_clear");
  test_assert_msg(V_at(&v,  8) == &V_append,
      "tst_V_append: v[ 8] == &V_append");
  test_assert_msg(V_at(&v,  9) == &V_init,
      "tst_V_append: v[ 9] == &V_init");
  test_assert_msg(V_at(&v, 10) == &V_del,
      "tst_V_append: v[10] == &V_del");
  test_assert_msg(V_at(&v, 11) == &V_clear,
      "tst_V_append: v[11] == &V_clear");
  test_assert_msg(V_at(&v, 12) == &V_append,
      "tst_V_append: v[12] == &V_append");

  V_del(&v);
}

void tst_V_pop() {
  struct Vector v;
  V_init(&v, 10);

  V_append(&v, &tst_V_append);
  V_append(&v, &V_init);
  V_append(&v, &V_del);
  V_append(&v, &V_clear);
  V_append(&v, &V_append);
  V_append(&v, &V_init);
  V_append(&v, &V_del);
  V_append(&v, &V_clear);
  V_append(&v, &V_append);
  V_append(&v, &V_init);
  V_append(&v, &V_del);
  V_append(&v, &V_clear);
  V_append(&v, &V_append);

  test_assert_msg(V_pop(&v) == &V_append, "tst_V_pop 01: v.pop() == &V_append");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 01: capacity == 20");
  test_assert_msg(v.size == 12,           "tst_V_pop 01: size == 12");
  test_assert_msg(V_pop(&v) == &V_clear , "tst_V_pop 02: v.pop() == &V_clear ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 02: capacity == 20");
  test_assert_msg(v.size == 11,           "tst_V_pop 02: size == 11");
  test_assert_msg(V_pop(&v) == &V_del   , "tst_V_pop 03: v.pop() == &V_del   ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 03: capacity == 20");
  test_assert_msg(v.size == 10,           "tst_V_pop 03: size == 10");
  test_assert_msg(V_pop(&v) == &V_init  , "tst_V_pop 04: v.pop() == &V_init  ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 04: capacity == 20");
  test_assert_msg(v.size ==  9,           "tst_V_pop 04: size ==  9");
  test_assert_msg(V_pop(&v) == &V_append, "tst_V_pop 05: v.pop() == &V_append");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 05: capacity == 20");
  test_assert_msg(v.size ==  8,           "tst_V_pop 05: size ==  8");
  test_assert_msg(V_pop(&v) == &V_clear , "tst_V_pop 06: v.pop() == &V_clear ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 06: capacity == 20");
  test_assert_msg(v.size ==  7,           "tst_V_pop 06: size ==  7");
  test_assert_msg(V_pop(&v) == &V_del   , "tst_V_pop 07: v.pop() == &V_del   ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 07: capacity == 20");
  test_assert_msg(v.size ==  6,           "tst_V_pop 07: size ==  6");
  test_assert_msg(V_pop(&v) == &V_init  , "tst_V_pop 08: v.pop() == &V_init  ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 08: capacity == 20");
  test_assert_msg(v.size ==  5,           "tst_V_pop 08: size ==  5");
  test_assert_msg(V_pop(&v) == &V_append, "tst_V_pop 09: v.pop() == &V_append");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 09: capacity == 20");
  test_assert_msg(v.size ==  4,           "tst_V_pop 09: size ==  4");
  test_assert_msg(V_pop(&v) == &V_clear , "tst_V_pop 10: v.pop() == &V_clear ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 10: capacity == 20");
  test_assert_msg(v.size ==  3,           "tst_V_pop 10: size ==  3");
  test_assert_msg(V_pop(&v) == &V_del   , "tst_V_pop 11: v.pop() == &V_del   ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 11: capacity == 20");
  test_assert_msg(v.size ==  2,           "tst_V_pop 11: size ==  2");
  test_assert_msg(V_pop(&v) == &V_init  , "tst_V_pop 12: v.pop() == &V_init  ");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 12: capacity == 20");
  test_assert_msg(v.size ==  1,           "tst_V_pop 12: size ==  1");
  test_assert_msg(V_pop(&v) == &tst_V_append, "tst_V_pop 13: v.pop() == &tst_V_append");
  test_assert_msg(v.capacity == 20,       "tst_V_pop 13: capacity == 20");
  test_assert_msg(v.size ==  0,           "tst_V_pop 13: size ==  0");

  V_del(&v);
}

void tst_V_at() {
  // already tested by tst_V_append()
}

void tst_V_set() {
  struct Vector v;
  V_init(&v, 10);

  V_append(&v, &tst_V_append);
  V_append(&v, &V_init);
  V_append(&v, &V_del);
  V_append(&v, &V_clear);

  V_set(&v, 1, &V_pop);
  V_set(&v, 1, &V_at);
  V_set(&v, 2, &V_set);

  test_assert_msg(V_at(&v, 1) == &V_at, "tst_V_set: v[1] == &V_at");
  test_assert_msg(V_at(&v, 2) == &V_set, "tst_V_set: v[2] == &V_set");
  test_assert_msg(v.capacity == 10, "tst_V_set: capacity == 10");
  test_assert_msg(v.size == 4, "tst_V_set: size == 4");

  V_del(&v);
}


int main(void) {
  tst_V_init();
  tst_V_del();
  tst_V_clear();
  tst_V_append();
  tst_V_pop();
  tst_V_at();
  tst_V_set();
  return testval;
}
