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
#include "bisonHelper.h"
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
   struct ast_while_stmt *while_loop;
   struct ast_counting_stmt *count_loop;
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
%type <while_loop> whilestmt
%type <count_loop> countingstmt
%type <expr> exitstmt
%%

program : headingstmt datasection algsection;

headingstmt: RWMAIN SEMICOLON;

datasection: RWDATA COLON
            |RWDATA COLON decstmtlist
            ; // decstmtlist is a listing of the variables used by this Slic program.

decstmtlist: decstmtlist decstmt
            |decstmt
            ; // decstmt is just a variable declaration.

decstmt: RWINT COLON varlist {
                                  $3 = malloc(sizeof(struct symbol_table_entry));
                                  $3->type = TYPE_INT;
                                  if(DEBUG) printf("$3->type: %d\n",$3->type);
                                  int i;
                                  for(i = table->count - entry_count; i < table->count; i++) {
                                    table->table[i].type = TYPE_INT;
                                  }
                                  entry_count = 0;
                                  if(DEBUG) printf("Finished integer varlist\n");
                                }
        |RWREAL COLON varlist { $3 = malloc(sizeof(struct symbol_table_entry));
                                $3->type=TYPE_REAL;
                                if(DEBUG) printf("$3->type: %d\n",$3->type);
                                int i;
                                for(i = table->count - entry_count; i < table->count; i++) {
                                  table->table[i].type = TYPE_REAL;
                                }
                                entry_count = 0;
                                if(DEBUG) printf("Finished real varlist\n");
                                }
        ; // The reserved word is followed by a list because any number of variables can be declared on a single line.

varlist: varref COMMA varlist {
                                $$->name = $1->name;
                                $$->kind = $1->kind;
                                $1->type = $$->type;
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
                                 if(DEBUG) printf("Got to array declaration\n");
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
             }
            |assignstmt { // Only one assignment
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->exp = $$->exp;
              temp->link = NULL;
              $$->link = temp;
            }
            |controlstmt programbody { // If/else statements
              // Note that if else statements will be parsed using this rule...
              $$ = malloc(sizeof(struct statement));
              $$->if_stmt = $1;
              $$->link = $2;
              $$->isCond = 1;
              if($1->isIfElse == 1) {
                $$->isIfElse = 1;
              }
              if(DEBUG) printf("Got to controlstmt programbody\n");
            }
            |controlstmt {
              $$ = malloc(sizeof(struct statement));
              $$->if_stmt = $1;
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->if_stmt = $$->if_stmt;
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
            |whilestmt programbody {
              if(DEBUG) printf("Got to whilestmt programbody\n");
              $$ = malloc(sizeof(struct statement));
              $$->while_stmt = $1;
              $$->link = $2;
              $$->isWhile = 1;
              if(DEBUG) printf("Finished whilestmt programbody\n");
            }
            |whilestmt {
              if(DEBUG) printf("Got to whilestmt\n");
              $$ = malloc(sizeof(struct statement));
              $$->while_stmt = $1;
              struct statement *temp = malloc(sizeof(struct statement));
              temp->while_stmt = $$->while_stmt;
              $$->link = temp;
              $$->isWhile = 1;

              if(DEBUG) printf("Finished whilestmt\n");
            }
            |countingstmt programbody {
              if(DEBUG) printf("Got to countingstmt programbody\n");
              $$ = malloc(sizeof(struct statement));
              $$->count_stmt = $1;
              $$->link = $2;
              $$->isCount = 1;
            }
            |countingstmt {
              if(DEBUG) printf("Got to countingstmt\n");
              $$ = malloc(sizeof(struct statement));
              $$->count_stmt = $1;
              struct statement *temp = malloc(sizeof(struct statement));
              temp->count_stmt = $$->count_stmt;
              $$->link = temp;
              $$->isCount = 1;
            }
            |exitstmt programbody {
              if(DEBUG) printf("Got to exitstmt programbody\n");
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              $$->link = $2;
            }
            |exitstmt {
              if(DEBUG) printf("Got to exitstmt\n");
              $$ = malloc(sizeof(struct statement));
              $$->exp = $1;
              struct statement *temp = malloc(sizeof(struct statement));
              temp->exp = $1;
              temp->link = NULL;
              $$->link = temp;
              if(DEBUG) printf("Finished exit...\n");
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
              assignStmtTargets($$, $$->target);
              if(DEBUG) printf("Address %d\n", $$->address);
              $$->arrayOffset = 0;
            }
            |VAR LBRACK bexp RBRACK assign bexp SEMICOLON {
              if(DEBUG) printf("Got to array assignment\n");
              $$ = malloc(sizeof(struct ast_expression));
              $$->kind = KIND_OP;
              $$->operator = OP_ASGN;
              $$->target = &table->table[isPresent($1)];
              $$->address = $$->target->address; // Get address of array, the offset will be calculated from l_operand
              $$->l_operand = $3; // Store array location in l_operand
              $$->r_operand = $6; // Store assignment value in r_operand
              $$->arrayOffset = $$->address; // Will be incremented later
              assignStmtTargets($$, $$->target);
            }
            ;

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
      $$ = $1;
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
            $$ = $1;
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
                            if(DEBUG) printf("Got to parenthesized expression\n");
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
           $$->l_operand->type = TYPE_VAR;
           $$->l_operand->target = &table->table[tableLoc];
         }
         if(varType == TYPE_REAL) {
           $$->l_operand = malloc(sizeof(struct ast_expression)); // Trying to hack together a solution with this
           $$->l_operand->type = TYPE_VAR;
           $$->l_operand->target = &table->table[tableLoc];
         }
       }
       |VAR LBRACK bexp RBRACK {
         // This is a rewritten portion of the above code that deals with
         // array references involving expressions
         if(DEBUG) printf("Got to VAR LBRACK bexp RBRACK\n");
         $$ = malloc(sizeof(struct ast_expression));
         $$->type = TYPE_ARRAY;
         int tableLoc = isPresent($1);
         int varType = table->table[tableLoc].type;
         if(varType == TYPE_INT) {
           $$->l_operand = malloc(sizeof(struct ast_expression));
           $$->l_operand->type = TYPE_VAR;
           $$->l_operand->target = &table->table[tableLoc]; // Get initial location of array
           $$->r_operand = $3;
         }
         if(varType == TYPE_REAL) {
           $$->l_operand = malloc(sizeof(struct ast_expression));
           $$->l_operand->type = TYPE_VAR;
           $$->l_operand->target = &table->table[tableLoc]; // Get initial location of array
           $$->r_operand = $3;
         }
         assignStmtTargets($$, &table->table[tableLoc]);
       }
       ;

