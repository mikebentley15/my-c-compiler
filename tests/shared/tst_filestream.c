#include "filestream.c"
#include "test_harness.c"

#include <stddef.h> // for NULL
#include <stdio.h>  // for fputc()

const char* test_file = "filestream_input.txt";

void tst_FS_init() {
  struct FileStream fs;
  FS_init(&fs, test_file);

  test_assert_msg(fs.is_init, "tst_FS_init: is_init == true");
  test_assert_msg(fs.file != NULL, "tst_FS_init: file != NULL");
  test_assert_msg(fs.filepath == test_file,
      "tst_FS_init: filepath != test_file");
  test_assert_msg(fs.lineno == 1, "tst_FS_init: lineno == 1");
  test_assert_msg(fs.column == 0, "tst_FS_init: column == 0");

  FS_close(&fs);
}

void tst_FS_close() {
  struct FileStream fs;
  FS_init(&fs, test_file);

  fs.lineno = 15;
  fs.column = 25;

  FS_close(&fs);
  test_assert_msg(!fs.is_init, "tst_FS_close: is_init == false");
  test_assert_msg(fs.file == NULL, "tst_FS_close: file == NULL");
  test_assert_msg(fs.filepath == NULL, "tst_FS_close: filepath == NULL");
  test_assert_msg(fs.lineno == 0, "tst_FS_close: lineno == 0");
  test_assert_msg(fs.column == 0, "tst_FS_close: column == 0");
}

void tst_FS_is_eof() {
  struct FileStream fs;
  bool is_eof = false;
  FS_init(&fs, test_file);

  is_eof = FS_is_eof(&fs);
  test_assert_msg(!is_eof, "tst_FS_is_eof before: false");

  // check to make sure no other side effects
  test_assert_msg(fs.is_init, "tst_FS_is_eof before: is_init == true");
  test_assert_msg(fs.file != NULL, "tst_FS_is_eof before: file != NULL");
  test_assert_msg(fs.filepath == test_file,
      "tst_FS_is_eof before: filepath == test_file");
  test_assert_msg(fs.lineno == 1, "tst_FS_is_eof before: lineno == 1");
  test_assert_msg(fs.column == 0, "tst_FS_is_eof before: column == 0");

  while (!feof(fs.file)) {
    fgetc(fs.file);
  }
  is_eof = FS_is_eof(&fs);
  test_assert_msg(is_eof, "tst_FS_is_eof after: true");

  // check to make sure no other side effects
  test_assert_msg(fs.is_init, "tst_FS_is_eof after: is_init == true");
  test_assert_msg(fs.file != NULL, "tst_FS_is_eof after: file != NULL");
  test_assert_msg(fs.filepath == test_file,
      "tst_FS_is_eof after: filepath == test_file");
  test_assert_msg(fs.lineno == 1, "tst_FS_is_eof after: lineno == 1");
  test_assert_msg(fs.column == 0, "tst_FS_is_eof after: column == 0");

  FS_close(&fs);
}

void tst_FS_get_char() {
  struct FileStream fs;
  bool is_eof = false;
  char ch = '\0';
  FS_init(&fs, test_file);

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(!is_eof, "tst_FS_get_char ts 01: is_eof == false");
  test_assert_msg(ch == 'A',  "tst_FS_get_char ts 01: ch == 'A'");
  test_assert_msg(fs.lineno == 1, "tst_FS_get_char ts 01: lineno == 1");
  test_assert_msg(fs.column == 1, "tst_FS_get_char ts 01: column == 1");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 01: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(!is_eof, "tst_FS_get_char ts 02: is_eof == false");
  test_assert_msg(ch == 'b',  "tst_FS_get_char ts 02: ch == 'b'");
  test_assert_msg(fs.lineno == 1, "tst_FS_get_char ts 02: lineno == 1");
  test_assert_msg(fs.column == 2, "tst_FS_get_char ts 02: column == 2");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 02: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(!is_eof, "tst_FS_get_char ts 03: is_eof == false");
  test_assert_msg(ch == 'c',  "tst_FS_get_char ts 03: ch == 'c'");
  test_assert_msg(fs.lineno == 1, "tst_FS_get_char ts 03: lineno == 1");
  test_assert_msg(fs.column == 3, "tst_FS_get_char ts 03: column == 3");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 03: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(!is_eof, "tst_FS_get_char ts 04: is_eof == false");
  test_assert_msg(ch == '\n', "tst_FS_get_char ts 04: ch == '\\n'");
  test_assert_msg(fs.lineno == 2, "tst_FS_get_char ts 04: lineno == 2");
  test_assert_msg(fs.column == 0, "tst_FS_get_char ts 04: column == 0");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 04: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(!is_eof, "tst_FS_get_char ts 05: is_eof == false");
  test_assert_msg(ch == 'd',  "tst_FS_get_char ts 05: ch == 'd'");
  test_assert_msg(fs.lineno == 2, "tst_FS_get_char ts 05: lineno == 2");
  test_assert_msg(fs.column == 1, "tst_FS_get_char ts 05: column == 1");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 05: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(!is_eof, "tst_FS_get_char ts 06: is_eof == false");
  test_assert_msg(ch == 'E',  "tst_FS_get_char ts 06: ch == 'E'");
  test_assert_msg(fs.lineno == 2, "tst_FS_get_char ts 06: lineno == 2");
  test_assert_msg(fs.column == 2, "tst_FS_get_char ts 06: column == 2");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 06: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(!is_eof, "tst_FS_get_char ts 07: is_eof == false");
  test_assert_msg(ch == 'f',  "tst_FS_get_char ts 07: ch == 'f'");
  test_assert_msg(fs.lineno == 2, "tst_FS_get_char ts 07: lineno == 2");
  test_assert_msg(fs.column == 3, "tst_FS_get_char ts 07: column == 3");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 07: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(!is_eof, "tst_FS_get_char ts 08: is_eof == false");
  test_assert_msg(ch == '\n', "tst_FS_get_char ts 08: ch == '\\n'");
  test_assert_msg(fs.lineno == 3, "tst_FS_get_char ts 08: lineno == 3");
  test_assert_msg(fs.column == 0, "tst_FS_get_char ts 08: column == 0");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 08: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(is_eof, "tst_FS_get_char ts 09: is_eof == true");
  test_assert_msg(ch == '\0', "tst_FS_get_char ts 09: ch == '\\0'");
  fputc(ch, stdout);
  test_assert_msg(fs.lineno == 3, "tst_FS_get_char ts 09: lineno == 3");
  test_assert_msg(fs.column == 0, "tst_FS_get_char ts 09: column == 0");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 09: is_init == true");

  is_eof = FS_is_eof(&fs);
  ch = FS_get_char(&fs);
  test_assert_msg(is_eof, "tst_FS_get_char ts 10: is_eof == true");
  test_assert_msg(ch == '\0', "tst_FS_get_char ts 10: ch == '\\0'");
  test_assert_msg(fs.lineno == 3, "tst_FS_get_char ts 10: lineno == 3");
  test_assert_msg(fs.column == 0, "tst_FS_get_char ts 10: column == 0");
  test_assert_msg(fs.is_init == true, "tst_FS_get_char ts 10: is_init == true");

  FS_close(&fs);
}

