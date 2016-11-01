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
  if(exp->kind == KIND_INT) { // If expression involves integers
    printf("LLI %d\n", exp->value);
  }
  else if(exp->kind == KIND_REAL) { // If expression involves reals
    printf("LLF %f\n", exp->rvalue);
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

    case OP_ADD:
      // printf("GOT TO OP_ADD PORTION!!!\n");
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      printf("ADI\n");
      // printf("exp->l_operand->value: %d\n", exp->l_operand->value);
      // exprgen(exp->l_operand);
      // exprgen(exp->r_operand);
      break;

    case OP_SUB:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      printf("SBI\n");
      break;

    case OP_MUL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      printf("MLI\n");
      break;

    case OP_DIV:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      printf("DVI\n");
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
