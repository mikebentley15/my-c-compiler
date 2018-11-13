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
// scanning           = {{eaten} token} EOF.
// token              = identifier | literal | operator.
// identifier         = underscore_letter {underscore_letter | digit}.
// underscore_letter  = "_" | letter.
// letter             = "A" | ... | "Z" | "a" | ... | "z"
// digit              = "0" | ... | "9".
// literal            = int_literal | str_literal | char_literal.
// int_literal        = "0" | nonzeroDigit {digit}.
// nonzeroDigit       = "1" | ... | "9"
// str_literal        = """ {printCharDbl} """.
// printCharDbl       = (^"\" & ^""") | escSeq
// escSeq             = "\n" | "\t" | ...
// char_literal       = "'" printCharSing "'".
// printCharSing      = (^"\" & ^"'") | escSeq
// operator           = "-" | "+" | "/" | ...
// eaten              = comment | whitespace.
// comment            = linecomment | mlinecomment.
// linecomment        = "//" {nonnewline} "\n".
// nonnewline         = ^newline
// mlinecomment       = "/*" {anything} "*/".
// anything           = .
// whitespace         = one_whitespace {one_whitespace}.
// one_whitespace     = " " | tab | newline.

//
//  Parser Grammar Rules:
//
// parsing            = {def (funcargdef (body | ";") |
//                            {arrayidx} | "=" expr] ";")}.
// def                = type identifier.
// type               = ["const"] ("void" | "int" | "char" | "bool") {"*"}.
// funcargdef         = "(" [type [identifier] {"," type [identifier]}] ")".
// arrayidx           = "[" expr "]"
// body               = "{" {bodyelem} "}".
// bodyelem           = whileloop | ifblock | statement.
// whileloop          = "while" "(" expr ")" body.
// ifblock            = "if" "(" expr ")" body 
//                        {"else" "if" "(" expr ")" body} ["else" body].
// statement          = identifier (funcarg | "=" expr) ";"
// funcarg            = "(" [expr {"," expr}] ")"
// expr               = str_literal | expr1
// expr1              = expr2 {"||" expr2}.
// expr2              = expr3 {"&&" expr3}.
// expr3              = expr4 {"|"  expr4}.
// expr4              = expr5 {"^"  expr5}.
// expr5              = expr6 {"&"  expr6}.
// expr6              = expr7 {("==" | "!=") expr7}.
// expr7              = expr8 {("<" | "<=" | ">" | ">=") expr8}.
// expr8              = expr9 {("<<" | ">>") expr9}.
// expr9              = term {("+" | "-") term}.
// term               = factor {("*" | "/" | "%") factor}.
// factor             = ["-"] ( identifier [funcarg] {arrayidx} | int_literal |
//                              char_literal | true_literal | false_literal |
//                              "(" expr ")" ).

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

// value returned by getchar() when end of file is reached.
const int EOF     = -1;

// token types (TT)
// - miscelaneous
const int TT_EOF               = -1;  // end of file (i.e. EOF)
const int TT_ERROR             =  0;  // not a valid token (default value)
const int TT_COMMENT           =  1;  // comment
const int TT_WHITESPACE        =  2;  // whitespace
const int TT_IDENTIFIER        =  3;  // ("_" | letter) {"_" | letter | digit}

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
const int TT_COMMA             = 30;  // ,
const int TT_DIVIDE            = 31;  // /
const int TT_EQUALITY          = 33;  // ==
const int TT_EQUALS            = 34;  // =
const int TT_GREATER           = 35;  // >
const int TT_GREATER_EQUAL     = 36;  // >=
const int TT_LESS              = 37;  // <
const int TT_LESS_EQUAL        = 38;  // <=
const int TT_LCURLY            = 39;  // {
const int TT_LPAREN            = 40;  // (
const int TT_LSQUARE           = 41;  // [
const int TT_MINUS             = 42;  // -
const int TT_MODULUS           = 43;  // %
const int TT_MULTIPLY          = 44;  // * (also the dereference)
const int TT_DEREFERENCE       = 44;  // *
const int TT_NOT_EQUAL         = 45;  // !=
const int TT_PLUS              = 46;  // +
const int TT_RCURLY            = 47;  // }
const int TT_RPAREN            = 48;  // )
const int TT_RSQUARE           = 49;  // ]
const int TT_SEMICOLON         = 50;  // ;
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
char  tok_str[5000];         // set by scan_next()
int   tok_size;              // size of tok_str (i.e. tok_str[tok_size] == '\0')
int   tok_int = 0;           // set by scan_next() when int or char literal
char  prev_char = EOF;       // prev character read by scanner
char  put_back  = EOF;       // a character was put back onto the read stack
int   read_char = EOF;       // read in from getchar()
char  next_char = '\0';      // next character to be read by the scanner
int   lineno = 1;            // line number
int   column = -1;           // column number
int   prev_line_column = 0;  // column count of previous line

