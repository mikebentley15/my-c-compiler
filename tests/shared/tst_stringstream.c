#include "stringstream.c"
#include "test_harness.c"

#include <stddef.h> // for NULL

void tst_SS_init() {
  struct StringStream ss;
  const char* content = "hello there";

  SS_init(&ss, content);
  test_assert_msg(ss.is_init, "tst_SS_init: is_init == true");
  test_assert_msg(ss.content == content, "tst_SS_init: content == content");
  test_assert_msg(ss.lineno == 1, "tst_SS_init: lineno == 1");
  test_assert_msg(ss.column == 0, "tst_SS_init: column == 0");
  test_assert_msg(ss.index == 0, "tst_SS_init: index == 0");

  SS_close(&ss);
}

void tst_SS_close() {
  struct StringStream ss;
  const char* content = "hello there";
  SS_init(&ss, content);

  ss.lineno = 15;
  ss.column = 25;
  ss.index = 107;

  SS_close(&ss);
  test_assert_msg(!ss.is_init, "tst_SS_close: is_init == false");
  test_assert_msg(ss.content == NULL, "tst_SS_close: content == NULL");
  test_assert_msg(ss.lineno == 0, "tst_SS_close: lineno == 0");
  test_assert_msg(ss.column == 0, "tst_SS_close: column == 0");
  test_assert_msg(ss.index == 0, "tst_SS_close: index == 0");
}

void tst_SS_is_eof() {
  struct StringStream ss;
  const char* content = "A";
  bool is_eof = false;
  SS_init(&ss, content);

  is_eof = SS_is_eof(&ss);
  test_assert_msg(!is_eof, "tst_SS_is_eof before: false");

  // check to make sure no other side effects
  test_assert_msg(ss.is_init, "tst_SS_is_eof before: is_init == true");
  test_assert_msg(ss.content == content,
      "tst_SS_is_eof before: content == content");
  test_assert_msg(ss.lineno == 1, "tst_SS_is_eof before: lineno == 1");
  test_assert_msg(ss.column == 0, "tst_SS_is_eof before: column == 0");
  test_assert_msg(ss.index == 0, "tst_SS_is_eof before: index == 0");

  ss.index = 1;
  is_eof = SS_is_eof(&ss);
  test_assert_msg(is_eof, "tst_SS_is_eof after: true");

  // check to make sure no other side effects
  test_assert_msg(ss.is_init, "tst_SS_is_eof after: is_init == true");
  test_assert_msg(ss.content == content,
      "tst_SS_is_eof after: content == content");
  test_assert_msg(ss.lineno == 1, "tst_SS_is_eof after: lineno == 1");
  test_assert_msg(ss.column == 0, "tst_SS_is_eof after: column == 0");
  test_assert_msg(ss.index == 1, "tst_SS_is_eof after: index == 1");

  SS_close(&ss);
}

