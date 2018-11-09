// Simple compiler.  All in one file.
//
// Usage:
//   simple-compiler < source.c > executable-name
//
// Description:
//   This will compile source.c into executable-name.  The compiler reads the
//   source code from standard in and outputs the executable code to standard
//   out.  Note, this is a subset of the C programming language.

//
//  Scanner Grammar Rules:
//
// anything           = .
// nonnewline         = ^newline
// letter             = "A" | ... | "Z" | "a" | ... | "z"
// underscore_letter  = "_" | letter
// digit              = "0" | ... | "9"
// nonzeroDigit       = "1" | ... | "9"
// whitespace         = " " | tab | "\n"
// escSeq             = "\n" | "\t" | ...
// printCharDbl       = (^"\" & ^""") | escSeq
// printCharSing      = (^"\" & ^"'") | escSeq
// identifier         = underscore_letter {underscore_letter | digit}
// linecomment        = "//" {nonnewline} "\n"
// mlinecomment       = "/*" {anything} "*/"
// comment            = linecomment | mlinecomment
// operator           = "-" | "+" | "/" | ...
// eaten              = comment | whitespace
// int_literal        = "0" | nonzeroDigit {digit}
// str_literal        = """ {printCharDbl} """
// char_literal       = "'" printCharSing "'"
// literal            = int_literal | str_literal | char_literal
// token              = identifier | operator | literal
// scanning           = {{eaten} token}


//
//  Parser Grammar Rules:
//

//>>----------------------------------------------------------------------------
// BEGIN LIBRARY FUNCTION DECLARATIONS

int getchar(void);
int putchar(int ch);

// END LIBRARY FUNCTION DECLARATIONS
//<<----------------------------------------------------------------------------

//>>----------------------------------------------------------------------------
// BEGIN CONSTANTS

// alternative types.
// This is so our compiler that supports NULL, true, and false can still
// compile this file.
const int nullptr = 0;
const int TRUE    = 1;
const int FALSE   = 0;

// token types (TT)
// - miscelaneous
const int TT_ERROR             =  0;  // not a valid token (default value)
const int TT_COMMENT           =  1;  // comment
const int TT_EOF               =  2;  // end of file (i.e. EOF)
const int TT_IDENTIFIER        =  3;  // ("_" | letter) {"_" | letter | digit}
// - literals
const int TT_INT_LITERAL       =  4;  // "0"|nonzeroDigit{digit}
const int TT_STRING_LITERAL    =  5;  // """ {printChar | "\'" | \n} """
const int TT_CHARACTER_LITERAL =  6;  // "'" (printChar | """) "'"
// - keywords
const int TT_BOOL              =  6;  // bool
const int TT_BREAK             =  7;  // break 
const int TT_CHAR              =  8;  // char
const int TT_CONST             =  9;  // const
const int TT_CONTINUE          = 10;  // continue
const int TT_ELSE              = 11;  // else
const int TT_FALSE_LITERAL     = 12;  // false
const int TT_IF                = 13;  // if
const int TT_INT               = 14;  // int
const int TT_NULL              = 15;  // NULL
const int TT_RETURN            = 16;  // return
const int TT_TRUE_LITERAL      = 17;  // true
const int TT_VOID              = 18;  // void
const int TT_WHILE             = 19;  // while
// - operators
const int TT_ARROW             = 20;  // ->
const int TT_BITSHIFT_LEFT     = 21;  // <<
const int TT_BITSHIFT_RIGHT    = 22;  // >>
const int TT_BITWISE_AND       = 23;  // & (also the address of operator)
const int TT_ADDRESS_OF        = 23;  // &
const int TT_BITWISE_INVERT    = 24;  // ~
const int TT_BITWISE_OR        = 25;  // |
const int TT_BITWISE_XOR       = 26;  // ^
const int TT_BOOLEAN_AND       = 27;  // &&
const int TT_BOOLEAN_NOT       = 28;  // !
const int TT_BOOLEAN_OR        = 29;  // ||
const int TT_COMMA             = 30;  // ','
const int TT_DIVIDE            = 31;  // /
const int TT_DOT               = 32;  // .
const int TT_EQUALITY          = 33;  // ==
const int TT_EQUALS            = 34;  // =
const int TT_GREATER           = 35;  // >
const int TT_GREATER_EQUAL     = 36;  // >=
const int TT_LESS              = 37;  // <
const int TT_LESS_EQUAL        = 38;  // <=
const int TT_LPAREN            = 39;  // (
const int TT_MINUS             = 40;  // -
const int TT_MODULUS           = 41;  // %
const int TT_MULTIPLY          = 42;  // * (also the dereference)
const int TT_DEREFERENCE       = 42;  // *
const int TT_NOT_EQUAL         = 43;  // !=
const int TT_PLUS              = 44;  // +
const int TT_RPAREN            = 45;  // )
const int TT_SEMICOLON         = 46;  // ;
// END CONSTANTS
//<<----------------------------------------------------------------------------

