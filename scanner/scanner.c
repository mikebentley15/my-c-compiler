#include "scanner.h"
#include "util.h"
#include "stringfunc.h"

#include <stdio.h>

// Returns the expanded version of "\<code>"
char expand_escape_character(char code) {
  // TODO: implement with a global lookup table with code being the index
  char expanded = code;
  if      (code == '"')  { expanded = '"';  }
  else if (code == '0')  { expanded = '\0'; }
  else if (code == '\'') { expanded = '\''; }
  else if (code == '\\') { expanded = '\\'; }
  else if (code == 'a')  { expanded = '\a'; }
  else if (code == 'b')  { expanded = '\b'; }
  else if (code == 'f')  { expanded = '\f'; }
  else if (code == 'n')  { expanded = '\n'; }
  else if (code == 'r')  { expanded = '\r'; }
  else if (code == 't')  { expanded = '\t'; }
  else if (code == 'v')  { expanded = '\v'; }
  else {
    error("Escape sequence not recognized: \\");
    putc(code);
  }
  return expanded;
}

void local_Scanner_nextchar(struct Scanner* scanner) {
  if (scanner->is_eof) {
    scanner->next = '\0';
  } else {
    local_Scanner_keepchar(scanner, scanner->next);
    scanner->next = CharStream_readchar(scanner->in);
    scanner->is_eof = CharStream_is_eof(scanner->in);
  }
}

void local_Scanner_keepchar(struct Scanner* scanner, char ch) {
  Str_append(&(scanner->tok_buf), ch);
}

void local_Scanner_buf_rm_one(struct Scanner* scanner) {
  Str_pop(&(scanner->tok_buf));
}

void local_Scanner_clearbuf(struct Scanner* scanner) {
  Str_clear(&(scanner->tok_buf));
}

bool local_Scanner_character_literal(struct Scanner* scanner) {
  bool is_character_literal = false;
  char literal = '\0';
  if (scanner->next == '\'') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '\\') {
      local_Scanner_nextchar(scanner);
      literal = expand_escape_character(scanner->next);
    } else {
      literal = scanner->next;
    }
    local_Scanner_nextchar(scanner);
    if (scanner->next == '\'') {
      local_Scanner_nextchar(scanner);
      local_Scanner_clearbuf(scanner);
      local_Scanner_keepchar(scanner, literal);
    } else {
      error("Character literal is only allowed to have one character");
    }
  }
  return is_character_literal;
}

bool local_Scanner_int_literal(struct Scanner* scanner) {
  bool is_int_literal = false;
  int value = 0;
  if (scanner->next == '0') {
    is_int_literal = true;
    local_Scanner_nextchar(scanner);
    if (is_digit(scanner->next)) {
      while (!scanner->is_eof && is_digit(scanner->next)) {
        local_Scanner_nextchar(scanner);
      }
      error("A multi-digit number cannot start with zero");
      puts(scanner->tok_buf.data);
    }
    scanner->int_val = 0;
  } else if (is_digit(scanner->next)) { // nonzero
    is_int_literal = true;
    while (!scanner->is_eof && is_digit(scanner->next)) {
      value = 10 * value + (scanner->next - '0');
      local_Scanner_nextchar(scanner);
    }
    scanner->int_val = value;
  }
  return is_int_literal;
}

bool local_Scanner_string_literal(struct Scanner* scanner) {
  bool is_string_literal = false;
  if (scanner->next == '"') {
    is_string_literal = true;
    local_Scanner_nextchar(scanner);
    local_Scanner_clearbuf(scanner); // remove the first '"'
    while (!scanner->is_eof && scanner->next != '"') {
      if (scanner->next == '\\') {
        local_Scanner_nextchar(scanner);
        local_Scanner_buf_rm_one(scanner);  // remove the '\\'
        local_Scanner_keepchar(scanner, expand_escape_character(scanner->next));
        local_Scanner_nextchar(scanner);
        local_Scanner_buf_rm_one(scanner);  // remove the unescaped character
      } else {
        local_Scanner_nextchar(scanner);
      }
    }
    if (scanner->is_eof && scanner->next != '"') {
      error("Reached end of file while parsing string literal");
    }
    local_Scanner_nextchar(scanner);
    local_Scanner_buf_rm_one(scanner); // remove the last '"'
  }
  return is_string_literal;
}