void tst_SS_get_char() {
  struct StringStream ss;
  const char* content = "Abc\ndEf\n";
  bool is_eof = false;
  char ch = '\0';
  SS_init(&ss, content);

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(!is_eof, "tst_SS_get_char ts 01: is_eof == false");
  test_assert_msg(ch == 'A',  "tst_SS_get_char ts 01: ch == 'A'");
  test_assert_msg(ss.lineno == 1, "tst_SS_get_char ts 01: lineno == 1");
  test_assert_msg(ss.column == 1, "tst_SS_get_char ts 01: column == 1");
  test_assert_msg(ss.index == 1, "tst_SS_get_char ts 01: index == 1");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 01: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(!is_eof, "tst_SS_get_char ts 02: is_eof == false");
  test_assert_msg(ch == 'b',  "tst_SS_get_char ts 02: ch == 'b'");
  test_assert_msg(ss.lineno == 1, "tst_SS_get_char ts 02: lineno == 1");
  test_assert_msg(ss.column == 2, "tst_SS_get_char ts 02: column == 2");
  test_assert_msg(ss.index == 2, "tst_SS_get_char ts 02: index == 2");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 02: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(!is_eof, "tst_SS_get_char ts 03: is_eof == false");
  test_assert_msg(ch == 'c',  "tst_SS_get_char ts 03: ch == 'c'");
  test_assert_msg(ss.lineno == 1, "tst_SS_get_char ts 03: lineno == 1");
  test_assert_msg(ss.column == 3, "tst_SS_get_char ts 03: column == 3");
  test_assert_msg(ss.index == 3, "tst_SS_get_char ts 03: index == 3");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 03: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(!is_eof, "tst_SS_get_char ts 04: is_eof == false");
  test_assert_msg(ch == '\n', "tst_SS_get_char ts 04: ch == '\\n'");
  test_assert_msg(ss.lineno == 2, "tst_SS_get_char ts 04: lineno == 2");
  test_assert_msg(ss.column == 0, "tst_SS_get_char ts 04: column == 0");
  test_assert_msg(ss.index == 4, "tst_SS_get_char ts 04: index == 4");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 04: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(!is_eof, "tst_SS_get_char ts 05: is_eof == false");
  test_assert_msg(ch == 'd',  "tst_SS_get_char ts 05: ch == 'd'");
  test_assert_msg(ss.lineno == 2, "tst_SS_get_char ts 05: lineno == 2");
  test_assert_msg(ss.column == 1, "tst_SS_get_char ts 05: column == 1");
  test_assert_msg(ss.index == 5, "tst_SS_get_char ts 05: index == 5");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 05: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(!is_eof, "tst_SS_get_char ts 06: is_eof == false");
  test_assert_msg(ch == 'E',  "tst_SS_get_char ts 06: ch == 'E'");
  test_assert_msg(ss.lineno == 2, "tst_SS_get_char ts 06: lineno == 2");
  test_assert_msg(ss.column == 2, "tst_SS_get_char ts 06: column == 2");
  test_assert_msg(ss.index == 6, "tst_SS_get_char ts 06: index == 6");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 06: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(!is_eof, "tst_SS_get_char ts 07: is_eof == false");
  test_assert_msg(ch == 'f',  "tst_SS_get_char ts 07: ch == 'f'");
  test_assert_msg(ss.lineno == 2, "tst_SS_get_char ts 07: lineno == 2");
  test_assert_msg(ss.column == 3, "tst_SS_get_char ts 07: column == 3");
  test_assert_msg(ss.index == 7, "tst_SS_get_char ts 07: index == 7");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 07: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(!is_eof, "tst_SS_get_char ts 08: is_eof == false");
  test_assert_msg(ch == '\n', "tst_SS_get_char ts 08: ch == '\\n'");
  test_assert_msg(ss.lineno == 3, "tst_SS_get_char ts 08: lineno == 3");
  test_assert_msg(ss.column == 0, "tst_SS_get_char ts 08: column == 0");
  test_assert_msg(ss.index == 8, "tst_SS_get_char ts 08: index == 8");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 08: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(is_eof, "tst_SS_get_char ts 09: is_eof == true");
  test_assert_msg(ch == '\0', "tst_SS_get_char ts 09: ch == '\\0'");
  test_assert_msg(ss.lineno == 3, "tst_SS_get_char ts 09: lineno == 3");
  test_assert_msg(ss.column == 0, "tst_SS_get_char ts 09: column == 0");
  test_assert_msg(ss.index == 8, "tst_SS_get_char ts 09: index == 8");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 09: is_init == true");

  is_eof = SS_is_eof(&ss);
  ch = SS_get_char(&ss);
  test_assert_msg(is_eof, "tst_SS_get_char ts 10: is_eof == true");
  test_assert_msg(ch == '\0', "tst_SS_get_char ts 10: ch == '\\0'");
  test_assert_msg(ss.lineno == 3, "tst_SS_get_char ts 10: lineno == 3");
  test_assert_msg(ss.column == 0, "tst_SS_get_char ts 10: column == 0");
  test_assert_msg(ss.index == 8, "tst_SS_get_char ts 10: index == 8");
  test_assert_msg(ss.is_init == true, "tst_SS_get_char ts 10: is_init == true");

  SS_close(&ss);
}

