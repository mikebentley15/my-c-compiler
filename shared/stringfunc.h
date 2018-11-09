#ifndef STRINGFUNC_H
#define STRINGFUNC_H

#include "bootstrap.h"

bool is_whitespace(char ch);
bool is_digit(char ch);
bool is_letter(char ch);

bool strequal(const char* a, const char* b);
bool strgreater(const char* a, const char* b);
int strcompare(const char* a, const char* b);

int   strlen(const char* a);
char* strdup(const char* a);

char* to_lowercase(char* str);
char* to_uppercase(char* str);

void* memcopy(void* dest, const void* source, int nbytes);
void* memfill(void* dest, int nbytes, char val);
void* memzero(void* dest, int nbytes);

#endif // STRINGFUNC_H