#include "charstream.c"
#include "stringfunc.c"

void tst_FS_to_CharStream() {
  struct FileStream fs;
  bool is_eof = false;
  char ch = '\0';
  int lineno = 0;
  int column = 0;
  FS_init(&fs, test_file);

  struct CharStream cs = FS_to_CharStream(&fs);
  test_assert_msg(cs.is_init, "tst_FS_to_CharStream begin: is_init == true");

  fs.lineno = 5;
  lineno = CS_get_lineno(&cs);
  test_assert_msg(lineno == 5, "tst_FS_to_CharStream: lineno == 5");

  fs.column = 23;
  column = CS_get_column(&cs);
  test_assert_msg(column == 23, "tst_FS_to_CharStream: column == 23");

  fs.lineno = 1;
  fs.column = 0;

  const char* contents = "Abc\ndEf\n";
  int i = 0;
  while (i < 8) {
    is_eof = CS_is_eof(&cs);
    ch = CS_get_char(&cs);
    test_assert_msg(!is_eof, "tst_FS_to_CharStream 01: is_eof == false");
    test_assert_msg(ch == contents[i],
        "tst_FS_to_CharStream 01: ch == contents[i]");
    i++;
  }

  is_eof = CS_is_eof(&cs);
  ch = CS_get_char(&cs);
  test_assert_msg(is_eof, "tst_FS_to_CharStream 02: is_eof == true");
  test_assert_msg(ch == '\0', "tst_FS_to_CharStream 02: ch == '\\0'");

  const char* filepath = CS_get_filepath(&cs);
  test_assert_msg(filepath == test_file,
      "tst_FS_to_CharStream: filepath == test_file");

  CS_close(&cs);

  test_assert_msg(!cs.is_init,
      "tst_FS_to_CharStream close: cs.is_init == false");
  test_assert_msg(cs.arg == NULL,
      "tst_FS_to_CharStream close: cs.arg == NULL");
  test_assert_msg(cs.get_char == NULL,
      "tst_FS_to_CharStream close: cs.get_char == NULL");
  test_assert_msg(cs.get_lineno == NULL,
      "tst_FS_to_CharStream close: cs.get_lineno == NULL");
  test_assert_msg(cs.get_column == NULL,
      "tst_FS_to_CharStream close: cs.get_column == NULL");
  test_assert_msg(cs.get_filepath == NULL,
      "tst_FS_to_CharStream close: cs.get_filepath == NULL");
  test_assert_msg(cs.is_eof == NULL,
      "tst_FS_to_CharStream close: cs.is_eof == NULL");
  test_assert_msg(cs.close == NULL,
      "tst_FS_to_CharStream close: cs.close == NULL");

  test_assert_msg(!fs.is_init,
      "tst_FS_to_CharStream close: fs.is_init == false");
  test_assert_msg(fs.file == NULL,
      "tst_FS_to_CharStream close: fs.file == NULL");
  test_assert_msg(fs.filepath == NULL,
      "tst_FS_to_CharStream close: fs.filepath == NULL");
  test_assert_msg(fs.lineno == 0,
      "tst_FS_to_CharStream close: fs.lineno == 0");
  test_assert_msg(fs.column == 0,
      "tst_FS_to_CharStream close: fs.column == 0");
}

int main(void) {
  tst_FS_init();
  tst_FS_close();
  tst_FS_is_eof();
  tst_FS_get_char();
  tst_FS_to_CharStream();
  return testval;
}

