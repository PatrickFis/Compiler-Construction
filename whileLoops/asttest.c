#include "ast.h"
#include <stdio.h>
// Compile with gcc asttest.c ast.c
//struct statement *list;
//struct statement *head;
int main() {
    struct statement *ast_test = malloc(sizeof(struct statement));
    struct ast_expression ast_exptest;
    // struct ast_expression *l_op;
    // struct ast_expression *r_op;
    ast_exptest = createExp('d', '-', 100);
    ast_test->exp = &ast_exptest;
    insertStmt(ast_test);
    printf("kind = %c, operator = %c, value = %d\n", head->exp->kind, head->exp->operator, head->exp->value); // Insert is correctly modifying the head statement
    ast_exptest = createExp('c', '+', 200);
    insertStmt(ast_test);
    struct ast_expression x = *head->link->exp; // Looks like this is working. Can correctly traverse the linked list.
    printf("kind = %c, operator = %c, value = %d\n", x.kind, x.operator, x.value);

//    ast_exptest.kind = 'c';
//    ast_exptest.l_operand = NULL;
//    ast_exptest.operator = '+';
//    ast_exptest.r_operand = NULL;
//    ast_exptest.value = 100;
    printf("kind = %c, operator = %c, value = %d\n", ast_exptest.kind, ast_exptest.operator, ast_exptest.value);
    return 0;
}
