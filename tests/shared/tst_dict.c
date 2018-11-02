#include "dict.c"
#include "test_harness.c"

#include "hash.c"
#include "stringfunc.c"
#include "util.c"

void tst_Dict_init() {
  struct Dict d;
  Dict_init(&d, 10);
  
  test_assert_msg(d.keys != NULL, "tst_Dict_init: keys != NULL");
  test_assert_msg(d.vals != NULL, "tst_Dict_init: vals != NULL");
  test_assert_msg(d.capacity >= 10, "tst_Dict_init: capacity >= 10");
  test_assert_msg(d.size == 0, "tst_Dict_init: size == 0");
  test_assert_msg(d.collisions == 0, "tst_Dict_init: collisions == 0");

  Dict_del(&d);
}

void tst_Dict_resize() {
  struct Dict d;
  Dict_init(&d, 10);

  Dict_set(&d, "hello", "precious");
  Dict_set(&d, "bye", "sweetums");

  test_assert_msg(d.size == 2,
      "tst_Dict_resize before: size == 2");
  test_assert_msg(d.collisions <= 1,
      "tst_Dict_resize before: collisions <= 1");
  test_assert_msg(d.capacity >= 10,
      "tst_Dict_resize before: capacity >= 10");
  test_assert_msg(Dict_contains(&d, "hello"),
      "tst_Dict_resize before: 'hello' in d");
  test_assert_msg(Dict_contains(&d, "bye"),
      "tst_Dict_resize before: 'bye' in d");
  test_assert_msg(strequal(Dict_at(&d, "hello"), "precious"),
      "tst_Dict_resize before: d['hello'] == 'precious'");
  test_assert_msg(strequal(Dict_at(&d, "bye"), "sweetums"),
      "tst_Dict_resize before: d['bye'] == 'sweetums'");

  Dict_resize(&d, 1000);

  test_assert_msg(d.size == 2,
      "tst_Dict_resize after: size == 2");
  test_assert_msg(d.collisions <= 1,
      "tst_Dict_resize after: collisions <= 1");
  test_assert_msg(d.capacity >= 1000,
      "tst_Dict_resize after: capacity >= 1000");
  test_assert_msg(Dict_contains(&d, "hello"),
      "tst_Dict_resize after: 'hello' in d");
  test_assert_msg(Dict_contains(&d, "bye"),
      "tst_Dict_resize after: 'bye' in d");
  test_assert_msg(strequal(Dict_at(&d, "hello"), "precious"),
      "tst_Dict_resize after: d['hello'] == 'precious'");
  test_assert_msg(strequal(Dict_at(&d, "bye"), "sweetums"),
      "tst_Dict_resize after: d['bye'] == 'sweetums'");

  Dict_del(&d);
}

void tst_Dict_del() {
  struct Dict d;
  Dict_init(&d, 10);
  
  d.size = 10;
  d.collisions = 1050;
  Dict_del(&d);

  test_assert_msg(d.keys == NULL, "tst_Dict_del: keys == NULL");
  test_assert_msg(d.vals == NULL, "tst_Dict_del: vals == NULL");
  test_assert_msg(d.capacity == 0, "tst_Dict_del: capacity == 0");
  test_assert_msg(d.size == 0, "tst_Dict_del: size == 0");
  test_assert_msg(d.collisions == 0, "tst_Dict_del: collisions == 0");
}

void tst_Dict_clear() {
  struct Dict d;
  Dict_init(&d, 10);

  Dict_set(&d, "hello", "value");
  Dict_set(&d, "bye", "value");

  Dict_clear(&d);

  test_assert_msg(!Dict_contains(&d, "hello"),
      "tst_Dict_clear: hello not in d");
  test_assert_msg(!Dict_contains(&d, "bye"), "tst_Dict_clear: bye not in d");
  test_assert_msg(d.keys != NULL, "tst_Dict_clear: keys != NULL");
  test_assert_msg(d.vals != NULL, "tst_Dict_clear: vals != NULL");
  test_assert_msg(d.size == 0, "tst_Dict_clear: size == 0");
  
  Dict_del(&d);
}

void tst_Dict_set() {
  struct Dict d;
  Dict_init(&d, 10);

  Dict_set(&d, "hello", "gorgeous");

  test_assert_msg(d.size == 1, "tst_Dict_set 01: size == 1");
  test_assert_msg(d.collisions == 0, "tst_Dict_set 01: collisions == 0");
  test_assert_msg(Dict_contains(&d, "hello"), "tst_Dict_set 01: 'hello' in d");
  test_assert_msg(!Dict_contains(&d, "bye"),
      "tst_Dict_set 01: 'bye' not in d");
  test_assert_msg(strequal(Dict_at(&d, "hello"), "gorgeous"),
      "tst_Dict_set 01: d['hello'] == 'gorgeous'");
  test_assert_msg(Dict_at(&d, "bye") == NULL,
      "tst_Dict_set 01: d['bye'] == NULL");

  Dict_set(&d, "hello", "beautiful");

  test_assert_msg(d.size == 1, "tst_Dict_set 02: size == 1");
  test_assert_msg(d.collisions == 0, "tst_Dict_set 02: collisions == 0");
  test_assert_msg(Dict_contains(&d, "hello"), "tst_Dict_set 02: 'hello' in d");
  test_assert_msg(!Dict_contains(&d, "bye"),
      "tst_Dict_set 02: 'bye' not in d");
  test_assert_msg(strequal(Dict_at(&d, "hello"), "beautiful"),
      "tst_Dict_set 02: d['hello'] == 'beautiful'");
  test_assert_msg(Dict_at(&d, "bye") == NULL,
      "tst_Dict_set 02: d['bye'] == NULL");

  Dict_set(&d, "bye", "sweetheart");

  test_assert_msg(d.size == 2, "tst_Dict_set 03: size == 2");
  test_assert_msg(d.collisions <= 1, "tst_Dict_set 03: collisions <= 1");
  test_assert_msg(Dict_contains(&d, "hello"), "tst_Dict_set 03: 'hello' in d");
  test_assert_msg(Dict_contains(&d, "bye"), "tst_Dict_set 03: 'bye' in d");
  test_assert_msg(strequal(Dict_at(&d, "hello"), "beautiful"),
      "tst_Dict_set 03: d['hello'] == 'beautiful'");
  test_assert_msg(strequal(Dict_at(&d, "bye"), "sweetheart"),
      "tst_Dict_set 03: d['bye'] == 'sweetheart'");
  
  Dict_del(&d);
}

void tst_Dict_contains() {
  // tested in tst_Dict_set()
}

void tst_Dict_at() {
  // tested in tst_Dict_set()
}


int main(void) {
  tst_Dict_init();
  tst_Dict_resize();
  tst_Dict_del();
  tst_Dict_clear();
  tst_Dict_set();
  tst_Dict_contains();
  tst_Dict_at();
  return testval;
}
