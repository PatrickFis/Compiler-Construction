#ifndef ast_h
#define ast_h

#define KIND_INT 0
#define KIND_REAL 1
#define KIND_OP 2

#define TYPE_VAR 3 // This is for parsing exp = varname

#define OP_ASGN 16 // Changed this to 16 instead of 0 because of segmentation faults...
#define OP_ADD 1
#define OP_SUB 2
#define OP_MUL 3
#define OP_DIV 4
#define OP_UMIN 5
#define OP_LSTHN 6
#define OP_LSTHNEQL 7
#define OP_GRTHN 8
#define OP_GRTHNEQL 9
#define OP_EQUAL 10
#define OP_NEQUAL 11
#define OP_AND 12
#define OP_OR 13
#define OP_NOT 14
#define OP_PRINT 15
#define OP_READ 17

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
    char type;
    char *charString;
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
void checkInstructions(int iBefore, int iAfter);
extern struct statement *list;
extern struct statement *head; // Pointer to head of the list

#endif /** ast_h */
