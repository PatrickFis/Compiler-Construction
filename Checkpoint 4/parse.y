%{

/*
 * ========================================================================
 *
 * parse.y - Bison parser for Slic.
 *
 * Programmer --- Patrick Fischer
 * Looks like I need to add something for a newline.
 * I need to change my list syntax. List is not a reserved word.
 * arrayname[exp] should instead be arrayname[LITINT];
 * Note: This parser assumes that the algsection is blank.
 * Should look like main;, data: (with variables, etc), blank algorithm:,
 * and then an end main;.
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

program : headingstmt datasection algsection endstmt;

headingstmt: RWMAIN SEMICOLON;

datasection: RWDATA COLON
            |RWDATA COLON decstmtlist
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
varref: VAR
       |VAR LBRACK LITINT RBRACK SEMICOLON;
       ; // varref refers to the VAR token or a list statement.

algsection: RWALG COLON;

endstmt: RWEND RWMAIN SEMICOLON
        |RWEND RWIF SEMICOLON
        |RWEND RWWHILE SEMICOLON
        |RWEND RWCOUNTING SEMICOLON
        ;

%%

int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
