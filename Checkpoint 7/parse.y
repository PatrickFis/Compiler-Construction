%{

/*
 * ========================================================================
 *
 * parse.y - Bison parser for Slic.
 * Fixed my symbol table code to allow for insertion of a comma separated list.
 * Added abstract syntax tree and code generation in ast.h and ast.c.
 *
 * Add print statements, if statements, and if/else statements. Also implement
 * code to parse array references.
 *
 * Programmer --- Patrick Fischer
 *
 * ========================================================================
 */

#include "stable.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>
#define DEBUG 0
struct symbol_table *table;
struct statement *list;
int entry_count = 0; // Used to keep track of what symbols are being inserted.
// Variable type will be changed based on entry_count.
%}

%union {
   char *sval;
   int ival;
   double rval;
   struct symbol_table *tableptr;
   struct symbol_table_entry *entry;
   struct statement *stmt;
   struct ast_expression *expr;
   struct ast_if_stmt *ifstmt;
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
%token <sval> CHARSTRING
%token <sval> CARRETURN
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
%type <expr> outputstmt
%type <expr> printlist
%type <expr> inputstmt
%type <ifstmt> controlstmt
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
                                  if(DEBUG) printf("$3->type: %d\n",$3->type);
                                  int i;
                                  for(i = table->count - entry_count; i < table->count; i++) {
                                    table->table[i].type = TYPE_INT;
                                  }
                                  entry_count = 0;
                                //$$->type=TYPE_INT;
                                //$$->name=$3->name;
                                //$$->kind = $3->kind;
                                //$$->address = $3->address;
                                //$$->size = $3->size;
                                //insert(*$$);
                                }
        |RWREAL COLON varlist { $3 = malloc(sizeof(struct symbol_table_entry));
                                $3->type=TYPE_REAL;
                                if(DEBUG) printf("$3->type: %d\n",$3->type);
                                int i;
                                for(i = table->count - entry_count; i < table->count; i++) {
                                  table->table[i].type = TYPE_REAL;
                                }
                                entry_count = 0;
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
                                $1->type = $$->type;
                                // $1->type = 0; // So without this line of code
                                // // my program will seg fault on OS X, but not
                                // // Windows or Linux. It produces the correct
                                // // output without this being set correctly.
                                // // I have a feeling the output might not be fully
                                // // correct for later expressions, as this relies
                                // // on the parser to recognize real and integer
                                // // values for later instructions.
                                $$->address = 0;
                                $$->size = $1->size;
                                insert(*$$);
                                entry_count++;
                                if(DEBUG) printf("$$->type: %d\n", $$->type);
                              }
        |varref SEMICOLON {
          $$->name = $1->name;
          $$->kind = $1->kind;
          $1->type = $$->type;
          // $1->type = 0; // Debug
          $$->address = 0;
          $$->size = $1->size;
          insert(*$$);
          entry_count++;
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
            |controlstmt programbody { // If/else statements
              $$ = malloc(sizeof(struct statement));
              $$->if_stmt = $1;
              $$->link = $2;
              $$->isCond = 1;
              if(DEBUG) printf("Got to controlstmt programbody\n");
            }
            |controlstmt {
              $$ = malloc(sizeof(struct statement));
              $$->if_stmt = $1;
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->if_stmt = $$->if_stmt;
              temp->link = NULL;
              $$->link = temp;
              $$->isCond = 1;
              if($1->isIfElse == 1) {
                $$->isIfElse = 1;
              }
              if(DEBUG) printf("Got to controlstmt\n");
            }
            |outputstmt programbody {
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              $$->link = $2;
              if(DEBUG) printf("Got to outputstmt programbody\n");
            }
            |outputstmt {
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->exp = $$->exp;
              temp->link = NULL;
              $$->link = temp;
              if(DEBUG) printf("Got to outputstmt\n");
            }
            |inputstmt programbody {
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              $$->link = $2;
              if(DEBUG) printf("Got to inputstmt programbody\n");
            }
            |inputstmt {
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->exp = $$->exp;
              temp->link = NULL;
              $$->link = temp;
              if(DEBUG) printf("Got to inputstmt\n");
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
        // $$->type = $1->type;
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
      // $$->type = $1->type;
    }

exp: exp ADD term {// Code to parse expressions
                    if(DEBUG) printf("Got to add\n");
                    $$ = malloc(sizeof(struct ast_expression));
                    $$->kind = KIND_OP;
                    $$->operator = OP_ADD;
                    $$->l_operand = $1;
                    $$->r_operand = $3;
                  }
    |exp MINUS term {
                      if(DEBUG) printf("Got to subtract\n");
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
            // $$->type = $1->type;
          }
          ;

term: term MULT factor {
                          if(DEBUG) printf("Got to mult\n");
                          $$ = malloc(sizeof(struct ast_expression));
                          $$->kind = KIND_OP;
                          $$->operator = OP_MUL;
                          $$->l_operand = $1;
                          $$->r_operand = $3;
                       }
     |term DIV factor {
                          if(DEBUG) printf("Got to div\n");
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
                // $$->type = $1->type;
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
              // $$->type = $1->type;
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
                            if(DEBUG) printf("Got to parenthesized expression\n");
                            // $2 = malloc(sizeof(struct ast_expression));
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

// controlstmt: RWIF bexp SEMICOLON RWEND RWIF SEMICOLON // condstmt is a conditional, may need to have this read in programbody after the first semicolon?
//             |RWELSE SEMICOLON RWEND RWIF SEMICOLON
//             ;

outputstmt: RWPRINT printlist {
              if(DEBUG) printf("Got to outputstmt: RWPRINT printlist\n");
              // printf("printlist = %s\n", $2); // Doesn't work with carriage returns
              $$ = malloc(sizeof(struct ast_expression));
              $$->operator = OP_PRINT;
              $$->r_operand = $2;
            //   // printf("%d\n",strlen($$->charString));
            //   char buf[1024];
            //   struct ast_expression *tempExp = $$->r_operand;
            //   while(tempExp != NULL) {
            //     // Parse the entire charstring
            //     // printf("GOT HERE\n");
            //     strncat(buf, tempExp->charString, sizeof(tempExp->charString));
            //     // printf("%s\n",buf);
            //     tempExp = tempExp->r_operand;
            //   }
            // if(DEBUG) printf("buf = %s\n", buf);
            //   $$->charString = strdup(buf);
            // if(DEBUG) printf("Got out of loop\n");
            };

printlist: CHARSTRING printlist {
            if(DEBUG) printf("Got to CHARSTRING printlist\n");
            if(DEBUG) printf("CHARSTRING: %s\n", $1);
            $$ = malloc(sizeof(struct ast_expression));
            $$->r_operand = malloc(sizeof(struct ast_expression));
            $$->charString = $1;
            $$->r_operand = $2;
          }
          |CHARSTRING COMMA printlist {
            if(DEBUG) printf("Got to CHARSTRING COMMA printlist\n");
            if(DEBUG) printf("CHARSTRING: %s\n", $1);
            $$ = malloc(sizeof(struct ast_expression));
            $$->r_operand = malloc(sizeof(struct ast_expression));
            // Temporarily getting rid of this comma to see if it makes printing easier
            // char *temp = ",";
            // strncat($1, temp, 1); // Append a comma to this string.
            $$->charString = $1;
            $$->r_operand = $3;
          }
          |CARRETURN printlist {
            $$ = malloc(sizeof(struct ast_expression));
            $$->r_operand = malloc(sizeof(struct ast_expression));
            $$->charString = $1;
            $$->r_operand = $2;
            if(DEBUG) printf("Got to CARRETURN printlist\n");
          }
          |CARRETURN COMMA printlist {
            $$ = malloc(sizeof(struct ast_expression));
            $$->r_operand = malloc(sizeof(struct ast_expression));
            $$->charString = "!,";
            $$->r_operand = $3;
            if(DEBUG) printf("Got to CARRETURN COMMA printlist\n");
          }
          |bexp COMMA printlist {
            printf("Got to bexp COMMA printlist\n");
            $$ = malloc(sizeof(struct ast_expression));
            // $$->r_operand = $3;
            // What if I set $$->l_operand to $1? Then I could just parse that part for bexp's...
            // $$ = $1;
            // $$->charString = $3->charString;
            $$->l_operand = malloc(sizeof(struct ast_expression));
            $$->r_operand = malloc(sizeof(struct ast_expression));
            $$->l_operand = $1;
            $$->r_operand = $3;
          }
          |bexp SEMICOLON{
            $$ = malloc(sizeof(struct ast_expression));
            printf("Got to bexp SEMICOLON\n");
            // $$->charString = NULL;
            // Added for debug...
            $$->l_operand = malloc(sizeof(struct ast_expression));
            $$->l_operand = $1;

            // $$->r_operand = malloc(sizeof(struct ast_expression));
            // $$->r_operand = $1;
            // Variable references are stored on the l_operand side of an exp
            // There's a segfault under this line... probably because the address isn't being assigned if a literal is passed in?
            // $$->address = $$->l_operand->target->address;
            // $$->target = &table->table[isPresent($1->l_operand->target->name)];
            // printf("$1->l_operand->target->name: %s\n", $1->l_operand->target->name);
            // $$->r_operand->charString = NULL;
            // $$->charString = "Found_a_vari";
          }
          |SEMICOLON {
            $$ = NULL;
            // $$->r_operand = NULL;
          }
          ;

inputstmt: RWREAD varref SEMICOLON {
              if(DEBUG) printf("Got to RWREAD varref SEMICOLON\n");
              $$ = malloc(sizeof(struct ast_expression));
              $$->address = table->table[isPresent($2->name)].address;
              $$->operator = OP_READ;
           }
           ;

// Kind of surprised that this rule did not produce any shift/reduce conflicts.
// Would not be a bad plan to keep an eye on it and watch for conflicts.
controlstmt: RWIF bexp SEMICOLON programbody RWEND RWIF SEMICOLON {
             printf("Got to controlstmt RWIF bexp SEMICOLON programbody RWEND RWIF SEMICOLON\n");
             $$ = malloc(sizeof(struct ast_if_stmt));
             printf("Got here\n");
             $$->conditional_stmt = $2;
             $$->body = $4;
             $$->isIfElse = 0;
             printf("Got to end of if statement\n");
           }
           |RWIF bexp SEMICOLON programbody RWELSE SEMICOLON programbody RWEND RWIF SEMICOLON {
             printf("Got to controlstmt RWIF bexp SEMICOLON programbody RWELSE SEMICOLON programbody RWEND RWIF SEMICOLON\n");
             $$ = malloc(sizeof(struct ast_if_stmt));
             $$->conditional_stmt = $2;
             $$->body = $4;
             $$->if_link = malloc(sizeof(struct ast_if_stmt));
             $$->if_link->body = $7; // Dunno if this will work.
             $$->isIfElse = 1;
             printf("Got to end of if else statement\n");
           }
           ;
endmainstmt: RWEND RWMAIN SEMICOLON { printf("Got to endmainstmt\n");};

%%
int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
