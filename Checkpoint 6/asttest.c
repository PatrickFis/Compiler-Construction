#include "ast.h"
#include <stdio.h>
// Compile with gcc asttest.c ast.c
//struct statement *list;
//struct statement *head;
int main() {
    struct statement *ast_test = malloc(sizeof(struct statement));
    struct ast_expression ast_exptest;
    struct ast_expression *l_op;
    struct ast_expression *r_op;
    ast_exptest = createExp('d', '-', 100);
//    ast_exptest.kind = 'c';
//    ast_exptest.l_operand = NULL;
//    ast_exptest.operator = '+';
//    ast_exptest.r_operand = NULL;
//    ast_exptest.value = 100;
    printf("kind = %c, operator = %c, value = %d", ast_exptest.kind, ast_exptest.operator, ast_exptest.value);
    return 0;
}
