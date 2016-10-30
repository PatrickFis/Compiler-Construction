%{

/*
 * ========================================================================
 *
 * parse.y - Bison parser for Slic.
 * bison -v will produce a .output file that will help with shift/reduce
 * errors.
 * Noticed that starting non-terminal and algsection both had a reference
 * to endmainstmt. Removed from starting non-terminal.
 * There may be a shift/reduce conflict with my assignstmt.
 * Added code to add variables to symbol table defined in stable.h.
 * Programmer --- Patrick Fischer
 *
 * ========================================================================
 */

#include "stable.h"
#include "ast.h"
#include <stdio.h>
#define DEBUG 0
struct symbol_table *table;
%}

%union {
   char *sval;
   int ival;
   struct symbol_table *tableptr;
   struct symbol_table_entry *entry;
   struct statement *stmt;
   struct ast_expression *expr;
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
%token <ival> LITINT
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
%token <sval> VAR
%token <sval> VARIABLE

%type <entry> decstmt
%type <entry> varlist
%type <entry> varref
%type <stmt> assignstmt
%type <sval> varname
%%

program : headingstmt datasection algsection;

headingstmt: RWMAIN SEMICOLON;

datasection: RWDATA COLON
            |RWDATA COLON decstmtlist
            ; // decstmtlist is a listing of the variables used by this Slic program.

decstmtlist: decstmtlist decstmt
            |decstmt
            ; // decstmt is just a variable declaration.

decstmt: RWINT COLON varlist {  $$ = malloc(sizeof(struct symbol_table_entry));
                                $$->type=TYPE_INT;
                                $$->name=$3->name;
                                $$->kind = $3->kind;
                                $$->address = $3->address;
                                $$->size = $3->size;
                                insert(*$$);
                                }
        |RWREAL COLON varlist { $$ = malloc(sizeof(struct symbol_table_entry));
                                $$->type=TYPE_REAL;
                                $$->name=$3->name;
                                $$->kind = $3->kind;
                                $$->address = $3->address;
                                $$->size = $3->size;
                                insert(*$$);
                                }
        ; // The reserved word is followed by a list because any number of variables can be declared on a single line.

varlist: varref COMMA varlist
        |varref SEMICOLON
        ;

varref: VAR { $$ = malloc(sizeof(struct symbol_table_entry));
              $$->name = $1;
              if(DEBUG) printf("$1 = %s\n", $1);
              $$->address = 0;
              $$->kind = KIND_SCALAR;
              $$->size = 1;
              }
       |VAR LBRACK LITINT RBRACK {
                                 $$ = malloc(sizeof(struct symbol_table_entry));
                                 $$->name = $1;
                                 $$->address = 0;
                                 $$->kind = KIND_ARRAY;
                                 $$->size = $3;
                                 }
       ; // varref refers to the VAR token or an array.

algsection: RWALG COLON programbody;

programbody: assignstmt programbody
            |endmainstmt;

assignstmt: VAR assign exp SEMICOLON
            {
              // So the retrieve function isn't working because isPresent isn't finding the correct variable...
              $$ = malloc(sizeof(struct statement));
              int tableLoc = isPresent($1); // Location of VAR in symbol table
              printf("%d, VAR: %s\n", tableLoc, $1);
              $$->target = &table->table[tableLoc]; // Why is this tableLoc - 1?
              //printf("target->name: %s\n", $$->target->name);
              // Will need code to insert this into a linked list
            };

varname: VAR;

assign: ASSIGNOP;

exp: MINUS exp // Unary minus(TEST THIS)
    |exp ADD term // Code to parse expressions
    |exp MINUS term
    |term
    ;

term: term MULT factor
     |term DIV factor
     |factor
     ;

factor: LITINT
       |LITREAL
       |LPAREN exp RPAREN
       ;

endmainstmt: RWEND RWMAIN SEMICOLON;

%%
int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
