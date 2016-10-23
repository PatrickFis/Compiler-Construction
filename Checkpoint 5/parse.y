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

#include <stdio.h>
#include "stable.h"
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
%type <entry> decstmtlist
%type <entry> decstmt
%%

program : headingstmt datasection algsection endmainstmt;

headingstmt: RWMAIN SEMICOLON;

datasection: RWDATA COLON { table = $$; table = malloc(sizeof(struct symbol_table));}
            |RWDATA COLON  decstmtlist { table = $$; table = malloc(sizeof(struct symbol_table));}
            ; // decstmtlist is a listing of the variables used by this Slic program.

decstmtlist: decstmtlist decstmt { $2 = malloc(sizeof(struct symbol_table_entry));}
            |decstmt { $1 = malloc(sizeof(struct symbol_table_entry));}
            ; // decstmt is just a variable declaration.

decstmt: RWINT COLON varlist { printf("%s", $1); $$->type = TYPE_INT;}
        |RWREAL COLON varlist { $$->type = TYPE_REAL;}
        ; // The reserved word is followed by a list because any number of variables can be declared on a single line.

varlist: varref COMMA varlist
        |varref SEMICOLON
        ;

varref: VAR
       |VAR LBRACK LITINT RBRACK
       ; // varref refers to the VAR token or an array.

algsection: RWALG COLON;

endmainstmt: RWEND RWMAIN SEMICOLON;

%%
int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
