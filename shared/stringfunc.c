#include "stringfunc.h"

#include <stddef.h> // for NULL

bool is_whitespace(char ch) {
  return ch == ' ' || ch == '\n' || ch == '\t';
}

bool is_digit(char ch) {
  return ch >= '0' && ch <= '9';
}

bool is_letter(char ch) {
  return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

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

void* memcopy(void* dest, const void* source, int nbytes) {
  char *to   = (char*) dest;
  char *from = (char*) source;
  int i = 0;
  while (i < nbytes) {
    to[i] = from[i];
    i++;
  }
  return dest;
}

void* memfill(void* dest, int nbytes, char val) {
  char* to = (char*) dest;
  int i = 0;
  while (i < nbytes) {
    to[i] = val;
    i++;
  }
  return dest;
}

void* memzero(void* dest, int nbytes) {
  char* to = (char*) dest;
  int i = 0;
  while (i < nbytes) {
    to[i] = '\0';
    i++;
  }
  return dest;
}
