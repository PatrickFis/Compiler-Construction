Compile using make. Compiled code can be deleted with make clean.
The compiler can be run using ./a.out, or ./a.exe and then piping in a file.

File list:
ast.c - Abstract syntax tree. Contains code to generate GSTAL instructions from
the abstract syntax tree.
ast.h - Abstract syntax tree header file.
bisonHelper.c - This is a helper file that I wrote to add some more functions
to my parser. I only ended up adding a function that would assign targets
from the symbol table to assignment statements.
bisonHelper.h - Bison helper header file.
flex.l - Flex input file.
main.c - Contains code to call yyparse(), and then prints out the GSTAL
instructions.
makefile - Contains code for make. Included are various different options,
including some for debugging.
parse.y - Bison input file, contains code for parsing.
stable.c - Symbol table. Contains code to insert variables into the symbol
table.
stable.h - Symbol table header file.

Issues:
The only issue I ran into was trying to use a counting loop with a counting variable
that was a floating point array reference. Since the documentation states that the
counting variable will be a scalar integer variable(and since I only found the error
a few hours before turn in), I did not implement a fix for this. It works with
integer variables, floating point variables, and integer array references.