outputstmt: RWPRINT printlist {
              if(DEBUG) printf("Got to outputstmt: RWPRINT printlist\n");
              $$ = malloc(sizeof(struct ast_expression));
              $$->operator = OP_PRINT;
              $$->r_operand = $2;
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
            if(DEBUG) printf("Got to bexp COMMA printlist\n");
            $$ = malloc(sizeof(struct ast_expression));
            $$->l_operand = malloc(sizeof(struct ast_expression));
            $$->r_operand = malloc(sizeof(struct ast_expression));
            $$->l_operand = $1;
            $$->r_operand = $3;
          }
          |bexp SEMICOLON{
            $$ = malloc(sizeof(struct ast_expression));
            if(DEBUG) printf("Got to bexp SEMICOLON\n");
            // Added for debug...
            $$->l_operand = malloc(sizeof(struct ast_expression));
            $$->l_operand = $1;
          }
          |SEMICOLON {
            $$ = NULL;
          }
          ;

inputstmt: RWREAD varref SEMICOLON {
              if(DEBUG) printf("Got to RWREAD varref SEMICOLON\n");
              $$ = malloc(sizeof(struct ast_expression));
              $$->address = table->table[isPresent($2->name)].address;
              $$->operator = OP_READ;
           }
           ;

controlstmt: RWIF bexp SEMICOLON programbody RWEND RWIF SEMICOLON {
              if(DEBUG) printf("Found an if\n");
              $$ = malloc(sizeof(struct ast_if_stmt));
              $$->conditional_stmt = $2;
              $$->body = $4;
              $$->isIfElse = 0;
             }
             |RWIF bexp SEMICOLON programbody RWELSE SEMICOLON programbody RWEND RWIF SEMICOLON {
               if(DEBUG) printf("Found an if else\n");
               $$ = malloc(sizeof(struct ast_if_stmt));
               $$->conditional_stmt = $2;
               $$->body = $4;
               // Added the tempLink structure to actually get nested if statements to work when else statements are present.
               $$->tempLink = malloc(sizeof(struct statement));
               $$->tempLink = $7;
              $$->isIfElse = 1;
             }
             ;
whilestmt: RWWHILE bexp SEMICOLON programbody RWEND RWWHILE SEMICOLON {
              if(DEBUG) printf("Got to whilestmt: RWWHILE bexp SEMICOLON programbody RWEND RWWHILE SEMICOLON\n");
              $$ = malloc(sizeof(struct ast_while_stmt));
              $$->conditional_stmt = $2;
              $$->body = $4;
           }
           ;

