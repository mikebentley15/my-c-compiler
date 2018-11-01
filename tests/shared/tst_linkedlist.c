#include "linkedlist.c"
#include "test_harness.c"

void tst_LL_init() {
  struct LinkedList ll;
  LL_init(&ll);
  test_assert_msg(ll.size == 0, "tst_LL_init: size == 0");
  test_assert_msg(ll.head == NULL, "tst_LL_init: head == NULL");
  test_assert_msg(ll.tail == NULL, "tst_LL_init: tail == NULL");
}

void tst_LL_clear() {
  struct LinkedList ll;
  LL_init(&ll);

  LL_clear(&ll);

  test_assert_msg(ll.size == 0, "tst_LL_clear 01: size == 0");
  test_assert_msg(ll.head == NULL, "tst_LL_clear 01: head == NULL");
  test_assert_msg(ll.tail == NULL, "tst_LL_clear 01: tail == NULL");

  // push some function pointers
  LL_push_head(&ll, &tst_LL_init);
  LL_push_head(&ll, &tst_LL_clear);

  test_assert_msg(ll.size == 2, "tst_LL_clear 02: size == 2");
  test_assert_msg(ll.head != NULL, "tst_LL_clear 02: head != NULL");
  test_assert_msg(ll.tail != NULL, "tst_LL_clear 02: tail != NULL");

  // clear
  LL_clear(&ll);

  test_assert_msg(ll.size == 0, "tst_LL_clear 03: size == 0");
  test_assert_msg(ll.head == NULL, "tst_LL_clear 03: head == NULL");
  test_assert_msg(ll.tail == NULL, "tst_LL_clear 03: tail == NULL");
}

void tst_LL_head() {
  struct LinkedList ll;
  LL_init(&ll);
  void* head = NULL;

  head = LL_head(&ll);
  test_assert_msg(head == NULL, "tst_LL_head 01: head == NULL");

  LL_push_tail(&ll, &tst_LL_head);

  head = LL_head(&ll);
  test_assert_msg(head == &tst_LL_head, "tst_LL_head 02: head == &tst_LL_head");

  LL_clear(&ll);
  LL_push_head(&ll, &ll);

  head = LL_head(&ll);
  test_assert_msg(head == &ll, "tst_LL_head 03: head == &ll");

  LL_push_tail(&ll, &tst_LL_head);

  head = LL_head(&ll);
  test_assert_msg(head == &ll, "tst_LL_head 04: head == &ll");

  LL_push_head(&ll, &LL_init);

  head = LL_head(&ll);
  test_assert_msg(head == &LL_init, "tst_LL_head 05: head == &LL_init");

  LL_clear(&ll);
}

void tst_LL_tail() {
  struct LinkedList ll;
  LL_init(&ll);
  void* tail = NULL;

  tail = LL_tail(&ll);
  test_assert_msg(tail == NULL, "tst_LL_tail 01: tail == NULL");

  LL_push_head(&ll, &tst_LL_tail);

  tail = LL_tail(&ll);
  test_assert_msg(tail == &tst_LL_tail, "tst_LL_tail 02: tail == &tst_LL_tail");

  LL_clear(&ll);
  LL_push_tail(&ll, &ll);

  tail = LL_tail(&ll);
  test_assert_msg(tail == &ll, "tst_LL_tail 03: tail == &ll");

  LL_push_head(&ll, &tst_LL_tail);

  tail = LL_tail(&ll);
  test_assert_msg(tail == &ll, "tst_LL_tail 04: tail == &ll");

  LL_push_tail(&ll, &LL_init);

  tail = LL_tail(&ll);
  test_assert_msg(tail == &LL_init, "tst_LL_tail 05: tail == &LL_init");

  LL_clear(&ll);
}