#include "charstream.c"
#include "stringfunc.c"

void tst_SS_to_CharStream() {
  struct StringStream ss;
  const char* content = "Abc\ndEf\ngh";
  bool is_eof = false;
  char ch = '\0';
  int lineno = 0;
  int column = 0;
  SS_init(&ss, content);

  struct CharStream cs = SS_to_CharStream(&ss);
  test_assert_msg(cs.is_init, "tst_SS_to_CharStream begin: is_init == true");

  ss.lineno = 5;
  lineno = CS_get_lineno(&cs);
  test_assert_msg(lineno == 5, "tst_SS_to_CharStream: lineno == 5");

  ss.column = 23;
  column = CS_get_column(&cs);
  test_assert_msg(column == 23, "tst_SS_to_CharStream: column == 23");

  ss.index = 5;
  is_eof = CS_is_eof(&cs);
  ch = CS_get_char(&cs);
  test_assert_msg(!is_eof, "tst_SS_to_CharStream 01: is_eof == false");
  test_assert_msg(ch == 'E', "tst_SS_to_CharStream 01: ch == 'E'");
  test_assert_msg(ss.index == 6, "tst_SS_to_CharStream 01: index == 6");

  ss.index = 10;
  is_eof = CS_is_eof(&cs);
  ch = CS_get_char(&cs);
  test_assert_msg(is_eof, "tst_SS_to_CharStream 02: is_eof == true");
  test_assert_msg(ch == '\0', "tst_SS_to_CharStream 02: ch == '\\0'");
  test_assert_msg(ss.index == 10, "tst_SS_to_CharStream 02: index == 10");

  const char* filepath = CS_get_filepath(&cs);
  test_assert_msg(strequal(filepath, "<content>"),
      "tst_SS_to_CharStream: filepath == \"<content>\"");

  CS_close(&cs);

  test_assert_msg(!cs.is_init,
      "tst_SS_to_CharStream close: cs.is_init == false");
  test_assert_msg(cs.arg == NULL,
      "tst_SS_to_CharStream close: cs.arg == NULL");
  test_assert_msg(cs.get_char == NULL,
      "tst_SS_to_CharStream close: cs.get_char == NULL");
  test_assert_msg(cs.get_lineno == NULL,
      "tst_SS_to_CharStream close: cs.get_lineno == NULL");
  test_assert_msg(cs.get_column == NULL,
      "tst_SS_to_CharStream close: cs.get_column == NULL");
  test_assert_msg(cs.get_filepath == NULL,
      "tst_SS_to_CharStream close: cs.get_filepath == NULL");
  test_assert_msg(cs.is_eof == NULL,
      "tst_SS_to_CharStream close: cs.is_eof == NULL");
  test_assert_msg(cs.close == NULL,
      "tst_SS_to_CharStream close: cs.close == NULL");

  test_assert_msg(!ss.is_init,
      "tst_SS_to_CharStream close: ss.is_init == false");
  test_assert_msg(ss.content == NULL,
      "tst_SS_to_CharStream close: ss.content == NULL");
  test_assert_msg(ss.lineno == 0,
      "tst_SS_to_CharStream close: ss.lineno == 0");
  test_assert_msg(ss.column == 0,
      "tst_SS_to_CharStream close: ss.column == 0");
  test_assert_msg(ss.index == 0,
      "tst_SS_to_CharStream close: ss.index == 0");
}

int main(void) {
  tst_SS_init();
  tst_SS_close();
  tst_SS_is_eof();
  tst_SS_get_char();
  tst_SS_to_CharStream();
  return testval;
}

