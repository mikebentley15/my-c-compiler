#include "set.c"
#include "test_harness.c"

#include "hash.c"
#include "util.c"
#include "stringfunc.c"

void tst_SSet_init() {
  struct StringSet set;
  SSet_init(&set, 10);

  test_assert_msg(set.data != NULL, "tst_SSet_init: data != NULL");
  test_assert_msg(set.size == 0, "tst_SSet_init: size == 0");
  test_assert_msg(set.capacity >= 10, "tst_SSet_init: capacity >= 10");
  test_assert_msg(set.collisions == 0, "tst_SSet_init: collisions == 0");

  SSet_del(&set);
}

void tst_SSet_del() {
  struct StringSet set;
  SSet_init(&set, 10);

  set.size = 10;
  set.collisions = 1050;
  SSet_del(&set);

  test_assert_msg(set.data == NULL, "tst_SSet_del: data == NULL");
  test_assert_msg(set.size == 0, "tst_SSet_del: size == 0");
  test_assert_msg(set.capacity == 0, "tst_SSet_del: capacity == 0");
  test_assert_msg(set.collisions == 0, "tst_SSet_del: collisions == 0");
}

void tst_SSet_clear() {
  struct StringSet set;
  SSet_init(&set, 10);

  SSet_add(&set, "hello");
  SSet_add(&set, "bye");

  SSet_clear(&set);

  test_assert_msg(!SSet_contains(&set, "hello"),
      "tst_SSet_clear: hello not in set");
  test_assert_msg(!SSet_contains(&set, "bye"),
      "tst_SSet_clear: bye not in set");
  test_assert_msg(set.size == 0, "tst_SSet_clear: size == 0");

  SSet_del(&set);
}

void tst_SSet_add() {
  struct StringSet set;
  SSet_init(&set, 10);
  bool added = false;

  added = SSet_add(&set, "hello");

  test_assert_msg(added, "tst_SSet_add 01: added == true");
  test_assert_msg(set.size == 1, "tst_SSet_add 01: size == 1");
  test_assert_msg(set.collisions == 0, "tst_SSet_add 01: collisions == 0");
  test_assert_msg(SSet_contains(&set, "hello"),
      "tst_SSet_add 01: hello in set");
  test_assert_msg(!SSet_contains(&set, "bye"),
      "tst_SSet_add 01: bye not in set");

  added = SSet_add(&set, "hello");

  test_assert_msg(!added, "tst_SSet_add 02: added == false");
  test_assert_msg(set.size == 1, "tst_SSet_add 02: size == 1");
  test_assert_msg(set.collisions == 0, "tst_SSet_add 02: collisions == 0");
  test_assert_msg(SSet_contains(&set, "hello"),
      "tst_SSet_add 02: hello in set");
  test_assert_msg(!SSet_contains(&set, "bye"),
      "tst_SSet_add 02: bye not in set");

  added = SSet_add(&set, "bye");

  test_assert_msg(added, "tst_SSet_add 03: added == true");
  test_assert_msg(set.size == 2, "tst_SSet_add 03: size == 2");
  test_assert_msg(SSet_contains(&set, "hello"),
      "tst_SSet_add 03: hello in set");
  test_assert_msg(SSet_contains(&set, "bye"),
      "tst_SSet_add 03: bye in set");

  SSet_del(&set);
}

void tst_SSet_contains() {
  // tested in tst_SSet_add()
}

// Not yet found to be necessary
//void tst_SSet_remove() {}
//void tst_SSet_resize() {}


int main(void) {
  tst_SSet_init();
  tst_SSet_del();
  tst_SSet_clear();
  tst_SSet_add();
  tst_SSet_contains();
  //tst_SSet_remove();
  //tst_SSet_resize();
  return testval;
}

