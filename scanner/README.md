# Scanner

A scanner is code that performs lexical analysis on source code to transform it
into tokens.

The first task for this is to define the syntax of the C language that I will
support.  Note, I do not support the full C language.  I support only a subset
of the official C language for simplicity and ease of design.

## Supported Syntax

### Preprocessing Directives

Preprocess directives will be supported, so \# followed by a preprocessing
directive name.  The preprocessing directives that are supported are

- `#include <FNAME>`
- `#include "FNAME"`
- `#ifdef VARNAME`
- `#ifndef VARNAME`
- `#else`
- `#endif`
- `#define VARNAME`
- `#define VARNAME VALUE`

Note, these preprocessing directives are not case sensative, but the `FNAME`,
`VARNAME` and `VALUE` are.  The two `#include` variants are different, where
the `<FNAME>` variant is a system include, so the scanner should distinguish
this in some way from the other one.

### Arithmetic and Operators

There are operators that I want to support:

- `+` addition
- `-` subtraction
- `*` multiplication
- `/` division
- `%` modulus
- `(` and `)` parentheses
- `--` decrement (both pre and post)
- `++` increment (both pre and post)

Then there are boolean comparison operators:

- `==` equality
- `!=` not equal
- `<=` less than or equal to
- `>=` greater than or equal to
- `&&` boolean and
- `||` boolean or
- `!` not operator

Now for operators on pointers and structs:

- `&` address of operator
- `.` to access an element of a struct
- `->` to access an element of a struct pointer
- `*` to dereference a pointer
- `[` and `]` with an integer in-between to index an array

Miscelaneous operators:

- ',' comma operator.  Needed for function declarations, definitions, and
  calls.  Other than that, I do not support other uses of the comma operator.

After that, we have bitwise operators.  Not sure if I want to include these in
the subset of the language I want to support.

- `>>` bitshift right
- `<<` bitshift left
- `^` bitwise xor
- `&` bitwise and
- `|` bitwise or
- `~` bitwise invert bits

Ternary operators are for now not supported:

- `:`
- `?`

Supported assignment operators

- `=` assignment

Not supported assignment operators

- `+=` plus equals
- `-=` minus equals
- `*=` times equals
- `/=` divide equals
- `%=` mod equals
- `<<=` left shift equals
- `>>=` right shift equals
- `&=` bitwise and equals
- `^=` bitwise xor equals
- `|=` bitwise or equals

### Scope Specification

There are directives that specify hints to the compiler and the type of scope.  This is what is supported:

- `extern`
- `const`

None of these are supported.

- `auto`
- `register`
- `volatile`
- `static`
- `_Packed`

### Types

The basic types that are supported are

- `void`
- `char`
- `int`
- `bool`

There are some types I don't think I will support:

- `short`
- `byte`
- `float`
- `double`
- `long`
- `long long`
- `long double`
- `unsigned` with anything
- `signed` with anything

Custom types are possible:

- `struct`
- `typedef`

Those not supported are

- `union`

Associated with type is the special function

- `sizeof` (called by `sizeof(TYPE)`)

### Statements

Each statement is separated by a semicolon `;`.

### Scope, Functions, and Control Flow

All scoped environments, including control flow use curly braces.  In C,
control flow statements optionally use curly braces, but do not have to if
there is only one statement within their scope.  That functionality will be
left out of the supported functionality for my compiler.

For a function, we simply use types, identifier, parentheses with types and
then curly braces.

- `{` and `}` curly braces to denote scope

Control flows that are supported

- `if`
- `else`
- `while`
- `break`
- `continue`

Those that are not supported:

- `for`
- `do` as in `do..while`
- `switch`
- `goto`
- `default

Note that since the `goto` statement is not supported, then also the feature to
label lines is not supported.

At the end of a function, there is a

- `return` statement

### Comments

There are two styles of comments:

- `// ...` ended by a newline
- `/* ... */` which can have newlines inside

### Boolean Values

The C language does not support the `bool` type, but my language will.  There
will be two values,

- `true`
- `false`

### NULL type

The value of null pointers is `NULL`.  This is a simple alias to zero.

### All Keywords

Some keywords were specified earlier, but here is the full complete list,
sorted alphabetically

- `bool`
- `break`
- `char`
- `const`
- `continue`
- `else`
- `enum`
- `if`
- `int`
- `return`
- `sizeof`
- `struct`
- `typedef`
- `void`
- `while`
- `true`
- `false`

Not supported keywords:

- `\_Packed`
- `and`
- `auto`
- `byte`
- `case`
- `default`
- `do`, as in `do`..`while`
- `double`
- `extern`
- `float`
- `for`
- `goto`
- `long double`
- `long long`
- `long`
- `or`
- `register`
- `short`
- `signed`
- `static`
- `switch`
- `union`
- `unsigned`
- `volatile`


### Identifiers

Custom identifiers, such as defined `struct`s, `typedef`s, `enum`s, and user
variables.  These identifiers start with a letter or an underscore and are made
of letters, numbers, and underscores.

### Numbers

These are identifiers that begin with a digit.  They can begin with a unary `+`
or `-` operator.

Not supported are hexadecimal (`0x...`) or binary (`0b...`) representations.

### Strings

Begin and end with `"`.  Inside of there, any valid character is allowed, but
additionally, to get a literal `"`, you need to escape it with backslash:
`"\""`, but you do not need to escape single quote characters `"'"`.

### Characters

To specify a single character value, it will be within single quotes, e.g.
`'a'` to represent the character "a".  There are some characters you need to
escape to represent:

- `\"` double quote
- `\'` single quote
- `\\` a literal backslash
- `\a` audible bell (0x07)
- `\b` backspace (0x08)
- `\f` form feed - new page (0x0c)
- `\n` newline (0x0a)
- `\r` carriage return (0x0d)
- `\t` horizontal tab (0x09)
- `\v` vertical tab (0x0b)

Escape characters not supported are

- `\?` (used to not be ambiguous with trigraphs which are not supported)
- `\nnn` where nnn is an octal value
- `\unnnn` where nnnn is a unicode sequence
- `\Unnnnnnn` where nnnnnnn is a unicode sequence of several characters

Trigraphs are not supported.

### Whitespace

Whitespace is generally ignored except

- To end line-style comments (with `//`)
- to separate identifiers and other tokens (not always necessary)

Whitespace is comprised of

- space
- tab
- newline

