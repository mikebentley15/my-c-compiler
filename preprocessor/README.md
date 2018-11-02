# Preprocessor

A preprocessor is code that find preprocessor directives (in the form of a
lexical analysis) on the source code (as a stream of characters) and transforms
them into a new stream of characters, after being processed.

A prime example is from the C programming language:

```c
#include <stdio.h>
```

This is a preprocessing directive that says to insert all characters from
`stdio.h` into this location in the file.

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
this in some way from the other one.  Also, there can be any whitespace between
the `#` sign and the directive, except for line feeds and newlines (meaning
spaces and tabs are allowed).

Macro defined functions are not supported, just like pragmas and other macros
not defined above.

The preprocessor here utilizes the scanner to identify these tokens and expands
them.  The expansion is used as the output from the preprocessor into the
scanner for later parsing.

This preprocessing step already requires some parsing, limited though it may
be.


### Comments

There are two styles of comments:

- `// ...` ended by a newline
- `/* ... */` which can have newlines inside

Comments are eaten by the preprocessor and are not passed on to the scanner.


### NULL type

`NULL` is a macro variable that is predefined as a literal 0.  It will simply
be replaced with a 0 literal before being passed to the scanner, just like any
other macro-defined variable (simple find and replace).


### Predefined Macro Variables

There are a number of predefined macro variables.  Some of these are standard while others are nonstandard.

#### Supported Standard Predefined Macro Variables

- `__FILE__`: the current filepath in double quotes
- `__LINE__`: the raw line number as an integer
- `__STDC__`: is 1 if the compiler conformed to ISO Standard C, and 0
  otherwise.  For this implementation, this maps to 0.
- `__STDC_VERSION__`: expands to an integer for the ISO Standard C that is
  implemented.  The format is yyyymmL.  For this implementation, it will be
  replace with 0.
- `__STDC_HOSTED__`: is 1 if the environment has the complete standard C
  library.  In this implementation, this maps to 0.

#### Unsupported Standard Predefined Macro Variables

- `__DATE__`: the current date in double quotes (as of preprocessing)
- `__TIME__`: the current time in double quotes (as of preprocessing)

