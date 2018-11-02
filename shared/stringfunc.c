#include "stringfunc.h"

#include <stddef.h> // for NULL

bool strequal(const char* a, const char* b) {
  return strcompare(a, b) == 0;
}

bool strgreater(const char* a, const char* b) {
  return strcompare(a, b) > 0;
}

// returns
// - negative if a < b
// - zero     if a == b
// - positive if a > b
int strcompare(const char* a, const char* b) {
  int comparison = 0;
  if (a == b) {
    comparison = 0;
  } else if (a == NULL) {
    comparison = -1;
  } else if (b == NULL) {
    comparison = 1;
  } else {
    int j = 0;
    while (a[j] == b[j] && a[j] != '\0' && b[j] != '\0') {
      j++;
    }
    comparison = a[j] - b[j];
  }
  return comparison;
}

char* to_lowercase(char* str) {
  int i = 0;
  while (str[i] != '\0') {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] = str[i] - 'A' + 'a';
    }
    i++;
  }
  return str;
}

char* to_uppercase(char* str) {
  int i = 0;
  while (str[i] != '\0') {
    if (str[i] >= 'a' && str[i] <= 'z') {
      str[i] = str[i] - 'a' + 'A';
    }
    i++;
  }
  return str;
}
