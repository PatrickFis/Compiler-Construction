#include "bisonHelper.h"
#include "ast.h"
#include "stable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void assignStmtTargets(struct ast_expression *root, struct symbol_table_entry *target) {
  if(root->l_operand != NULL) {
    assignStmtTargets(root->l_operand, target);
    root->target = target;
  }
  if(root->r_operand != NULL) {
    assignStmtTargets(root->r_operand, target);
    root->target = target;
  }
  if(root->type == TYPE_VAR) {
    return;
  }
  root->target = target;
}
