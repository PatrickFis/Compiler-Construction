#include "ast.h"
#include "stable.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0
struct statement *list; // Extern struct declared in ast.h. Used as a linked list.
struct statement *head;
int count = 0; // Keep track of how many statements we have

void insertStmt(struct statement *stmt) {
  count++;
  // printf("stmt->exp->kind: %d\n", stmt->exp->operator);
  if(head == NULL) {
    // Initalize head here if it has not already been done.
    head = malloc(sizeof(struct statement));
    head->link = NULL;
    head->exp = stmt->exp;
    // printf("head kind: %d, operator: %d, value: %d\n", head->exp->kind, head->exp->operator, head->exp->value);
    return;
  }
  // printf("head kind: %d, operator: %d, value: %d\n", head->exp->kind, head->exp->operator, head->exp->value);
  if(head->link == NULL) { // Added this because the head node was being overwritten
    struct statement *next = malloc(sizeof(struct statement));
    next->link = stmt;
    next->exp = stmt->exp;
    head->link = next;
  }
  else {
    struct statement *next;
    next = head;
    while(next->link != NULL) {
      next = next->link;
    }
    next->link = stmt;
    next->exp = stmt->exp;
  }
}

/*
 *  This function goes through the abstract syntax tree and calls the exprgen
 *  function on each statement in a linked list. Will probably change these
 *  functions when implementing control structures to utilize arrays for
 *  easy jump locations.
 */
void printList() {
  struct statement *next;
  next = list;
  printf("ISP %d\n", table->memorySize);
  while(next->link != NULL) {
    // printf("Calling exprgen\n"); // Debug
    exprgen(next->exp);
    // printf("exprgen finished\n"); // Debug
    next = next->link;
  }
}

/*
 *  This function recursively reassigns types. It is just being kept in case
 *  I need it later.
 */
void recurseAssign(struct ast_expression *exp, int type) {
  // printf("Got here\n");
  exp->type = type;
  if(exp->l_operand != NULL) {
    recurseAssign(exp->l_operand, type);
    exp->l_operand->type = type;
  }
  if(exp->r_operand != NULL) {
    recurseAssign(exp->r_operand, type);
    exp->r_operand->type = type;
  }
}

/*
 *  This function recursively assigns targets to expressions, which allows the
 *  code generator to know what kind of instructions to print.
 */
void assignTarget(struct ast_expression *exp, struct symbol_table_entry target) {
  if(exp->type == TYPE_VAR)
    return;
  if(exp->l_operand != NULL) {
    assignTarget(exp->l_operand, target);
    exp->l_operand->target = &target;
    // exp->l_operand->type = target.type;
  }
  if(exp->r_operand != NULL) {
    assignTarget(exp->r_operand, target);
    exp->r_operand->target = &target;
    // exp->r_operand->type = target.type;
  }
  if(exp->l_operand == NULL && exp->type != TYPE_VAR) {
    // printf("exp->type = %d\n", exp->type);
    exp->type = target.type;
    // printf("exp->type = %d\n", exp->type);
  }
  if(exp->r_operand == NULL && exp->type != TYPE_VAR) {
    exp->type = target.type;
  }
}

/*
 *  This function generates gstal code for a given expression. It goes through
 *  the left and right operands of an expression recursively. In the case that
 *  a given expression contains another variable reference (e.g. n := x),
 *  recursion will halt after loading the variable.
 */

