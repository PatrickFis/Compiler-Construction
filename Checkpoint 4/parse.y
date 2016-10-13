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
%}

%union {
   char *sval;
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
%token        NEWLINE
%token <sval> VARIABLE

%%

program : headingstmt datasection algsection endmainstmt;

headingstmt: RWMAIN SEMICOLON NEWLINE;

datasection: RWDATA COLON NEWLINE
            |RWDATA COLON NEWLINE decstmtlist
            ; // decstmtlist is a listing of the variables used by this Slic program.

decstmtlist: decstmtlist decstmt
            |decstmt
            ; // decstmt is just a variable declaration.

decstmt: RWINT COLON varlist
        |RWREAL COLON varlist
        ; // The reserved word is followed by a list because any number of variables can be declared on a single line.

varlist: varref COMMA varlist
        |varref SEMICOLON NEWLINE
        |varref SEMICOLON
        ;

varref: VAR
       |VAR LBRACK LITINT RBRACK
       ; // varref refers to the VAR token or an array.

algsection: RWALG COLON NEWLINE;

endmainstmt: RWEND RWMAIN SEMICOLON NEWLINE;

%%
int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