bool local_Scanner_identifier(struct Scanner* scanner) {
  bool is_identifier = false;
  if (scanner->next == '_' || is_letter(scanner->next)) {
    is_identifier = true;
    while (!scanner->is_eof &&
           (scanner->next == '_' ||
            is_letter(scanner->next) ||
            is_digit(scanner->next)))
    {
      local_Scanner_nextchar(scanner);
    }
  }
  return is_identifier;
}

// local functions end

void Scanner_init(struct Scanner* scanner, struct CharStream* stream) {
  if (scanner->is_init) {
    Scanner_close(scanner);
  }
  scanner->in = stream;
  scanner->is_init = true;
  scanner->is_eof = CharStream_is_eof(scanner->in);
  Str_init(&(scanner->tok_buf));
  local_Scanner_clearbuf(scanner);
  local_Scanner_nextchar(scanner);
}

void Scanner_close(struct Scanner* scanner) {
  scanner->in = NULL;
  scanner->is_init = false;
  scanner->is_eof = false;
  scanner->next = '\0';
  Str_del(&(scanner->tok_buf));
}

bool Scanner_is_eof(struct Scanner* scanner) {
  return scanner->is_eof;
}

struct Token Scanner_next(struct Scanner* scanner) {
  struct Token tok;
  local_Scanner_clearbuf(scanner);
  tok.type = TT_ERROR;
  tok.str_val = scanner->tok_buf.data;
  tok.int_val = 0;

  if (scanner->is_eof) {
    tok.type = TT_EOF;
  } else if (scanner->next == '\n') {
    local_Scanner_nextchar(scanner);
    tok.type = TT_NEWLINE;
  } else if (scanner->next == '\\') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '\n') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_LINE_CONTINUATION;
    } else {
      error("A char \\ found not at the end of the line");
      tok.type = TT_ERROR;
    }
  } else if (is_whitespace(scanner->next)) {
    while (!scanner->is_eof && is_whitespace(scanner->next) &&
           scanner->next != '\n')
    {
      local_Scanner_nextchar(scanner);
    }
    tok.type = TT_WHITESPACE;
  } else if (scanner->next == '/') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '/') {
      // line comment
      while (scanner->next != '\n' && !scanner->is_eof) {
        local_Scanner_nextchar(scanner);
      }
      tok.type = TT_LINE_COMMENT;
    } else if (scanner->next == '*') {
      bool should_continue = true;
      local_Scanner_nextchar(scanner);
      while (should_continue) {
        if (scanner->is_eof) {
          should_continue = false;
          error("End of file reached during a multiline comment");
        } else if (scanner->next == '*') {
          local_Scanner_nextchar(scanner);
          if (scanner->next == '/') {
            should_continue = false;
          }
        }
        local_Scanner_nextchar(scanner);
      }
      tok.type = TT_MULTILINE_COMMENT;
    } else if (scanner->is_eof) {
      error("Unrecognized token '/' at the end of file");
    } else {
      // handle the division operator
      if (scanner->next == '=') {
        local_Scanner_nextchar(scanner);
        tok.type = TT_DIVIDE_EQUALS;
      } else {
        tok.type = TT_DIVIDE;
      }
    }
  } else if (scanner->next == '#') {
    local_Scanner_nextchar(scanner);
    tok.type = TT_POUND;
  } else if (scanner->next == '+') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '+') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_INCREMENT;
    } else if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_PLUS_EQUALS;
    } else {
      tok.type = TT_PLUS;
    }
  } else if (scanner->next == '-') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '-') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_DECREMENT;
    } else if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_MINUS_EQUALS;
    } else if (scanner->next == '>') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_ARROW;
    } else {
      tok.type = TT_MINUS;
    }
  } else if (scanner->next == '*') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_MULTIPLY_EQUALS;
    } else {
      tok.type = TT_MULTIPLY;
    }
  } else if (scanner->next == '%') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_MODULUS_EQUALS;
    } else {
      tok.type = TT_MODULUS;
    }
  } else if (scanner->next == '(') {
    local_Scanner_nextchar(scanner);
    tok.type = TT_LPAREN;
  } else if (scanner->next == ')') {
    local_Scanner_nextchar(scanner);
    tok.type = TT_RPAREN;
  } else if (scanner->next == '.') {
    local_Scanner_nextchar(scanner);
    tok.type = TT_DOT;
  } else if (scanner->next == '=') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_EQUALITY;
    } else {
      tok.type = TT_EQUALS;
    }
  } else if (scanner->next == '!') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_NOT_EQUAL;
    } else {
      tok.type = TT_BOOLEAN_NOT;
    }
  } else if (scanner->next == '<') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_LESS_EQUAL;
    } else if (scanner->next == '<') {
      local_Scanner_nextchar(scanner);
      if (scanner->next == '=') {
        local_Scanner_nextchar(scanner);
        tok.type = TT_BITSHIFT_LEFT_EQUALS;
      } else {
        tok.type = TT_BITSHIFT_LEFT;
      }
    } else {
      tok.type = TT_LESS;
    }
  } else if (scanner->next == '>') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_GREATER_EQUAL;
    } else if (scanner->next == '>') {
      local_Scanner_nextchar(scanner);
      if (scanner->next == '=') {
        local_Scanner_nextchar(scanner);
        tok.type = TT_BITSHIFT_RIGHT_EQUALS;
      } else {
        tok.type = TT_BITSHIFT_RIGHT;
      } 
    } else {
      tok.type = TT_GREATER;
    }
  } else if (scanner->next == '&') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '&') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_BOOLEAN_AND;
    } else if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_BITWISE_AND_EQUALS;
    } else {
      tok.type = TT_BITWISE_AND;
    }
  } else if (scanner->next == '|') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '|') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_BOOLEAN_OR;
    } else if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_BIWISE_OR_EQUALS;
    } else {
      tok.type = TT_BITWISE_OR;
    }
  } else if (scanner->next == '^') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '=') {
      local_Scanner_nextchar(scanner);
      tok.type = TT_BITWISE_XOR_EQUALS;
    } else {
      tok.type = TT_BITWISE_XOR;
    }
  } else if (scanner->next == '~') {
    local_Scanner_nextchar(scanner);
    tok.type = TT_BITWISE_INVERT;
  } else if (scanner->next == ':') {
    local_Scanner_nextchar(scanner);
    tok.type = TT_COLON;
  } else if (scanner->next == '?') {
    local_Scanner_nextchar(scanner);
    tok.type = TT_QUESTION;
  } else if (local_Scanner_character_literal(scanner)) {
    tok.type = TT_CHARACTER_LITERAL;
  } else if (local_Scanner_int_literal(scanner)) {
    tok.type = TT_INT_LITERAL;
  } else if (local_Scanner_string_literal(scanner)) {
    tok.type = TT_STRING_LITERAL;
  } else if (local_Scanner_identifier(scanner)) {
    tok.type = TT_IDENTIFIER;
  } else {
    local_Scanner_nextchar(scanner);
    tok.type = TT_ERROR;
    error("Unrecognized character: ");
    puts(scanner->tok_buf.data);
  }

  // error handling
  if (tok.type >= TT_UNSUPPORTED_OPERATORS_BEGIN &&
      tok.type <= TT_UNSUPPORTED_OPERATORS_END)
  {
    error("Unsupported operator: ");
  }

  return tok;
}

