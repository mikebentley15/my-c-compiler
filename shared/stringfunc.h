#ifndef STRINGFUNC_H
#define STRINGFUNC_H

#include "bootstrap.h"

bool strequal(const char* a, const char* b);
bool strgreater(const char* a, const char* b);
int strcompare(const char* a, const char* b);

char* to_lowercase(char* str);
char* to_uppercase(char* str);

#endif // STRINGFUNC_H
