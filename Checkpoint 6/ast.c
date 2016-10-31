#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

struct statement *list; // Extern struct declared in ast.h. Used as a linked list.
struct statement *head;

void insertStmt(struct statement *stmt) {
    struct statement test = *stmt;
    printf("stmt->exp->kind: %d\n", stmt->exp->operator);
    if(head == NULL) {
        // Initalize head here if it has not already been done.
        head = malloc(sizeof(struct statement));
        head->link = NULL;
        head->exp = stmt->exp;
        head->target = stmt->target;
        return;
    }
    struct statement *next;
    next = head;
    while(next->link != NULL) {
        next = next->link;
    }
    next->link = stmt;
    next->exp = stmt->exp;
    next->target = stmt->target;
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

struct ast_expression createExp(char kind, char operator, int value) {
    struct ast_expression expr;
    expr.kind = kind;
    expr.operator = operator;
    expr.value = value;
    return expr;
}
