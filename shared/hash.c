#include "hash.h"

unsigned long djb2_hash(const char* str) {
  unsigned long hash = 5381;
  int i = 0;

  while (str[i] != '\0') {
    hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    i++;
  }

  return hash; 
}

