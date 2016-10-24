%{

/*
 * ========================================================================
 *
 * parse.y - Bison parser for Slic.
 *
 * Programmer --- Patrick Fischer
 *
 * ========================================================================
 */

#include "stable.h"
#include <stdio.h>
struct symbol_table *table;
%}

%union {
   char *sval;
   struct symbol_table *tableptr;
   struct symbol_table_entry *entry;
}

%token        RWMAIN
%token        RWEXIT
%token        RWIF
%token        RWELSE
%token        RWWHILE
%token        RWCOUNTING
%token        RWEND
%token        RWDATA
%token        RWALG
%token        LITINT
%token        LITREAL
%token        RWINT
%token        RWREAL
%token        RWUPWARD
%token        RWDOWNWARD
%token        RWTO
%token        RWREAD
%token        RWPRINT
%token        CHARSTRING
%token        CARRETURN
%token        ASSIGNOP
%token        COMMA
%token        SEMICOLON
%token        COLON
%token        LPAREN
%token        RPAREN
%token        LBRACK
%token        RBRACK
%token        ADD
%token        MINUS
%token        MULT
%token        DIV
%token        LESS
%token        GREATER
%token        LESSEQU
%token        GREATEQU
%token        EQUAL
%token        NOTEQUAL
%token        AND
%token        OR
%token        NOT
%token        VAR
%token <sval> VARIABLE

%type <tableptr> datasection
%type <entry> varref
%%

program : headingstmt datasection algsection endmainstmt;

headingstmt: RWMAIN SEMICOLON;

datasection: RWDATA COLON { table = $$; table = malloc(sizeof(struct symbol_table));}
            |RWDATA COLON  decstmtlist { table = $$; table = malloc(sizeof(struct symbol_table));}
            ; // decstmtlist is a listing of the variables used by this Slic program.

decstmtlist: decstmtlist decstmt
            |decstmt
            ; // decstmt is just a variable declaration.

decstmt: RWINT COLON varlist
        |RWREAL COLON varlist
        ; // The reserved word is followed by a list because any number of variables can be declared on a single line.

varlist: varref COMMA varlist
        |varref SEMICOLON
        ;

varref: VAR { $$ = malloc(sizeof(struct symbol_table_entry));
              $$->name = "Test";
              $$->address = 2;
              $$->kind = 1;
              $$->type = TYPE_INT;
              $$->size = 1;
              insert(*$$);
              }
       |VAR LBRACK LITINT RBRACK
       ; // varref refers to the VAR token or an array.

algsection: RWALG COLON;

endmainstmt: RWEND RWMAIN SEMICOLON;

%%
int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
