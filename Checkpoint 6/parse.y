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
struct statement *list;
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
%type <expr> exp
%type <expr> term
%type <expr> factor
%type <stmt> programbody
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

algsection: RWALG COLON programbody {
            list = $3;
            };

programbody: assignstmt programbody {
             insertStmt($1);
             }
            |endmainstmt;

assignstmt: VAR assign exp SEMICOLON
            {
              $$ = malloc(sizeof(struct statement));
              if(DEBUG) printf("%d, VAR: %s\n", isPresent($1), $1);
              $$->target = &table->table[isPresent($1)];
              if(DEBUG) printf("target->name: %s\n", $$->target->name);
              $$->exp = malloc(sizeof(struct ast_expression));
              $$->exp->kind = $3->kind;
              $$->exp->operator = $3->operator;
              $$->exp->value = $3->value;
              $$->exp->address = $$->target->address;

              $$->exp->l_operand = $3->l_operand; // Added because portion under exp wasn't working
              $$->exp->r_operand = $3->r_operand;
              // Will need code to insert this into a linked list
              //insertStmt($$);

            };

assign: ASSIGNOP;

exp: MINUS exp  { // Unary minus(TEST THIS)
                  if(DEBUG) printf("GOT HERE1\n");
                  $$ = malloc(sizeof(struct ast_expression));
    }
    |exp ADD term {// Code to parse expressions
      if(DEBUG) printf("GOT HERE2\n");
      $$ = malloc(sizeof(struct ast_expression));
      $$->l_operand = $1;
    //  printf("l_op %d\n", $$->l_operand->value);
      $$->kind = KIND_OP;
      $$->operator = OP_ADD;
      $$->r_operand = $3;
    //  printf("r_op %d\n", $$->r_operand->value);
    }
    |exp MINUS term {
      if(DEBUG) printf("GOT HERE3\n");
      $$ = malloc(sizeof(struct ast_expression));
    }
    |term {
      if(DEBUG) printf("GOT HERE4\n");
      //$$->kind = KIND_OP;
      $$ = $1;
    }
    ;

term: term MULT factor
     |term DIV factor
     |factor {
      if(DEBUG) printf("GOT HERE44\n");
      //$$->kind = KIND_OP;
      $$ = $1;
     }
     ;

factor: LITINT {
        if(DEBUG) printf("%d\n", $1);
        $$ = malloc(sizeof(struct ast_expression));
        $$->kind = KIND_INT;
        $$->value = $1;
       }
       |LITREAL
       |LPAREN exp RPAREN
       ;

endmainstmt: RWEND RWMAIN SEMICOLON;

%%
int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
