#ifndef TOKEN_H
#define TOKEN_H

enum TokenType {
  TOKEN_TYPE_ERROR = 0,
  TOKEN_TYPE_SEMICOLON = 1,
  TOKEN_TYPE_COLON = 2,
  TOKEN_TYPE_LPAREN = 3,
  TOKEN_TYPE_RPAREN = 4,
  TOKEN_TYPE_PLUS = 5,
  TOKEN_TYPE_MINUS = 6,
  TOKEN_TYPE_ASSIGNMENT = 7,
  TOKEN_TYPE_EQUAL = 8,
  TOKEN_TYPE_LESS_OR_EQ = 9,
  TOKEN_TYPE_GREATER_OR_EQ = 10,
  TOKEN_TYPE_LCURLY = 11,
  TOKEN_TYPE_RCURLY = 12,
};

// the token type
struct Token {
  enum TokenType type;
  char* value;
};

#endif // TOKEN_H