// END GLOBAL VARIABLES
//<<----------------------------------------------------------------------------

//>>----------------------------------------------------------------------------
// BEGIN GENERIC HELPER FUNCTIONS

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

// END GENERIC HELPER FUNCTIONS
//<<----------------------------------------------------------------------------

//>>----------------------------------------------------------------------------
// BEGIN SCANNER FUNCTIONS

void scan_error(const char* msg) {
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
  if (next_char != EOF) {
    push_tok_char();
    prev_char = next_char;
  }

  if (next_char == '\n') {
    lineno = lineno + 1;
    prev_line_column = column;
    column = 0;
  } else if (next_char != EOF) {
    column = column + 1;
  }

  if (put_back != EOF) {
    next_char = put_back;
    put_back = EOF;
  } else if (next_char != EOF) {
    read_char = getchar();
    if (read_char < 0) { // for portability, don't assume EOF == -1
      next_char = EOF;
    } else {
      next_char = read_char;
    }
  }
}

void put_back_char() {
  if (prev_char == '\n') {
    lineno = lineno - 1;
    column = prev_line_column;
  } else {
    column = column - 1;
  }
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

int one_whitespace() {
  if (next_char == ' ' || next_char == '\t' || next_char == '\n') {
    get_next_char();
    return TRUE;
  }
  return FALSE;
}

int whitespace() {
  if (one_whitespace()) {
    while (one_whitespace()) { }
    tok_type = TT_WHITESPACE;
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
    else if (next_char == 'n')  { next_char = '\n'; lineno = lineno - 1; }
    else if (next_char == 'r')  { next_char = '\r'; }
    else if (next_char == 't')  { next_char = '\t'; }
    else if (next_char == 'v')  { next_char = '\v'; }
    else if (next_char == EOF) {
      scan_error("End of file reached when scanning escape sequence");
      return FALSE;
    } else {
      scan_error("Escape sequence not recognized");
      return FALSE;
    }
    get_next_char();
    return TRUE;
  }
  return FALSE;
}

int printCharDbl() {
  if (next_char == EOF) {
    return FALSE;
  } else if (next_char != '\\' && next_char != '"') {
    get_next_char();
    return TRUE;
  } else if (escSeq()) {
    return TRUE;
  }
  return FALSE;
}

int printCharSing() {
  if (next_char == EOF) {
    return FALSE;
  } else if (next_char != '\\' && next_char != '\'') {
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
      while (next_char != '\n' && next_char != EOF) {
        get_next_char();
      }
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
    // TODO: handle EOF
    if (next_char == '*') {
      get_next_char();
      while (TRUE) {
        get_next_char();
        if (prev_char == '*' && next_char == '/') {
          get_next_char();
          return TRUE;
        } else if (next_char == EOF) {
          scan_error("End of file reached during a multi-line comment");
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

int single_op(char ch, int toktype) {
  if (next_char == ch) {
    get_next_char();
    tok_type = toktype;
    return TRUE;
  }
  return FALSE;
}

int double_op(char c1, int t1, char c2, int t2) {
  if (single_op(c1, t1)) {
    single_op(c2, t2);
    return TRUE;
  }
  return FALSE;
}

int operator() {
  if (single_op('<', TT_LESS)) {
    if (single_op('<', TT_BITSHIFT_LEFT)) { }
    else if (single_op('=', TT_LESS_EQUAL)) { }
    return TRUE;
  } else if (single_op('>', TT_GREATER)) {
    if (single_op('>', TT_BITSHIFT_RIGHT)) { }
    else if (single_op('=', TT_GREATER_EQUAL)) { }
    return TRUE;
  } else if (double_op('&', TT_BITWISE_AND, '&', TT_BOOLEAN_AND)) {
    return TRUE;
  } else if (single_op('~', TT_BITWISE_INVERT)) {
    return TRUE;
  } else if (double_op('|', TT_BITWISE_OR, '|', TT_BOOLEAN_OR)) {
    return TRUE;
  } else if (single_op('^', TT_BITWISE_XOR)) {
    return TRUE;
  } else if (double_op('!', TT_BOOLEAN_NOT, '=', TT_NOT_EQUAL)) {
    return TRUE;
  } else if (single_op(',', TT_COMMA)) {
    return TRUE;
  } else if (single_op('/', TT_DIVIDE)) {
    return TRUE;
  } else if (double_op('=', TT_EQUALS, '=', TT_EQUALITY)) {
    return TRUE;
  } else if (single_op('{', TT_LCURLY)) {
    return TRUE;
  } else if (single_op('(', TT_LPAREN)) {
    return TRUE;
  } else if (single_op('[', TT_LSQUARE)) {
    return TRUE;
  } else if (single_op('-', TT_MINUS)) {
    return TRUE;
  } else if (single_op('%', TT_MODULUS)) {
    return TRUE;
  } else if (single_op('*', TT_MULTIPLY)) {
    return TRUE;
  } else if (single_op('+', TT_PLUS)) {
    return TRUE;
  } else if (single_op('}', TT_RCURLY)) {
    return TRUE;
  } else if (single_op(')', TT_RPAREN)) {
    return TRUE;
  } else if (single_op(']', TT_RSQUARE)) {
    return TRUE;
  } else if (single_op(';', TT_SEMICOLON)) {
    return TRUE;
  }
  return FALSE;
}

int eaten() {
  if (comment() || whitespace()) {
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
    tok_type = TT_STRING_LITERAL;
    pop_tok_char();
    while (printCharDbl()) { }
    if (next_char == '"') {
      get_next_char();
      pop_tok_char();
      return TRUE;
    } else if (next_char == EOF) {
      scan_error("End of file reached while scanning string literal");
    } else {
      scan_error("String literal did not finish");
    }
    return TRUE;
  }
  return FALSE;
}

int char_literal() {
  if (next_char == '\'') {
    get_next_char();
    tok_type = TT_CHARACTER_LITERAL;
    pop_tok_char();
    if (printCharSing()) {
      tok_int = (int) prev_char;
      if (next_char == '\'') {
        get_next_char();
        pop_tok_char();
      } else if (next_char == EOF) {
        scan_error("End of file reached while scanning character literal");
      } else {
        scan_error("Character literal is longer than one character");
      }
    } else if (next_char == EOF) {
      scan_error("End of file reached while scanning character literal");
    } else {
      scan_error("Invalid character literal");
    }
    return TRUE;
  }
  return FALSE;
}

int literal() {
  if (int_literal() || str_literal() || char_literal()) {
    return TRUE;
  }
  return FALSE;
}

int token() {
  if (identifier() || literal() || operator()) {
    return TRUE;
  }
  return FALSE;
}

void get_next_tok() {
  while (eaten()) { }
  clear_tok();
  if (token()) { }
  else if (next_char == EOF) { tok_type = TT_EOF; }
  else {
    scan_error("Unrecognized token, skipping character");
    get_next_char();
  }
}

void scanner_init() {
  get_next_char();
}

// END SCANNER FUNCTIONS
//<<----------------------------------------------------------------------------



//>>----------------------------------------------------------------------------
// BEGIN PARSER FUNCTIONS

void parse_error(const char* msg) {
  printstr("Parser error: ");
  printstr(msg);
  printstr("\n  ");
  print_tok();
  printstr("  lineno      = ");
  printint(lineno);
  putchar('\n');
  printstr("  column      = ");
  printint(column);
  putchar('\n');
}

int expr();

int funcarg() {
  if (tok_type == TT_LPAREN) {
    get_next_tok();
    if (expr()) {
      // handle expr
    }
    while (tok_type == TT_COMMA) {
      get_next_tok();
      if (expr()) {
        // handle expr
      } else {
        parse_error("Expected expression");
      }
    }
    if (tok_type == TT_RPAREN) {
      get_next_tok();
    } else {
      parse_error("Expected closing parenthese on function call");
    }
    // handle function call
    return TRUE;
  }
  return FALSE;
}

int arrayidx() {
  if (tok_type == TT_LSQUARE) {
    if (expr()) {
      // handle expr
    } else {
      parse_error("Expected expression in array index");
    }
    if (tok_type != TT_RSQUARE) {
      parse_error("Expected ']'");
    }
    return TRUE;
  }
  return FALSE;
}

int factor() {
  if (tok_type == TT_MINUS) {
    // handle unary minus sign
    get_next_tok();
  }

  if (tok_type == TT_IDENTIFIER) {
    get_next_tok();
    if (funcarg()) {
      // handle a function call
    } else {
      // handle a variable name
    }
    while (arrayidx()) {
      // handle array indexing
    }
    // handle identifier
    return TRUE;
  } else if (tok_type == TT_INT_LITERAL) {
    get_next_tok();
    // handle int literal
    return TRUE;
  } else if (tok_type == TT_CHAR_LITERAL) {
    get_next_tok();
    // handle character literal
    return TRUE;
  } else if (tok_type == TT_TRUE_LITERAL) {
    get_next_tok();
    // handle true literal
    return TRUE;
  } else if (tok_type == TT_FALSE_LITERAL) {
    get_next_tok();
    // handle false literal
    return TRUE;
  } else if (tok_type == TT_LPAREN) {
    get_next_tok();
    if (!expr()) {
      parse_error("expected expression within parentheses");
    }
    if (tok_type != TT_RPAREN) {
      parse_error("expected right parentheses");
    }
    return TRUE;
  }
  return FALSE;
}

int term() { }
int expr9() { }
int expr8() { }
int expr7() { }
int expr6() { }
int expr5() { }
int expr4() { }
int expr3() { }
int expr2() { }
int expr1() { }
int expr() { }
int statement() { }
int ifblock() { }
int whileloop() { }
int bodyelem() { }
int body() { }
int funcargdef() { }
int type() { }
int def() { }
void parse_all() { }
void parser_init() { }

// END PARSER FUNCTIONS
//<<----------------------------------------------------------------------------

//>>----------------------------------------------------------------------------
// BEGIN MAIN FUNCTIONS

void print_tok_type() {
  if      (tok_type == TT_EOF              ) { printstr("TT_EOF"); }
  else if (tok_type == TT_ERROR            ) { printstr("TT_ERROR"); }
  else if (tok_type == TT_COMMENT          ) { printstr("TT_COMMENT"); }
  else if (tok_type == TT_WHITESPACE       ) { printstr("TT_WHITESPACE"); }
  else if (tok_type == TT_IDENTIFIER       ) { printstr("TT_IDENTIFIER"); }
  else if (tok_type == TT_INT_LITERAL      ) { printstr("TT_INT_LITERAL"); }
  else if (tok_type == TT_STRING_LITERAL   ) { printstr("TT_STRING_LITERAL"); }
  else if (tok_type == TT_CHARACTER_LITERAL) { printstr("TT_CHARACTER_LITERAL"); }
  else if (tok_type == TT_BOOL             ) { printstr("TT_BOOL"); }
  else if (tok_type == TT_BREAK            ) { printstr("TT_BREAK"); }
  else if (tok_type == TT_CHAR             ) { printstr("TT_CHAR"); }
  else if (tok_type == TT_CONST            ) { printstr("TT_CONST"); }
  else if (tok_type == TT_CONTINUE         ) { printstr("TT_CONTINUE"); }
  else if (tok_type == TT_ELSE             ) { printstr("TT_ELSE"); }
  else if (tok_type == TT_FALSE_LITERAL    ) { printstr("TT_FALSE_LITERAL"); }
  else if (tok_type == TT_IF               ) { printstr("TT_IF"); }
  else if (tok_type == TT_INT              ) { printstr("TT_INT"); }
  else if (tok_type == TT_NULL             ) { printstr("TT_NULL"); }
  else if (tok_type == TT_RETURN           ) { printstr("TT_RETURN"); }
  else if (tok_type == TT_TRUE_LITERAL     ) { printstr("TT_TRUE_LITERAL"); }
  else if (tok_type == TT_VOID             ) { printstr("TT_VOID"); }
  else if (tok_type == TT_WHILE            ) { printstr("TT_WHILE"); }
  else if (tok_type == TT_BITSHIFT_LEFT    ) { printstr("TT_BITSHIFT_LEFT"); }
  else if (tok_type == TT_BITSHIFT_RIGHT   ) { printstr("TT_BITSHIFT_RIGHT"); }
  else if (tok_type == TT_BITWISE_AND      ) { printstr("TT_BITWISE_AND"); }
  else if (tok_type == TT_ADDRESS_OF       ) { printstr("TT_ADDRESS_OF"); }
  else if (tok_type == TT_BITWISE_INVERT   ) { printstr("TT_BITWISE_INVERT"); }
  else if (tok_type == TT_BITWISE_OR       ) { printstr("TT_BITWISE_OR"); }
  else if (tok_type == TT_BITWISE_XOR      ) { printstr("TT_BITWISE_XOR"); }
  else if (tok_type == TT_BOOLEAN_AND      ) { printstr("TT_BOOLEAN_AND"); }
  else if (tok_type == TT_BOOLEAN_NOT      ) { printstr("TT_BOOLEAN_NOT"); }
  else if (tok_type == TT_BOOLEAN_OR       ) { printstr("TT_BOOLEAN_OR"); }
  else if (tok_type == TT_COMMA            ) { printstr("TT_COMMA"); }
  else if (tok_type == TT_DIVIDE           ) { printstr("TT_DIVIDE"); }
  else if (tok_type == TT_EQUALITY         ) { printstr("TT_EQUALITY"); }
  else if (tok_type == TT_EQUALS           ) { printstr("TT_EQUALS"); }
  else if (tok_type == TT_GREATER          ) { printstr("TT_GREATER"); }
  else if (tok_type == TT_GREATER_EQUAL    ) { printstr("TT_GREATER_EQUAL"); }
  else if (tok_type == TT_LESS             ) { printstr("TT_LESS"); }
  else if (tok_type == TT_LESS_EQUAL       ) { printstr("TT_LESS_EQUAL"); }
  else if (tok_type == TT_LCURLY           ) { printstr("TT_LCURLY"); }
  else if (tok_type == TT_LPAREN           ) { printstr("TT_LPAREN"); }
  else if (tok_type == TT_LSQUARE          ) { printstr("TT_LSQUARE"); }
  else if (tok_type == TT_MINUS            ) { printstr("TT_MINUS"); }
  else if (tok_type == TT_MODULUS          ) { printstr("TT_MODULUS"); }
  else if (tok_type == TT_MULTIPLY         ) { printstr("TT_MULTIPLY"); }
  else if (tok_type == TT_DEREFERENCE      ) { printstr("TT_DEREFERENCE"); }
  else if (tok_type == TT_NOT_EQUAL        ) { printstr("TT_NOT_EQUAL"); }
  else if (tok_type == TT_PLUS             ) { printstr("TT_PLUS"); }
  else if (tok_type == TT_RCURLY           ) { printstr("TT_RCURLY"); }
  else if (tok_type == TT_RPAREN           ) { printstr("TT_RPAREN"); }
  else if (tok_type == TT_RSQUARE          ) { printstr("TT_RSQUARE"); }
  else if (tok_type == TT_SEMICOLON        ) { printstr("TT_SEMICOLON"); }
}

void print_tok() {
  printstr("Token: loc=(");
  printint(lineno);
  printstr(", ");
  printint(column);
  printstr("; tok=(\"");
  printstr(tok_str);
  printstr("\", ");
  printint(tok_type);
  printstr(" (");
  print_tok_type();
  printstr("), ");
  printint(tok_int);
  printstr(")\n");
}

void print_all_toks() {
  get_next_tok();
  while (tok_type != TT_EOF) {
    print_tok();
    get_next_tok();
  }
}

int main(void) {
  scanner_init();
  print_all_toks();
  return 0;
}