struct Token Scanner_p_next_token(void* s) {
  struct Scanner *scanner = (struct Scanner*) s;
  return Scanner_next(scanner);
}

int Scanner_p_get_lineno(void* s) {
  struct Scanner *scanner = (struct Scanner*) s;
  return scanner->in->get_lineno(scanner->in->arg);
}

int Scanner_p_get_column(void* s) {
  struct Scanner *scanner = (struct Scanner*) s;
  return scanner->in->get_column(scanner->in->arg);
}

const char* Scanner_p_get_filepath(void* s) {
  struct Scanner *scanner = (struct Scanner*) s;
  return scanner->in->get_filepath(scanner->in->arg);
}

bool Scanner_p_is_eof(void* s) {
  struct Scanner *scanner = (struct Scanner*) s;
  return scanner->in->is_eof(scanner->in->arg);
}

void Scanner_p_close(void* s) {
  struct Scanner *scanner = (struct Scanner*) s;
  scanner->in->close(scanner->in->arg);
}

struct TokenStream Scanner_to_TokenStream(struct Scanner* scanner) {
  struct TokenStream stream;
  stream.arg          = scanner;
  stream.next_token   = &Scanner_p_next_token;
  stream.get_lineno   = &Scanner_p_get_lineno;
  stream.get_filepath = &Scanner_p_get_filepath;
  stream.is_eof       = &Scanner_p_is_eof;
  stream.close        = &Scanner_p_close;
}
