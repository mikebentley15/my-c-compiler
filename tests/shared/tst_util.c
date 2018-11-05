#include "util.c"
#include "test_harness.c"

void tst_error() {
  // no clean way to test this without POSIX headers
}

void tst_is_prime() {
  test_assert_msg(!is_prime(  1),   "1 is not prime");
  test_assert_msg( is_prime(  2),   "2 is prime");
  test_assert_msg( is_prime(  3),   "3 is prime");
  test_assert_msg(!is_prime(  4),   "4 is not prime");
  test_assert_msg( is_prime(  5),   "5 is prime");
  test_assert_msg(!is_prime(  6),   "6 is not prime");
  test_assert_msg( is_prime(  7),   "7 is prime");
  test_assert_msg(!is_prime(  8),   "8 is not prime");
  test_assert_msg(!is_prime(  9),   "9 is not prime");
  test_assert_msg(!is_prime( 10),  "10 is not prime");
  test_assert_msg( is_prime( 11),  "11 is prime");
  test_assert_msg(!is_prime( 12),  "12 is not prime");
  test_assert_msg( is_prime( 13),  "13 is prime");
  test_assert_msg(!is_prime(111), "111 is not prime");
  test_assert_msg( is_prime(113), "113 is prime");
}

void tst_next_prime() {
  test_assert_msg(next_prime(  1) ==   2, "next_prime(  1) ==   2");
  test_assert_msg(next_prime(  2) ==   3, "next_prime(  2) ==   3");
  test_assert_msg(next_prime(  3) ==   5, "next_prime(  3) ==   5");
  test_assert_msg(next_prime(  4) ==   5, "next_prime(  4) ==   5");
  test_assert_msg(next_prime(  5) ==   7, "next_prime(  5) ==   7");
  test_assert_msg(next_prime(  6) ==   7, "next_prime(  6) ==   7");
  test_assert_msg(next_prime(  7) ==  11, "next_prime(  7) ==  11");
  test_assert_msg(next_prime(  8) ==  11, "next_prime(  8) ==  11");
  test_assert_msg(next_prime(  9) ==  11, "next_prime(  9) ==  11");
  test_assert_msg(next_prime( 10) ==  11, "next_prime( 10) ==  11");
  test_assert_msg(next_prime( 11) ==  13, "next_prime( 11) ==  13");
  test_assert_msg(next_prime( 12) ==  13, "next_prime( 12) ==  13");
  test_assert_msg(next_prime( 13) ==  17, "next_prime( 13) ==  17");
  test_assert_msg(next_prime(111) == 113, "next_prime(111) == 113");
  test_assert_msg(next_prime(113) == 127, "next_prime(113) == 127");
}

int main(void) {
  tst_error();
  tst_is_prime();
  tst_next_prime();
  return testval;
}