countingstmt: RWCOUNTING VAR RWUPWARD bexp RWTO bexp SEMICOLON programbody RWEND RWCOUNTING SEMICOLON {
                if(DEBUG) printf("Got to counting upward statement\n");
                $$ = malloc(sizeof(struct ast_counting_stmt));
                // Change the target into an assignment
                $$->target_assignment = malloc(sizeof(struct ast_expression));
                $$->target_assignment->kind = KIND_OP;
                $$->target_assignment->operator = OP_ASGN;
                $$->target_assignment->r_operand = $4;
                $$->target_assignment->target = &table->table[isPresent($2)]; // Get target from symbol table
                $$->target_assignment->address = $$->target_assignment->target->address;
                assignStmtTargets($$->target_assignment, $$->target_assignment->target);
                $$->target_assignment->arrayOffset = 0;

                $$->startexpr = $4;
                $$->endexpr = $6;
                $$->body = $8;
                $$->direction = 1;
            }
            |RWCOUNTING VAR RWDOWNWARD bexp RWTO bexp SEMICOLON programbody RWEND RWCOUNTING SEMICOLON {
              if(DEBUG) printf("Got to counting downward statement\n");
              $$ = malloc(sizeof(struct ast_counting_stmt));
              // Change the target into an assignment
              $$->target_assignment = malloc(sizeof(struct ast_expression));
              $$->target_assignment->kind = KIND_OP;
              $$->target_assignment->operator = OP_ASGN;
              $$->target_assignment->r_operand = $4;
              $$->target_assignment->target = &table->table[isPresent($2)]; // Get target from symbol table
              $$->target_assignment->address = $$->target_assignment->target->address;
              assignStmtTargets($$->target_assignment, $$->target_assignment->target);
              $$->target_assignment->arrayOffset = 0;

              $$->startexpr = $4;
              $$->endexpr = $6;
              $$->body = $8;
              $$->direction = 0;
            }
            |RWCOUNTING VAR LBRACK bexp RBRACK RWUPWARD bexp RWTO bexp SEMICOLON programbody RWEND RWCOUNTING SEMICOLON {
              if(DEBUG) printf("Got to counting upward statement\n");
              if(DEBUG) printf("Got to counting loop with an array\n");
              $$ = malloc(sizeof(struct ast_counting_stmt));
              // Change the target into an assignment
              $$->target_assignment = malloc(sizeof(struct ast_expression));
              $$->target_assignment->kind = KIND_OP;
              $$->target_assignment->operator = OP_ASGN;
              $$->target_assignment->l_operand = $4; // Array index is stored in l_operand
              $$->target_assignment->r_operand = $7;
              $$->target_assignment->target = &table->table[isPresent($2)]; // Get target from symbol table
              $$->target_assignment->address = $$->target_assignment->target->address;
              assignStmtTargets($$->target_assignment, $$->target_assignment->target);
              $$->target_assignment->arrayOffset = 0;

              $$->startexpr = $7;
              $$->endexpr = $9;
              $$->body = $11;
              $$->direction = 1;
            }
            |RWCOUNTING VAR LBRACK bexp RBRACK RWDOWNWARD bexp RWTO bexp SEMICOLON programbody RWEND RWCOUNTING SEMICOLON {
              if(DEBUG) printf("Got to counting upward statement\n");
              if(DEBUG) printf("Got to counting loop with an array\n");
              $$ = malloc(sizeof(struct ast_counting_stmt));
              // Change the target into an assignment
              $$->target_assignment = malloc(sizeof(struct ast_expression));
              $$->target_assignment->kind = KIND_OP;
              $$->target_assignment->operator = OP_ASGN;
              $$->target_assignment->l_operand = $4; // Array index is stored in l_operand
              $$->target_assignment->r_operand = $7;
              $$->target_assignment->target = &table->table[isPresent($2)]; // Get target from symbol table
              $$->target_assignment->address = $$->target_assignment->target->address;
              assignStmtTargets($$->target_assignment, $$->target_assignment->target);
              $$->target_assignment->arrayOffset = 0;

              $$->startexpr = $7;
              $$->endexpr = $9;
              $$->body = $11;
              $$->direction = 0;
            }
            ;

exitstmt: RWEXIT SEMICOLON {
          if(DEBUG) printf("Found an exit\n");
          $$ = malloc(sizeof(struct ast_expression));
          $$->operator = OP_EXIT;
          $$->l_operand = NULL;
          $$->r_operand = NULL;
        }
        ;

endmainstmt: RWEND RWMAIN SEMICOLON { if(DEBUG) printf("Got to endmainstmt\n");};

%%
int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}
