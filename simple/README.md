# Simple Compiler

The other attempt I had made was far too complicated and too ambitious, trying
to incorporate a preprocessor, a scanner, and a processor, all completely unit
tested and passing around the pointers.

I found that my attempt required a lot of handling of dynamic memory
allocation, which may be overkill for such a small project.  It was good
exercise for managing a larger project, but for this self-compiling compiler, I
want simplicity.

Therefore, I am going to implement the scanner, parser, and compiler all in a
single file.  Maybe a little crazy, but I would no longer require a
preprocessor.  Also, I would have the possibility of only requiring the use of
`getchar(void)` and `putchar(char ch)` from the standard library.  No other
library functions will be used, including any header files.
Note: `getchar()` grabs a single character from the standard input, so the usage of this compiler will be

```bash
$ simple-compiler < source.c > a.out
```

and this will output the generated machine code to standard output, so you will
want to capture the output into a file (as seen above being placed into
`a.out`).Therefore, I have also simplified the command-line parsing to be
completely non-existent.  We don't even need to implement error checking for a
missing source file or failure to open the output file!

With this approach, I am no longer going to be able to use `bool`.  And I will
need to define `true` and `false` as integer constants.  I will also not need
to implement local variables, structs, or enums.  All variables will be basic
types (except for floating-point) and will be global variables.  The only type
of local variable will be those that are sent as parameters into functions (if
any, which I will attempt to leave to a minimum).

This should make the subset that is supported much easier to manage.  It may be
tricky to create a lookup table of keywords.  For that, I could use an already
sorted list of `char*`, or a sorted list of `char**`, or even an `if..else`
chain.  The first two would be implemented with a binary search algorithm,
whereas the last one is a manual approach to a linear search.

One of the beauties of this approach is that the functions will be clean and
will follow the grammar rules of recursive descent, even for the scanner.  I
will implement the scanner as a recursive descent algorithm, even though it can
be expressed as a regular expression.
