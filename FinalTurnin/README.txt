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