void tst_LL_pop_head() {
  struct LinkedList ll;
  LL_init(&ll);
  void* head = NULL;

  head = LL_pop_head(&ll);
  test_assert_msg(head == NULL, "tst_LL_pop_head 01: head == NULL");

  LL_push_tail(&ll, &tst_LL_pop_head);

  head = LL_pop_head(&ll);
  test_assert_msg(head == &tst_LL_pop_head,
      "tst_LL_pop_head 02: head == &tst_LL_pop_head");
  test_assert_msg(ll.size == 0, "tst_LL_pop_head 02: size == 0");
  test_assert_msg(ll.head == NULL, "tst_LL_pop_head 02: ll.head == NULL");
  test_assert_msg(ll.tail == NULL, "tst_LL_pop_head 02: ll.tail == NULL");

  LL_push_head(&ll, &ll);

  head = LL_pop_head(&ll);
  test_assert_msg(head == &ll, "tst_LL_pop_head 03: head == &ll");
  test_assert_msg(ll.size == 0, "tst_LL_pop_head 03: size == 0");
  test_assert_msg(ll.head == NULL, "tst_LL_pop_head 03: ll.head == NULL");
  test_assert_msg(ll.tail == NULL, "tst_LL_pop_head 03: ll.tail == NULL");

  LL_push_head(&ll, &ll);
  LL_push_tail(&ll, &tst_LL_pop_head);

  head = LL_pop_head(&ll);
  test_assert_msg(head == &ll, "tst_LL_pop_head 04: head == &ll");
  test_assert_msg(ll.size == 1, "tst_LL_pop_head 04: size == 0");
  test_assert_msg(ll.head == ll.tail, "tst_LL_pop_head 04: ll.head == ll.tail");

  LL_push_head(&ll, &LL_init);
  LL_push_tail(&ll, &LL_pop_tail);

  head = LL_pop_head(&ll);
  test_assert_msg(head == &LL_init, "tst_LL_pop_head 05: head == &LL_init");
  test_assert_msg(ll.size == 2, "tst_LL_pop_head 05: size == 0");
  test_assert_msg(ll.head != NULL, "tst_LL_pop_head 05: ll.head != NULL");
  test_assert_msg(ll.tail != NULL, "tst_LL_pop_head 05: ll.tail != NULL");
  test_assert_msg(ll.head != ll.tail, "tst_LL_pop_head 05: ll.head != ll.tail");

  LL_clear(&ll);
}

void tst_LL_pop_tail() {
  struct LinkedList ll;
  LL_init(&ll);
  void* tail = NULL;

  tail = LL_pop_tail(&ll);
  test_assert_msg(tail == NULL, "tst_LL_pop_tail 01: tail == NULL");

  LL_push_head(&ll, &tst_LL_pop_tail);

  tail = LL_pop_tail(&ll);
  test_assert_msg(tail == &tst_LL_pop_tail,
      "tst_LL_pop_tail 02: tail == &tst_LL_pop_tail");
  test_assert_msg(ll.size == 0, "tst_LL_pop_tail 02: size == 0");
  test_assert_msg(ll.head == NULL, "tst_LL_pop_tail 02: ll.head == NULL");
  test_assert_msg(ll.tail == NULL, "tst_LL_pop_tail 02: ll.tail == NULL");

  LL_push_tail(&ll, &ll);

  tail = LL_pop_tail(&ll);
  test_assert_msg(tail == &ll, "tst_LL_pop_tail 03: tail == &ll");
  test_assert_msg(ll.size == 0, "tst_LL_pop_tail 03: size == 0");
  test_assert_msg(ll.head == NULL, "tst_LL_pop_tail 03: ll.head == NULL");
  test_assert_msg(ll.tail == NULL, "tst_LL_pop_tail 03: ll.tail == NULL");

  LL_push_tail(&ll, &ll);
  LL_push_head(&ll, &tst_LL_pop_tail);

  tail = LL_pop_tail(&ll);
  test_assert_msg(tail == &ll, "tst_LL_pop_tail 04: tail == &ll");
  test_assert_msg(ll.size == 1, "tst_LL_pop_tail 04: size == 0");
  test_assert_msg(ll.head == ll.tail, "tst_LL_pop_tail 04: ll.head == ll.tail");

  LL_push_tail(&ll, &LL_init);
  LL_push_head(&ll, &LL_pop_head);

  tail = LL_pop_tail(&ll);
  test_assert_msg(tail == &LL_init, "tst_LL_pop_tail 05: tail == &LL_init");
  test_assert_msg(ll.size == 2, "tst_LL_pop_tail 05: size == 0");
  test_assert_msg(ll.head != NULL, "tst_LL_pop_tail 05: ll.head != NULL");
  test_assert_msg(ll.tail != NULL, "tst_LL_pop_tail 05: ll.tail != NULL");
  test_assert_msg(ll.head != ll.tail, "tst_LL_pop_tail 05: ll.head != ll.tail");

  LL_clear(&ll);
}

void tst_LL_push_head() {
  struct LinkedList ll;
  LL_init(&ll);

  // was already tested in tst_LL_pop_head() and tst_LL_pop_tail()
}

void tst_LL_push_tail() {
  struct LinkedList ll;
  LL_init(&ll);

  // was already tested in tst_LL_pop_head() and tst_LL_pop_tail()
}

// Haven't yet had a need for these:
//void tst_LL_insert() {}
//void tst_LL_at() {}
//void tst_LL_pop_at() {}

int main(void) {
  tst_LL_init();
  tst_LL_clear();
  tst_LL_head();
  tst_LL_tail();
  tst_LL_pop_head();
  tst_LL_pop_tail();
  tst_LL_push_head();
  tst_LL_push_tail();
  //tst_LL_insert();
  //tst_LL_at();
  //tst_LL_pop_at();
  return testval;
}