//>>----------------------------------------------------------------------------
// BEGIN GLOBAL VARIABLES

// printint variables
char  printint_buf[100];     // buffer for the printint() function
char *printint_ptr;          // pointer used in printint_buf
int   printint_is_neg;       // boolean for printint() to capture if i < 0

// scanner function variables
int   tok_type = TT_ERROR;   // set by scan_next()
char  tok_str[1000];         // set by scan_next()
int   tok_size;              // size of tok_str (i.e. tok_str[tok_size] == '\0')
int   tok_int = 0;           // set by scan_next() when int or char literal
char  prev_char = '\0';      // prev character read by scanner
char  put_back  = '\0';      // a character was put back onto the read stack
char  next_char = '\0';      // next character to be read by the scanner
int   lineno = 1;            // line number
int   column = 0;            // column number

// END GLOBAL VARIABLES
//<<----------------------------------------------------------------------------

int strequal(const char* a, const char* b) {
  while (a[0] == b[0] && a[0] != '\0') {
    a = a + 1;
    b = b + 1;
  }
  if (a[0] == b[0]) {
    return TRUE;
  }
  return FALSE;
}

void printstr(const char* msg) {
  while (msg[0] != '\0') {
    putchar(msg[0]);
    msg = msg + 1;
  }
}

void printint(int i) {
  if (i < 0) {
    i = -i;
    printint_is_neg = TRUE;
  } else {
    printint_is_neg = FALSE;
  }
  printint_ptr = printint_buf + 99;
  printint_ptr[0] = '\0';
  printint_ptr = printint_ptr - 1;
  printint_ptr[0] = '0' + (i % 10);
  i = i / 10;
  while (i > 0) {
    printint_ptr = printint_ptr - 1;
    printint_ptr[0] = '0' + (i % 10);
    i = i / 10;
  }
  if (printint_is_neg) {
    printint_ptr = printint_ptr - 1;
    printint_ptr[0] = '-';
  }
  printstr(printint_ptr);
}

void scan_error(char char* msg) {
  printstr("Scanner error: ");
  printstr(msg);
  putchar('\n');
  printstr("  next_char   = '");
  putchar(next_char);
  printstr("'\n");
  printstr("  lineno      = ");
  printint(lineno);
  putchar('\n');
  printstr("  column      = ");
  printint(column);
  putchar('\n');
}

void push_tok_char() {
  // TODO: check for overflow
  tok_str[tok_size] = next_char;
  tok_size = tok_size + 1;
  tok_str[tok_size] = '\0';
}

void pop_tok_char() {
  tok_size = tok_size - 1;
  tok_str[tok_size] = '\0';
}

void clear_tok() {
  tok_size = 0;
  tok_str[0] = '\0';
}

void get_next_char() {
  push_tok_char();
  prev_char = next_char;
  if (put_back != '\0') {
    next_char = put_back;
    put_back = '\0';
  } else {
    next_char = getchar();
  }
}

void put_back_char() {
  put_back = next_char;
  next_char = prev_char;
}

int letter() {
  if ((next_char >= 'A' && next_char <= 'Z') ||
      (next_char >= 'a' && next_char <= 'z'))
  {
    get_next_char();
    return TRUE;
  }
  return FALSE;
}

int underscore_letter() {
  if (next_char == '_') {
    get_next_char();
    return TRUE;
  } else if (letter()) {
    return TRUE;
  }
  return FALSE;
}

int digit() {
  if (next_char >= '0' && next_char <= '9') {
    get_next_char();
    return TRUE;
  }
  return FALSE;
}

int nonzeroDigit() {
  if (next_char >= '1' && next_char <= '9') {
    get_next_char();
    return TRUE;
  }
  return FALSE;
}

int whitespace() {
  if (next_char == ' ' || next_char == '\t' || next_char == '\n') {
    get_next_char();
    return TRUE;
  }
  return FALSE;
}

