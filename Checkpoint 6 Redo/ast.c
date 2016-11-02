#include "ast.h"
#include "stable.h"
#include <stdio.h>
#include <stdlib.h>

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

//struct ast_expression createExp(char kind, char operator, struct ast_expression l_operand,
//                                struct ast_expression r_operand, int value) {
//    struct ast_expression expr;
//    expr.kind = kind;
//    expr.operator = operator;
////    expr.l_operand = malloc(sizeof(struct ast_expression));
////    expr.l_operand = &l_operand;
////    expr.r_operand = malloc(sizeof(struct ast_expression));
////    expr.r_operand = &r_operand;
//    expr.value = value;
//    return expr;
//}

void printList() {
  struct statement *next;
  next = list;
  while(next->link != NULL) {
    // printf("kind: %d, operator: %d, value: %d\n", next->exp->kind, next->exp->operator, next->exp->value);
    exprgen(next->exp);
    next = next->link;
  }
}

// Code generation for expressions
void exprgen(struct ast_expression *exp) {
  // printf("exp->value = %d\n", exp->value);
  if(exp->kind == TYPE_INT) { // If expression involves integers
    printf("LLI %d\n", exp->value);
  }
  else if(exp->kind == KIND_REAL) { // If expression involves reals
    printf("LLF %f\n", exp->rvalue);
  }
  if(exp->type == TYPE_VAR) { // Last ditch effort...
    printf("LAA %d\n",exp->l_operand->target->address);
    printf("LOD\n");
    // printf("exp->l_operand->target->address: %d\n",exp->l_operand->target->address);
    return; // Just stop the recursion when you reach a variable reference
  }
  switch(exp->operator) {
    case OP_ASGN:
      // Load values
      // printf("OP_ASGN FOUND\n");
      if(exp->r_operand != NULL) // Load the address used for assignment
        printf("LAA %d\n", exp->address);
      if(exp->l_operand != NULL) // This check is probably unnecessary
        exprgen(exp->l_operand);
      if(exp->r_operand != NULL)
        exprgen(exp->r_operand);
      if(exp->l_operand == NULL && exp->r_operand != NULL) {
        // Surprisingly, checking if r_operand is not NULL seems to have fixed
        // an issue where the STO instruction was being printed more than
        // once.
        printf("STO\n");
      }
      break;
    case OP_UMIN:
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("NGI\n");
      else if(exp->type == KIND_REAL) {
        printf("NGF");
      }
      break;

    case OP_ADD:
      // printf("GOT TO OP_ADD PORTION!!!\n");
      // printf("exp->type: %d\n", exp->type);
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("ADI\n");
      if(exp->type == KIND_REAL) {
        printf("ADF\n");
      }
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
      if(exp->type == TYPE_INT) printf("SBI\n");
      else if(exp->type == KIND_REAL) {
        printf("SBF\n");
      }
      break;

    case OP_MUL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("MLI\n");
      else if(exp->type == KIND_REAL) {
        printf("MLF\n");
      }
      break;

    case OP_DIV:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("DVI\n");
      else if(exp->type == KIND_REAL) {
        printf("DVF\n");
      }
      break;

    case OP_LSTHN:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("LTI\n");
      else if(exp->type == KIND_REAL) {
        printf("LTF\n");
      }
      break;

    case OP_LSTHNEQL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("LEI\n");
      else if(exp->type == KIND_REAL) {
        printf("LEF\n");
      }
      break;

    case OP_GRTHN:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("GTI\n");
      else if(exp->type == KIND_REAL) {
        printf("GTF\n");
      }
      break;

    case OP_GRTHNEQL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("GEI\n");
      else if(exp->type == KIND_REAL) {
        printf("GEF\n");
      }
      break;

    case OP_EQUAL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("EQI\n");
      else if(exp->type == KIND_REAL) {
        printf("EQF\n");
      }
      break;

    case OP_NEQUAL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) printf("NEI\n");
      else if(exp->type == KIND_REAL) {
        printf("NEF\n");
      }
      break;

    case OP_AND: // These Boolean instructions require a bit of clever faking
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) {
        // Boolean and = multiplication
        printf("MLI\n");
        printf("LLI 1\n");
        printf("EQI\n"); // Check if l_op * r_op = 1
      }
      else if(exp->type == KIND_REAL) {
        printf("MLF\n");
        printf("LLF 1.0\n");
        printf("EQF\n");
      }
      break;

    case OP_OR:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->type == TYPE_INT) {
        // Boolean or = addition
        printf("ADI\n");
        printf("LLI 1\n");
        printf("EQI\n");
      }
      else if(exp->type == KIND_REAL) {
        printf("ADF\n");
        printf("LLF 1.0\n");
        printf("EQF\n");
      }
      break;

    case OP_NOT:
      // if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      // printf("exp->type %d\n", exp->type);
      if(exp->type == TYPE_INT) {
        // Boolean not = complement
        printf("LLI 0\n");
        printf("NEI\n"); // If exp != 0, then exp is true
      }
      else if(exp->type == KIND_REAL) {
        // printf("exp->type: %d\n", exp->value);
        printf("LLF 0.0\n");
        printf("NEF\n");
      }
      break;


  }
}

struct ast_expression createExp(char kind, char operator, int value) {
  struct ast_expression expr;
  expr.kind = kind;
  expr.operator = operator;
  expr.value = value;
  return expr;
}
