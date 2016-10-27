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

extern struct statement *tree;

#endif /** ast_h */
