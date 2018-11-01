#include "stringfunc.h"

#include <stddef.h> // for NULL

bool strequal(const char* a, const char* b) {
  bool are_equal = false;
  if (a == b) {
    are_equal = true;
  } else if (a == NULL || b == NULL) {
    are_equal = false;
  } else {
    int j = 0;
    int k = 0;
    while (a[j] == b[k] && a[j] != '\0' && b[k] != '\0') {
      j++;
      k++;
    }
    are_equal = (a[j] == b[k]);
  }
  return are_equal;
}
