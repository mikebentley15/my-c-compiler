#include "scanner.h"

#include <stdio.h>

// local functions begin

bool string_equal(const char* a, const char* b) {
  int i = 0;
  while (a[i] == b[i] && a[i] != '\0') { i++; }
  return a[i] == b[i];
}

void error(char *message) {
  puts("Error: ");
  puts(message);
  putc('\n');
}

// Returns the TokenType enum for the keyword else TT_IDENTIFIER,
enum TokenType getIdnetifierType(const char* word) {
  enum TokenType type;

  if      (string_equal(word, "_Packed"))  { type = TT_PACKED;     }
  else if (string_equal(word, "and"))      { type = TT_AND;        }
  else if (string_equal(word, "auto"))     { type = TT_AUTO;       }
  else if (string_equal(word, "bool"))     { type = TT_BOOL;       }
  else if (string_equal(word, "break"))    { type = TT_BREAK;      }
  else if (string_equal(word, "byte"))     { type = TT_BYTE;       }
  else if (string_equal(word, "case"))     { type = TT_CASE;       }
  else if (string_equal(word, "char"))     { type = TT_CHAR;       }
  else if (string_equal(word, "const"))    { type = TT_CONST;      }
  else if (string_equal(word, "continue")) { type = TT_CONTINUE;   }
  else if (string_equal(word, "default"))  { type = TT_DEFAULT;    }
  else if (string_equal(word, "do"))       { type = TT_DO;         }
  else if (string_equal(word, "double"))   { type = TT_DOUBLE;     }
  else if (string_equal(word, "else"))     { type = TT_ELSE;       }
  else if (string_equal(word, "enum"))     { type = TT_ENUM;       }
  else if (string_equal(word, "extern"))   { type = TT_EXTERN;     }
  else if (string_equal(word, "false"))    { type = TT_FALSE;      }
  else if (string_equal(word, "float"))    { type = TT_FLOAT;      }
  else if (string_equal(word, "for"))      { type = TT_FOR;        }
  else if (string_equal(word, "goto"))     { type = TT_GOTO;       }
  else if (string_equal(word, "if"))       { type = TT_IF;         }
  else if (string_equal(word, "int"))      { type = TT_INT;        }
  else if (string_equal(word, "long"))     { type = TT_LONG;       }
  else if (string_equal(word, "or"))       { type = TT_OR;         }
  else if (string_equal(word, "register")) { type = TT_REGISTER;   }
  else if (string_equal(word, "return"))   { type = TT_RETURN;     }
  else if (string_equal(word, "short"))    { type = TT_SHORT;      }
  else if (string_equal(word, "signed"))   { type = TT_SIGNED;     }
  else if (string_equal(word, "sizeof"))   { type = TT_SIZEOF;     }
  else if (string_equal(word, "static"))   { type = TT_STATIC;     }
  else if (string_equal(word, "struct"))   { type = TT_STRUCT;     }
  else if (string_equal(word, "switch"))   { type = TT_SWITCH;     }
  else if (string_equal(word, "true"))     { type = TT_TRUE;       }
  else if (string_equal(word, "typedef"))  { type = TT_TYPEDEF;    }
  else if (string_equal(word, "union"))    { type = TT_UNION;      }
  else if (string_equal(word, "void"))     { type = TT_VOID;       }
  else if (string_equal(word, "volatile")) { type = TT_VOLATILE;   }
  else if (string_equal(word, "while"))    { type = TT_WHILE;      }
  else                                     { type = TT_IDENTIFIER; }

  return type;
}

bool is_whitspace(char ch) {
  return ch == ' ' || ch == '\n' || ch == '\t';
}

bool is_digit(char ch) {
  return ch >= '0' && ch <= '9';
}

bool is_letter(char ch) {
  return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

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
  if (buflen == 499) {
    error("Scanner buffer exceeded");
  } else {
    scanner->tok_buf[scanner->buflen] = scanner->next;
    scanner->buflen++;
    scanner->tok_buf[scanner->buflen] = '\0';
  }
}

void local_Scanner_buf_rm_one(struct Scanner* scanner) {
  scanner->buflen--;
  scanner->tok_buf[scanner->buflen] = '\0';
}

void local_Scanner_clearbuf(struct Scanner* scanner) {
  scanner->buflen = 0;
  scanner->tok_buf[scanner->buflen] = '\0';
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
      puts(scanner->tok_buf);
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
  local_Scanner_clearbuf(scanner);
  local_Scanner_nextchar(scanner);
}

void Scanner_close(struct Scanner* scanner) {
  scanner->in = NULL;
  scanner->is_init = false;
  scanner->is_eof = false;
  scanner->next = '\0';
  scanner->tok_buf[0] = '\0';
}

bool Scanner_is_eof(struct Scanner* scanner) {
  return scanner->is_eof;
}

struct Token Scanner_next(struct Scanner* scanner) {
  struct Token tok;
  local_Scanner_clearbuf(scanner);
  tok.type = TT_ERROR;
  tok.str_val = scanner->tok_buf;
  tok.int_val = 0;

  // eat whitespace
  while (!scanner->is_eof && is_whitespace(scanner->next)) {
    local_Scanner_nextchar(scanner);
    local_Scanner_clearbuf(scanner);
  }

  // eat comments
  bool already_found_token = false;
  if (scanner->next == '/') {
    local_Scanner_nextchar(scanner);
    if (scanner->next == '/') {
      while (scanner->next != '\n' && !scanner->is_eof) {
        local_Scanner_nextchar(scanner);
        local_Scanner_clearbuf(scanner);
      }
      local_Scanner_nextchar(scanner);
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
    } else if (scanner->is_eof) {
      error("Unrecognized token '/' at the end of file");
    } else {
      already_found_token = true;
      // handle the division operator
      if (scanner->next == '=') {
        local_Scanner_nextchar(scanner);
        tok.type = TT_DIVIDE_EQUALS;
      } else {
        tok.type = TT_DIVIDE;
      }
    }
  }

  if (already_found_token) {
    // do nothing, already captured TT_DIVIDE or TT_DIVIDE_EQUALS
  } if (scanner->is_eof) {
    tok.type = TT_EOF;
  } else if (scanner->next == '#') {
    // eat the rest of the line
    while (scanner->next != '\n' && !scanner->is_eof) {
      local_Scanner_nextchar(scanner);
    }
    tok.type = TT_ERROR;
    error("Cannot handle preprocessing directive: ");
    puts(scanner->tok_buf);
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
    tok.type = getIdentifierType(scanner->tok_buf);
  } else {
    local_Scanner_nextchar(scanner);
    tok.type = TT_ERROR;
    error("Unrecognized character: ");
    puts(scanner->tok_buf);
  }

  // error handling
  if (tok.type > TT_KEYWORD_UNSUPPORTED_BEGIN && tok.type < TT_KEYWORD_UNSUPPORTED_END) {
    error("Unsupported keyword: ");
    puts(scanner->tok_buf);
  } else if (tok.type > TT_UNSUPPORTED_OPERATORS_BEGIN) {
    error("Unsupported operator: ");
  }

  // TODO: use a preprocessor here maybe

  return tok;
}

