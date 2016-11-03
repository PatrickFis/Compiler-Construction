%{

/*
 * ========================================================================
 *
 * parse.y - Bison parser for Slic.
 * Fixed my symbol table code to allow for insertion of a comma separated list.
 * Added abstract syntax tree and code generation in ast.h and ast.c.
 *
 * Programmer --- Patrick Fischer
 *
 * ========================================================================
 */

#include "stable.h"
#include "ast.h"
#include <stdio.h>
#define DEBUG 1
struct symbol_table *table;
struct statement *list;
%}

%union {
   char *sval;
   int ival;
   double rval;
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
%token <rval> LITREAL
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
%type <expr> assignstmt
%type <expr> exp
%type <expr> term
%type <expr> factor
%type <expr> unit
%type <expr> rexp
%type <expr> bexp
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

decstmt: RWINT COLON varlist {  //$$ = malloc(sizeof(struct symbol_table_entry));
                                  $3 = malloc(sizeof(struct symbol_table_entry));
                                  $3->type = TYPE_INT;
                                //$$->type=TYPE_INT;
                                //$$->name=$3->name;
                                //$$->kind = $3->kind;
                                //$$->address = $3->address;
                                //$$->size = $3->size;
                                //insert(*$$);
                                }
        |RWREAL COLON varlist { $3 = malloc(sizeof(struct symbol_table_entry));
                                $3->type=TYPE_REAL;
                                //$$->name=$3->name;
                                //$$->kind = $3->kind;
                                //$$->address = $3->address;
                                //$$->size = $3->size;
                                //insert(*$$);
                                }
        ; // The reserved word is followed by a list because any number of variables can be declared on a single line.

varlist: varref COMMA varlist {
                                $$->name = $1->name;
                                $$->kind = $1->kind;
                                $$->address = 0;
                                $$->size = $1->size;
                                insert(*$$);
                                if(DEBUG) printf("$$->type: %d\n", $$->type);
                              }
        |varref SEMICOLON {
          $$->name = $1->name;
          $$->kind = $1->kind;
          $$->address = 0;
          $$->size = $1->size;
          insert(*$$);
          if(DEBUG) printf("$$->type: %d\n", $$->type);
        }
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

algsection: RWALG COLON programbody endmainstmt{
            list = $3;
            };

programbody: assignstmt programbody { // Multiple assignments, removed endmainstmt from programbody to stop a segfault
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              $$->link = $2;
              //insertStmt($$);
             }
            |assignstmt { // Only one assignment
              // printf("Only one assignment");
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->exp = $$->exp;
              temp->link = NULL;
              $$->link = temp;
            }
            ;

assignstmt: VAR assign bexp SEMICOLON
            {
              // This appears to to be working
              if(DEBUG) printf("Got to assignstmt\n");
              $$ = malloc(sizeof(struct ast_expression));
              $$->kind = KIND_OP;
              $$->operator = OP_ASGN;
              // $$->l_operand = NULL; // Gonna set this to NULL and use r_operand for the exp
              $$->r_operand = $3;
              $$->target = &table->table[isPresent($1)]; // Get target from symbol table
              $$->address = $$->target->address;
              if(DEBUG) printf("Address %d\n", $$->address);
            };

assign: ASSIGNOP;

bexp: bexp AND rexp { // Code to parse & booleans, bexp = Boolean Expression
                    if(DEBUG) printf("Got to AND\n");
                    $$ = malloc(sizeof(struct ast_expression));
                    $$->kind = KIND_OP;
                    $$->operator = OP_AND;
                    $$->l_operand = $1;
                    $$->r_operand = $3;
                  }
      |bexp OR rexp { // Code to parse | booleans
                  if(DEBUG) printf("Got to OR\n");
                  $$ = malloc(sizeof(struct ast_expression));
                  $$->kind = KIND_OP;
                  $$->operator = OP_OR;
                  $$->l_operand = $1;
                  $$->r_operand = $3;
                 }
      |NOT rexp { // Code to parse ~ booleans.
        if(DEBUG) printf("Got to NOT\n");
        $$ = malloc(sizeof(struct ast_expression));
        $$->kind = KIND_OP;
        $$->operator = OP_NOT;
        $$->r_operand = $2;
      }
      |rexp {
        $$ = $1;
      }
      ;

rexp: rexp LESS exp { // Code to parse < booleans, rexp = Relation Expression
                      if(DEBUG) printf("Got to LESS\n");
                      $$ = malloc(sizeof(struct ast_expression));
                      $$->kind = KIND_OP;
                      $$->operator = OP_LSTHN;
                      $$->l_operand = $1;
                      $$->r_operand = $3;
                   }
    |rexp LESSEQU exp { // Code to parse <= booleans
                        if(DEBUG) printf("Got to LESSTHN\n");
                        $$ = malloc(sizeof(struct ast_expression));
                        $$->kind = KIND_OP;
                        $$->operator = OP_LSTHNEQL;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                      }
    |rexp GREATER exp { // Code to parse > booleans
                        if(DEBUG) printf("Got to GREATER\n");
                        $$ = malloc(sizeof(struct ast_expression));
                        $$->kind = KIND_OP;
                        $$->operator = OP_GRTHN;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                      }
    |rexp GREATEQU exp { // Code to parse >= booleans
                        if(DEBUG) printf("Got to GREATEQU\n");
                        $$ = malloc(sizeof(struct ast_expression));
                        $$->kind = KIND_OP;
                        $$->operator = OP_GRTHNEQL;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                       }
    |rexp EQUAL exp { // Code to parse = booleans
                      if(DEBUG) printf("Got to EQUAL\n");
                      $$ = malloc(sizeof(struct ast_expression));
                      $$->kind = KIND_OP;
                      $$->operator = OP_EQUAL;
                      $$->l_operand = $1;
                      $$->r_operand = $3;
                    }
    |rexp NOTEQUAL exp { // Code to parse <> booleans
                        if(DEBUG) printf("Got to NOTEQUAL\n");
                        $$ = malloc(sizeof(struct ast_expression));
                        $$->kind = KIND_OP;
                        $$->operator = OP_NEQUAL;
                        $$->l_operand = $1;
                        $$->r_operand = $3;
                       }
                       ;
    |exp {
      if(DEBUG) printf("Got to exp\n");
      //$$ = malloc(sizeof(struct ast_expression));
      //$$->kind = KIND_OP;
      //$$->l_operand = $1;
      $$ = $1;
    }

exp: exp ADD term {// Code to parse expressions
                    if(DEBUG) printf("GOT HERE2\n");
                    $$ = malloc(sizeof(struct ast_expression));
                    $$->kind = KIND_OP;
                    $$->operator = OP_ADD;
                    $$->l_operand = $1;
                    $$->r_operand = $3;
                  }
    |exp MINUS term {
                      if(DEBUG) printf("GOT HERE3\n");
                      $$ = malloc(sizeof(struct ast_expression));
                      $$->kind = KIND_OP;
                      $$->operator = OP_SUB;
                      $$->l_operand = $1;
                      $$->r_operand = $3;
                    }
    |term {
            if(DEBUG) printf("Got to term\n");
            //$$ = malloc(sizeof(struct ast_expression));
            //$$->kind = KIND_OP;
            //$$->l_operand = $1;
            $$ = $1;
          }
          ;

term: term MULT factor {
                          $$ = malloc(sizeof(struct ast_expression));
                          $$->kind = KIND_OP;
                          $$->operator = OP_MUL;
                          $$->l_operand = $1;
                          $$->r_operand = $3;
                       }
     |term DIV factor {
                          $$ = malloc(sizeof(struct ast_expression));
                          $$->kind = KIND_OP;
                          $$->operator = OP_DIV;
                          $$->l_operand = $1;
                          $$->r_operand = $3;
                      }
     |factor {
              if(DEBUG) printf("Got to factor\n");
                //$$ = malloc(sizeof(struct ast_expression));
                //$$->kind = KIND_OP;
                //$$->l_operand = $1;
                $$ = $1;
             }
             ;
factor: MINUS unit {
                    if(DEBUG) printf("Got to negation\n");
                    $$ = malloc(sizeof(struct ast_expression));
                    $$->kind = KIND_OP;
                    $$->operator = OP_UMIN;
                    $$->r_operand = $2;
                   }
       |unit {
              if(DEBUG) printf("Got to unit\n");
              $$ = $1;
             }
             ;

unit: LITINT { // Parses integers
                if(DEBUG) printf("%d\n", $1);
                  $$ = malloc(sizeof(struct ast_expression));
                  $$->kind = KIND_INT;
                  $$->value = $1;
                  $$->type = TYPE_INT;
                  $$->r_operand = NULL; // Last ditch effort
                  $$->l_operand = NULL;
               }
       |LITREAL { // Parses reals
                  if(DEBUG) printf("%f\n", $1);
                  $$ = malloc(sizeof(struct ast_expression));
                  $$->kind = KIND_REAL;
                  $$->rvalue = $1;
                  $$->type = TYPE_REAL;
                  $$->r_operand = NULL;
                  $$->l_operand = NULL;
                }
       |LPAREN bexp RPAREN {
                            $2 = malloc(sizeof(struct ast_expression));
                            $$ = $2;
                          }
       |VAR {
         if(DEBUG) printf("Got to VAR\n");
         $$ = malloc(sizeof(struct ast_expression));
         $$->type = TYPE_VAR;
         int tableLoc = isPresent($1);
         if(DEBUG) printf("tableLoc: %d\n", tableLoc);
         int varType = table->table[tableLoc].type;
         if(DEBUG) printf("varType: %d\n", varType);
         if(varType == TYPE_INT) {
           $$->l_operand = malloc(sizeof(struct ast_expression)); // Trying to hack together a solution with this
          //  $$->l_operand->kind = KIND_INT;
           $$->l_operand->type = TYPE_VAR;
           $$->l_operand->target = &table->table[tableLoc];
         }
         if(varType == TYPE_REAL) {
           $$->l_operand = malloc(sizeof(struct ast_expression)); // Trying to hack together a solution with this
          //  $$->l_operand->kind = KIND_REAL;
           $$->l_operand->type = TYPE_VAR;
           $$->l_operand->target = &table->table[tableLoc];
         }
       }
                          ;

endmainstmt: RWEND RWMAIN SEMICOLON;

%%
int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
