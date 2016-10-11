%{

/*
 * ========================================================================
 *
 * parse.y - Bison parser for Slic.
 *
 * Programmer --- Patrick Fischer
 * Looks like I need to add something for a newline.
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

headingstmt: RWMAIN SEMICOLON;

datasection: DATA COLON
            |DATA COLON decstmtlist
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
       |liststmt
       ; // varref refers to the VAR token or a list statement.

liststmt: RWLIST LBRACK exp RBRACK SEMICOLON;

algsection: RWALG COLON programbody; // programbody will contain actual code

programbody: assignstmt programbody // assignstmt is for assigning values to variables
            |repstmt programbody// repstmt is for while and counting loops
            |controlstmt programbody // controlstmt is for if and else statements
            |inputstmt programbody // inputstmt is for read statements
            |outputstmt programbody // outputstmt is for print statements
            |exitstmt // exitstmt is for the reserved word exit
            ;

assignstmt: varref assign exp SEMICOLON;

assign: ASSIGNOP; // assign derives the token for :=

repstmt: RWWHILE condstmt SEMICOLON
        |RWCOUNTING varref RWUPWARD varref RWTO varref SEMICOLON
        |RWCOUNTING varref RWDOWNWARD varref RWTO varref SEMICOLON
        |endstmt SEMICOLON
        ;

controlstmt: RWIF condstmt SEMICOLON // condstmt is a conditional
            |RWELSE SEMICOLON
            |endstmt SEMICOLON
            ; // endstmt will be declared later for end if, while, counting, etc.

conditionals: LESS
             |GREATER
             |LESSEQU
             |GREATEQU
             |EQUAL
             |NOTEQUAL
             |AND
             |OR
             |NOT
             ;

condstmt: exp conditionals exp;

inputstmt: RWREAD VAR SEMICOLON;

outputstmt: RWPRINT printlist; // printlist will be used to make a large string

printlist: CHARSTRING printlist
          |CARRETURN printlist
          |CARRETURN COMMA printlist
          |SEMICOLON
          ;

exitstmt: RWEXIT SEMICOLON;

endstmt: RWEND RWMAIN SEMICOLON
        |RWEND RWIF SEMICOLON
        |RWEND RWWHILE SEMICOLON
        |RWEND RWCOUNTING SEMICOLON
        ;

exp: exp ADD term // Code to parse expressions
    |exp MINUS term
    |term
    ;

term: term MULT factor
     |term DIV factor
     |factor
     ;

factor: atom
       |LPAREN exp RPAREN
       ;

commentstmt: COMMENT;

%%

int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
