#ifndef ast_h
#define ast_h

#define KIND_INT 0
#define KIND_REAL 1
#define KIND_OP 2

#define OP_ASGN 0
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_UMIN 5

struct statement {
    struct statement *link;
    struct ast_expression *exp;
    //struct symbol_table_entry *target;
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
    double rvalue;
    int address;
    struct symbol_table_entry *target;
};

void insertStmt(struct statement *stmt);
//struct ast_expression createExp(char kind, char operator, struct ast_expression l_operand, struct ast_expression r_operand, int value);
struct ast_expression createExp(char kind, char operator, int value);
void printList();
void exprgen(struct ast_expression *exp);
extern struct statement *list;
extern struct statement *head; // Pointer to head of the list

#endif /** ast_h */
