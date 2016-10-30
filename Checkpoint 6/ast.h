#ifndef ast_h
#define ast_h

struct statement {
    struct statement *link;
    struct ast_expression *exp;
};

struct ast_node { // This portion will probably be used for control structures, etc
    struct ast_node *left;
    struct ast_node *right;
};

struct ast_expression { // This portion will just be used for expressions(possibly just integer expressions)
    char kind;
    char operator;
    struct ast_expression *l_operand;
    struct ast_expression *r_operand;
    int value;
};

void insertStmt(struct statement *stmt);
//struct ast_expression createExp(char kind, char operator, struct ast_expression l_operand, struct ast_expression r_operand, int value);
struct ast_expression createExp(char kind, char operator, int value);
extern struct statement *list;
extern struct statement *head; // Pointer to head of the list

#endif /** ast_h */
