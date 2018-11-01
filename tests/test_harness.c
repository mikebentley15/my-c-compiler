#include "bootstrap.h"

#include <stdio.h>  // for fputs() and fputc()

int testval = 0;

void test_assert_msg(bool expr, const char* msg) {
  if (!expr) {
    testval++;
    fputs("Assertion Failure: ", stderr);
    fputs(msg, stderr);
    fputc('\n', stderr);
  }
}