int escSeq() {
  if (next_char == '\\') {
    get_next_char();
    pop_tok_char();  // remove the \ character
    if      (next_char == '"')  { next_char = '"';  }
    else if (next_char == '0')  { next_char = '\0'; }
    else if (next_char == '\'') { next_char = '\''; }
    else if (next_char == '\\') { next_char = '\\'; }
    else if (next_char == 'a')  { next_char = '\a'; }
    else if (next_char == 'b')  { next_char = '\b'; }
    else if (next_char == 'f')  { next_char = '\f'; }
    else if (next_char == 'n')  { next_char = '\n'; }
    else if (next_char == 'r')  { next_char = '\r'; }
    else if (next_char == 't')  { next_char = '\t'; }
    else if (next_char == 'v')  { next_char = '\v'; }
    else {
      scan_error("Escape sequence not recognized");
    }
    get_next_char();
    return TRUE;
  }
  return FALSE;
}

int printCharDbl() {
  if (next_char != '\\' && next_char != '"') {
    get_next_char();
    return TRUE;
  } else if (escSeq()) {
    return TRUE;
  }
  return FALSE;
}

int printCharSing() {
  if (next_char != '\\' && next_char != '\'') {
    get_next_char();
    return TRUE;
  } else if (escSeq()) {
    return TRUE;
  }
  return FALSE;
}

void find_identifier_type() {
  // read the identifier for keywords
  if      (strequal(tok_str, "bool"    )) { tok_type = TT_BOOL;          }
  else if (strequal(tok_str, "break"   )) { tok_type = TT_BREAK;         }
  else if (strequal(tok_str, "char"    )) { tok_type = TT_CHAR;          }
  else if (strequal(tok_str, "const"   )) { tok_type = TT_CONST;         }
  else if (strequal(tok_str, "continue")) { tok_type = TT_CONTINUE;      }
  else if (strequal(tok_str, "else"    )) { tok_type = TT_ELSE;          }
  else if (strequal(tok_str, "false"   )) { tok_type = TT_FALSE_LITERAL; }
  else if (strequal(tok_str, "if"      )) { tok_type = TT_IF;            }
  else if (strequal(tok_str, "int"     )) { tok_type = TT_INT;           }
  else if (strequal(tok_str, "NULL"    )) { tok_type = TT_NULL;          }
  else if (strequal(tok_str, "return"  )) { tok_type = TT_RETURN;        }
  else if (strequal(tok_str, "true"    )) { tok_type = TT_TRUE_LITERAL;  }
  else if (strequal(tok_str, "void"    )) { tok_type = TT_VOID;          }
  else if (strequal(tok_str, "while"   )) { tok_type = TT_WHILE;         }
  else                                    { tok_type = TT_IDENTIFIER;    }
}

int identifier() {
  if (underscore_letter()) {
    while (underscore_letter() || digit()) { }
    find_identifier_type();
    return TRUE;
  }
  return FALSE;
}

int linecomment() {
  if (next_char == '/') {
    get_next_char();
    if (next_char == '/') {
      while (next_char != '\n') { get_next_char(); }
      return TRUE;
    } else {
      put_back_char();
    }
  }
  return FALSE;
}

int mlinecomment() {
  if (next_char == '/') {
    get_next_char();
    if (next_char == '*') {
      get_next_char();
      while (TRUE) {
        get_next_char();
        if (prev_char == '*' && next_char == '/') {
          get_next_char();
          return TRUE;
        }
      }
    } else {
      put_back_char();
    }
  }
  return FALSE;
}

int comment() {
  if (linecomment() || mlinecomment()) {
    tok_type = TT_COMMENT;
    return TRUE;
  }
  return FALSE;
}

int int_literal() {
  if (nonzeroDigit()) {
    tok_int = prev_char - '0';
    while (digit()) {
      tok_int = tok_int * 10 + (prev_char - '0');
    }
    tok_type = TT_INT_LITERAL;
    return TRUE;
  } else if (next_char == '0') {
    get_next_char();
    tok_int = 0;
    tok_type = TT_INT_LITERAL;
    return TRUE;
  }
  return FALSE;
}

int str_literal() {
  if (next_char == '"') {
    get_next_char();
    pop_tok_char();
    while (printCharDbl()) { }
    tok_type = TT_STRING_LITERAL;
    if (next_char == '"') {
      get_next_char();
      pop_tok_char();
      return TRUE;
    } else {
      scan_error("");
    }
  }
  return FALSE;
}

int operator() {
  
}

void get_next_tok() {
  clear_tok();
  // TODO: implement
}

int main(void) {
  

  return 0;
}
