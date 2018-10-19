#ifndef TOKEN_H
#define TOKEN_H

// the token type
struct Token {
  int type;
  char* value;
};

// a global token
extern Token g_tok;

enum TokenType {
  SEMICOLON,
  COLON,
  LPAREN,
  RPAREN,
  PLUS,
  MINUS,
  ASSIGNMENT,
  EQUAL,
  LESS_OR_EQ,
  GREATER_OR_EQ,
  LCURLY,
  RCURLY,
};

#endif // TOKEN_H
