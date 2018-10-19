#include "bootstrap.h"
#include "charstream.c"

int tst_CharStream_init() {
  int retval = 1;
  return retval;
}

int tst_CharStream_init_string() {
  int retval = 1;
  return retval;
}

int tst_CharStream_close() {
  int retval = 1;
  return retval;
}

int tst_CharStream_is_eof() {
  int retval = 1;
  return retval;
}

int tst_CharStream_readchar() {
  int retval = 1;
  return retval;
}

int main(void) {
  int retval = 0;
  retval = retval + tst_CharStream_init();
  retval = retval + tst_CharStream_init_string();
  retval = retval + tst_CharStream_close();
  retval = retval + tst_CharStream_is_eof();
  retval = retval + tst_CharStream_readchar();
  return retval;
}