void exprgen(struct ast_expression *exp) {
  // printf("exp->value = %d\n", exp->value);
  if(DEBUG) printf("Got to exprgen\n");
  if(DEBUG) printf("exp->type: %d\n", exp->type);
  if(DEBUG) printf("exp->operator: %d\n", exp->operator);
  // if(exp->target != NULL) printf("%s\n", exp->target->name);
  if(exp->kind == KIND_INT && exp->type != TYPE_VAR) { // If expression involves integers
    if(DEBUG) printf("Got to load int\n");
    printf("LLI %d\n", exp->value);
  }
  else if(exp->kind == KIND_REAL && exp->type != TYPE_VAR) { // If expression involves reals
    if(DEBUG) printf("Got to load real\n");
    printf("LLF %f\n", exp->rvalue);
  }
  if(exp->type == TYPE_VAR) {
    if(DEBUG) printf("Got to variable type\n");
    printf("LAA %d\n",exp->l_operand->target->address);
    printf("LOD\n");
    if(DEBUG) printf("Finished variable type\n"); // Debug
    return; // Just stop the recursion when you reach a variable reference
  }
  if(DEBUG) printf("Got to switch statement\n");
  switch(exp->operator) {
    case OP_ASGN:
      if(DEBUG) printf("Got to OP_ASGN\n");
      assignTarget(exp, *exp->target);
      // Load values
      // printf("OP_ASGN FOUND\n");
      if(exp->r_operand != NULL) {// Load the address used for assignment
        if(DEBUG) printf("r_operand != NULL\n");
        printf("LAA %d\n", exp->address);
        exprgen(exp->r_operand);
      }
      if(exp->l_operand != NULL) {// This check is probably unnecessary
        if(DEBUG) printf("l_operand != NULL\n");
        exprgen(exp->l_operand);
      }
      // if(exp->r_operand != NULL) // Why is this check here?
      //   exprgen(exp->r_operand);
      if(exp->l_operand == NULL && exp->r_operand != NULL) {
        if(DEBUG) printf("Got to STO\n");
        // Surprisingly, checking if r_operand is not NULL seems to have fixed
        // an issue where the STO instruction was being printed more than
        // once.
        printf("STO\n");
      }
      break;

    case OP_UMIN:
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("NGI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("NGF");
      }
      break;

    case OP_ADD:
      // printf("GOT TO OP_ADD PORTION!!!\n");
      // printf("exp->type: %d\n", exp->type);
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        printf("ADI\n");
      }
      if(exp->target->type == TYPE_REAL) {
        printf("ADF\n");
      }
      // if(seenReal == 1) {
      //   printf("ADF\n");
      // }
      // else if(seenReal == 0) printf("ADI\n");
      // if(exp->type == TYPE_VAR) {
      //   printf("%d\n",exp->l_operand->target->address);
      //   // printf("%d\n",exp->target->);
      //   printf("LAA type var ");
      // }
      // printf("exp->l_operand->value: %d\n", exp->l_operand->value);
      // exprgen(exp->l_operand);
      // exprgen(exp->r_operand);
      break;

    case OP_SUB:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("SBI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("SBF\n");
      }
      break;

    case OP_MUL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("MLI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("MLF\n");
      }
      break;

    case OP_DIV:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("DVI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("DVF\n");
      }
      break;

    case OP_LSTHN:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("LTI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("LTF\n");
      }
      break;

    case OP_LSTHNEQL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("LEI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("LEF\n");
      }
      break;

    case OP_GRTHN:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("GTI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("GTF\n");
      }
      break;

    case OP_GRTHNEQL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("GEI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("GEF\n");
      }
      break;

    case OP_EQUAL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("EQI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("EQF\n");
      }
      break;

    case OP_NEQUAL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) printf("NEI\n");
      else if(exp->target->type == TYPE_REAL) {
        printf("NEF\n");
      }
      break;

    case OP_AND: // These Boolean instructions require a bit of clever faking
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // Boolean and = multiplication
        printf("MLI\n");
        printf("LLI 1\n");
        printf("EQI\n"); // Check if l_op * r_op = 1
      }
      else if(exp->target->type == TYPE_REAL) {
        printf("MLF\n");
        printf("LLF 1.0\n");
        printf("EQF\n");
      }
      break;

    case OP_OR:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // Boolean or = addition
        printf("ADI\n");
        printf("LLI 1\n");
        printf("EQI\n");
      }
      else if(exp->target->type == TYPE_REAL) {
        printf("ADF\n");
        printf("LLF 1.0\n");
        printf("EQF\n");
      }
      break;

    case OP_NOT:
      // if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      // printf("exp->type %d\n", exp->type);
      if(exp->target->type == TYPE_INT) {
        // Boolean not = complement
        printf("LLI 0\n");
        printf("NEI\n"); // If exp != 0, then exp is true
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("exp->type: %d\n", exp->value);
        printf("LLF 0.0\n");
        printf("NEF\n");
      }
      break;


  }
  if(DEBUG) printf("Finished exprgen\n");
}

struct ast_expression createExp(char kind, char operator, int value) {
  struct ast_expression expr;
  expr.kind = kind;
  expr.operator = operator;
  expr.value = value;
  return expr;
}
