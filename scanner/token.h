#ifndef TOKEN_H
#define TOKEN_H

#include "bootstrap.h"

enum TokenType {
  TT_ERROR = 0,                    // not a valid token (default value)

  // All keywords
  TT_KEYWORD_BEGIN,

  // All types
  TT_TYPES_BEGIN,
  TT_BOOL,                         // bool
  TT_CHAR,                         // char
  TT_INT,                          // int
  TT_VOID,                         // void
  TT_ENUM,                         // enum
  TT_STRUCT,                       // struct
  TT_TYPES_END,

  TT_BREAK,                        // break
  TT_CONST,                        // const
  TT_CONTINUE,                     // continue
  TT_ELSE,                         // else
  TT_IF,                           // if
  TT_RETURN,                       // return
  TT_SIZEOF,                       // sizeof
  TT_TYPEDEF,                      // typedef
  TT_WHILE,                        // while

  TT_KEYWORD_UNSUPPORTED_BEGIN,
  TT_PACKED,                       // _Packed
  TT_AND,                          // and
  TT_AUTO,                         // auto
  TT_BYTE,                         // byte
  TT_CASE,                         // case
  TT_DEFAULT,                      // default
  TT_DO,                           // do
  TT_DOUBLE,                       // double
  TT_EXTERN,                       // extern
  TT_FLOAT,                        // float
  TT_FOR,                          // for
  TT_GOTO,                         // goto
  TT_LONG,                         // long
  TT_OR,                           // or
  TT_REGISTER,                     // register
  TT_SHORT,                        // short
  TT_SIGNED,                       // signed
  TT_STATIC,                       // static
  TT_SWITCH,                       // switch
  TT_UNION,                        // union
  TT_UNSIGNED,                     // unsigned
  TT_VOLATILE,                     // volatile
  TT_KEYWORD_UNSUPPORTED_END,
  
  TT_KEYWORD_END,

  // Preprocessing tokens
  TT_PREPROCESSING_BEGIN,

  TT_POUND,                        // '#' symbol
  TT_PP_INCLUDE,                   // #include
  TT_PP_IFDEF,                     // #ifdef
  TT_PP_IFNDEF,                    // #ifndef
  TT_PP_ELSE,                      // #else
  TT_PP_ENDIF,                     // #endif
  TT_PP_DEFINE,                    // #define

  TT_PP_UNSUPPORTED_BEGIN,
  TT_PP_PRAGMA,                    // #pragma
  TT_PP_UNDEF,                     // #undef
  TT_PP_UNSUPPORTED_END,

  TT_PREPROCESSING_END,

  // All operators
  TT_OPERATOR_BEGIN,
  
  TT_PLUS,                         // +
  TT_MINUS,                        // -
  TT_MULTIPLY,                     // * (also the dereference)
  TT_DEREFERENCE = TT_MULTIPLY,    // *
  TT_DIVIDE,                       // /
  TT_MODULUS,                      // %
  TT_LPAREN,                       // (
  TT_RPAREN,                       // )
  TT_DECREMENT,                    // --
  TT_INCREMENT,                    // ++

  TT_EQUALS,                       // =
  TT_EQUALITY,                     // ==
  TT_NOT_EQUAL,                    // !=
  TT_LESS,                         // <
  TT_GREATER,                      // >
  TT_LESS_EQUAL,                   // <=
  TT_GREATER_EQUAL,                // >=
  TT_BOOLEAN_AND,                  // &&
  TT_BOOLEAN_OR,                   // ||
  TT_BOOLEAN_NOT,                  // !

  TT_BITSHIFT_RIGHT,               // >>
  TT_BITSHIFT_LEFT,                // <<
  TT_BITWISE_XOR,                  // ^
  TT_BITWISE_AND,                  // & (also the address of operator)
  TT_ADDRESS_OF = TT_BITWISE_AND,  // &
  TT_BITWISE_OR,                   // |
  TT_BITWISE_INVERT,               // ~

  TT_DOT,                          // .
  TT_ARROW,                        // ->

  TT_UNSUPPORTED_OPERATORS_BEGIN,
  TT_COLON,                        // :
  TT_QUESTION,                     // ?
  TT_PLUS_EQUALS,                  // +=
  TT_MINUS_EQUALS,                 // -=
  TT_MULTIPLY_EQUALS,              // *=
  TT_DIVIDE_EQUALS,                // /=
  TT_MODULUS_EQUALS,               // %=
  TT_BITSHIFT_LEFT_EQUALS,         // <<=
  TT_BITSHIFT_RIGHT_EQUALS,        // >>=
  TT_BITWISE_AND_EQUALS,           // &=
  TT_BITWISE_OR_EQUALS,            // |=
  TT_BITWISE_XOR_EQUALS,           // ^=
  TT_UNSUPPORTED_OPERATORS_END,

  TT_IDENTIFIER,                   // ("_" | letter) {"_" | letter | digit}

  TT_LITERAL_VALUES_BEGIN,
  TT_INT_LITERAL,                  // "0"|nonzeroDigit{digit}
  TT_STRING_LITERAL,               // """ {printableCharacter | "\'" | newline} """
  TT_CHARACTER_LITERAL,            // "'" (printableCharacter | """) "'"
  TT_TRUE_LITERAL,                 // true
  TT_FALSE_LITERAL,                // false
  TT_LITERAL_VALUES_END,

  TT_OPERATOR_END,


  TT_OTHER_BEGIN,                  // not keyword, not operator, just other...

  TT_COMMENT_BEGIN,
  // comments
  TT_LINE_COMMENT,                 // "//" {printableCharacter} newline
  TT_MULTILINE_COMMENT,            // "/*" {printableCharacter | newline} "*/"
  TT_COMMENT_END,

  TT_WHITESPACE,                   // " " | tab
  TT_NEWLINE,                      // end of line (i.e. "\n")
  TT_EOF,                          // end of file (i.e. EOF)
  TT_LINE_CONTINUATION,            // line continuation (i.e. "\\\n")
  TT_SEMICOLON,                    // ';'

  TT_OTHER_END,
};

// letter             = "A" | ... | "Z" | "a" | ... | "z"
// digit              = "0" | ... | "9"
// nonzeroDigit       = "1" | ... | "9"
// whitespace         = " " | tab | newline
// printableCharacter = letter | digit | <lots of others> ... | escapeSequence
// escapeSequence     = "\n" |
//                      "\t" |
//                      ...

struct Token {
  enum TokenType  type;
  char*           strval;
  int             intval;
};

struct TokenStream {
  void*          arg;
  struct Token (*next_token)(void*);
  int          (*get_lineno)(void*);
  int          (*get_column)(void*);
  const char*  (*get_filepath)(void*);
  bool         (*is_eof)(void*);
  void         (*close)(void*);
};

struct Token TS_next_token(struct TokenStream* ts);
int          TS_get_lineno(struct TokenStream* ts);
int          TS_get_column(struct TokenStream* ts);
const char*  TS_get_filepath(struct TokenStream* ts);
bool         TS_is_eof(struct TokenStream* ts);
void         TS_close(struct TokenStream* ts);

#endif // TOKEN_H
