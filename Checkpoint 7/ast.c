#include "ast.h"
#include "stable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
struct statement *list; // Extern struct declared in ast.h. Used as a linked list.
struct statement *head;
int count = 0; // Keep track of how many statements we have

char instructionList[10000][100]; // Instructions will be stored in this array...
// Note that this means there can only be 10000 instructions.
// This will be incremented to determine where instructions need to go in the array
int instructionCounter = 0;

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

/*
 *  This function goes through the abstract syntax tree and calls the exprgen
 *  function on each statement in a linked list. Will probably change these
 *  functions when implementing control structures to utilize arrays for
 *  easy jump locations.
 */
void printList() {
  struct statement *next;
  next = list;
  sprintf(instructionList[instructionCounter++], "ISP %d", table->memorySize);
  // printf("ISP %d\n", table->memorySize);
  while(next->link != NULL) {
    // printf("Calling exprgen\n"); // Debug
    exprgen(next->exp);
    // printf("exprgen finished\n"); // Debug
    next = next->link;
  }

  for(int i = 0; i < instructionCounter; i++) {
    printf("%s\n", instructionList[i]);
  }
}

/*
 *  This function recursively reassigns types. It is just being kept in case
 *  I need it later.
 */
void recurseAssign(struct ast_expression *exp, int type) {
  // printf("Got here\n");
  exp->type = type;
  if(exp->l_operand != NULL) {
    recurseAssign(exp->l_operand, type);
    exp->l_operand->type = type;
  }
  if(exp->r_operand != NULL) {
    recurseAssign(exp->r_operand, type);
    exp->r_operand->type = type;
  }
}

/*
 *  This function recursively assigns targets to expressions, which allows the
 *  code generator to know what kind of instructions to print.
 */
void assignTarget(struct ast_expression *exp, struct symbol_table_entry target) {
  if(exp->type == TYPE_VAR)
    return;
  if(exp->l_operand != NULL) {
    assignTarget(exp->l_operand, target);
    exp->l_operand->target = &target;
    // exp->l_operand->type = target.type;
  }
  if(exp->r_operand != NULL) {
    assignTarget(exp->r_operand, target);
    exp->r_operand->target = &target;
    // exp->r_operand->type = target.type;
  }
  if(exp->l_operand == NULL && exp->type != TYPE_VAR) {
    // printf("exp->type = %d\n", exp->type);
    exp->type = target.type;
    // printf("exp->type = %d\n", exp->type);
  }
  if(exp->r_operand == NULL && exp->type != TYPE_VAR) {
    exp->type = target.type;
  }
}

/*
 *  This function will parse print statements and call exprgen when needed
 *  Maybe this should be rewritten to be like exprgen?
 */
void parsePrintStatement(struct ast_expression *exp) {
  printf("Got to parse print statement\n");
  if(exp->r_operand != NULL)
    parsePrintStatement(exp->r_operand);
  // if(strlen(exp->charString) > 2) {
    // exp->charString[0] = "";
    // exp->charString[strlen(exp->charString)-1] = "";
  // }
  // Goes through the string...
  if(exp->charString == NULL) {
    printf("FINALLY GOT A NULL\n");
    // exprgen(exp->r_operand);
    // exprgen(exp);
    // printf("exp->address: %d\n", exp->address);
  }
  if(exp->charString != NULL) {
    for(int i = 0; i < strlen(exp->charString); i++) {
      // Skip over quotes and commas
      // Will need to add a check for "" in a string, since quotes can be printed
      if(exp->charString[i] == '\"' || exp->charString[i] == ',') continue;
      printf("charString[i] = %c\n", exp->charString[i]);
    }
    printf("exp->charString: %s\n", exp->charString);
  }
}

/*
 *  This is for parsing print statements. Currently it will print instructions
 *  with the same type as the first entry in the symbol table. This may be
 *  fixable by storing our instructions in an array and doing some backtracing.
 *  Can possible do a check to see if the previous instruction was a LOD, and
 *  if it was go back to the instruction before the LOD and use it's target.
 *  I suppose this can keep track of if an integer type or real type has been
 *  seen and assign it to every x expression in case only one LOD is found.
 */
void parsePrintStatementv2(struct ast_expression *exp) {
  struct ast_expression *x = exp->r_operand;
  printf("iCounter = %d\n", instructionCounter);
  while(x != NULL) {
    if(x->charString == NULL) {
      x->target = &table->table[0];
      // This, and iCounterAfter, will be used to determine if the correct instructions were printed
      int iCounterBefore = instructionCounter;
      exprgen(x);
      int iCounterAfter = instructionCounter;
      printf("iB %d iA %d\n", iCounterBefore, iCounterAfter);
      // Get the first instruction inserted by the above call to exprgen and
      // find the address used by it. May need to change this to scan all
      // instructions generated between iCounterBefore and iCounterAfter.
      char *targetToken = strtok(instructionList[iCounterBefore], " ");
      targetToken = strtok(NULL, " ");
      int tar = atoi(targetToken);
      printf("tar = %d\n", tar);
      printf("tar type = %d\n", table->table[tar].type);
      printf("%s\n", targetToken);
      for(int i = iCounterBefore; i < iCounterAfter; i++) {
        printf("%s\n", instructionList[i]);
      }
      printf("\n\n\n\n\n");
      break;
    }
    printf("x->charString, %s\n", x->charString);
    int i;
    // Print a newline character
    if(strcmp(x->charString, "!,") == 0 || strcmp(x->charString, "!") == 0) {
      sprintf(instructionList[instructionCounter++], "PTL");
    }
    // Print characters enclosed by quotation marks. Will add a check for "" later.
    for(i = 1; i < strlen(x->charString)-1; i++) {
      // printf("%d\n", (int)x->charString[i]);
      sprintf(instructionList[instructionCounter++], "LLI %d", (int)x->charString[i]);
      sprintf(instructionList[instructionCounter++], "PTC");
    }
    x = x->r_operand;
  }
}
/*
 *  This function generates gstal code for a given expression. It goes through
 *  the left and right operands of an expression recursively. In the case that
 *  a given expression contains another variable reference (e.g. n := x),
 *  recursion will halt after loading the variable.
 */

void exprgen(struct ast_expression *exp) {
  // printf("exp->value = %d\n", exp->value);
  // printf("Got here");
  if(DEBUG) printf("Got to exprgen\n");
  if(DEBUG) printf("exp->type: %d\n", exp->type);
  if(DEBUG) printf("exp->operator: %d\n", exp->operator);
  // if(exp->target != NULL) printf("%s\n", exp->target->name);
  if(exp->operator == OP_PRINT) { // This occurs if an expression is a print statement
    // printf("Got to OP_PRINT\n");
    // if(exp->r_operand != NULL)exprgen(exp->r_operand);
    // struct ast_expression *x = exp->r_operand;
    // while(x != NULL) {
    //   printf("x->charString, %s\n", x->charString);
    //   x = x->r_operand;
    // }
    // printf("exp->charString, %s\n", exp->charString);
    parsePrintStatementv2(exp);
    return;
  }
  if(exp->kind == KIND_INT && exp->type != TYPE_VAR) { // If expression involves integers
    if(DEBUG) printf("Got to load int\n");
    // printf("LLI %d\n", exp->value);
    sprintf(instructionList[instructionCounter++], "LLI %d", exp->value);
  }
  else if(exp->kind == KIND_REAL && exp->type != TYPE_VAR) { // If expression involves reals
    if(DEBUG) printf("Got to load real\n");
    // printf("LLF %f\n", exp->rvalue);
    sprintf(instructionList[instructionCounter++], "LLF %f", exp->rvalue);
  }
  if(exp->type == TYPE_VAR) {
    if(DEBUG) printf("Got to variable type\n");
    // printf("LAA %d\n",exp->l_operand->target->address);
    // printf("LOD\n");
    sprintf(instructionList[instructionCounter++], "LAA %d", exp->l_operand->target->address);
    sprintf(instructionList[instructionCounter++], "LOD");
    if(DEBUG) printf("Finished variable type\n"); // Debug
    return; // Just stop the recursion when you reach a variable reference
  }
  if(DEBUG) printf("Got to switch statement\n");
  switch(exp->operator) {
    case OP_ASGN:
      if(DEBUG) printf("Got to OP_ASGN\n");
      assignTarget(exp, *exp->target);
      // Load values
      // printf("OP_ASGN FOUND\n");
      if(exp->r_operand != NULL) {// Load the address used for assignment
        if(DEBUG) printf("r_operand != NULL\n");
        // printf("LAA %d\n", exp->address);
        sprintf(instructionList[instructionCounter++], "LAA %d", exp->address);
        exprgen(exp->r_operand);
      }
      if(exp->l_operand != NULL) {// This check is probably unnecessary
        if(DEBUG) printf("l_operand != NULL\n");
        exprgen(exp->l_operand);
      }
      // if(exp->r_operand != NULL) // Why is this check here?
      //   exprgen(exp->r_operand);
      if(exp->l_operand == NULL && exp->r_operand != NULL) {
        if(DEBUG) printf("Got to STO\n");
        // Surprisingly, checking if r_operand is not NULL seems to have fixed
        // an issue where the STO instruction was being printed more than
        // once.
        // printf("STO\n");
        sprintf(instructionList[instructionCounter++], "STO");
      }
      break;

    case OP_UMIN:
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("NGI\n");
        sprintf(instructionList[instructionCounter++], "NGI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("NGF");
        sprintf(instructionList[instructionCounter++], "NGF");
      }
      break;

    case OP_ADD:
      // printf("GOT TO OP_ADD PORTION!!!\n");
      // printf("exp->type: %d\n", exp->type);
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("ADI\n");
        sprintf(instructionList[instructionCounter++], "ADI");
      }
      if(exp->target->type == TYPE_REAL) {
        // printf("ADF\n");
        sprintf(instructionList[instructionCounter++], "ADF");
      }
      // if(seenReal == 1) {
      //   printf("ADF\n");
      // }
      // else if(seenReal == 0) printf("ADI\n");
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
      if(exp->target->type == TYPE_INT) {
        // printf("SBI\n");
        sprintf(instructionList[instructionCounter++], "SBI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("SBF\n");
        sprintf(instructionList[instructionCounter++], "SBF");
      }
      break;

    case OP_MUL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("MLI\n");
        sprintf(instructionList[instructionCounter++], "MLI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("MLF\n");
        sprintf(instructionList[instructionCounter++], "MLF");
      }
      break;

    case OP_DIV:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("DVI\n");
        sprintf(instructionList[instructionCounter++], "DVI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("DVF\n");
        sprintf(instructionList[instructionCounter++], "DVF");
      }
      break;

    case OP_LSTHN:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("LTI\n");
        sprintf(instructionList[instructionCounter++], "LTI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("LTF\n");
        sprintf(instructionList[instructionCounter++], "LTF");
      }
      break;

    case OP_LSTHNEQL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("LEI\n");
        sprintf(instructionList[instructionCounter++], "LEI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("LEF\n");
        sprintf(instructionList[instructionCounter++], "LEF");
      }
      break;

    case OP_GRTHN:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("GTI\n");
        sprintf(instructionList[instructionCounter++], "GTI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("GTF\n");
        sprintf(instructionList[instructionCounter++], "GTF");
      }
      break;

    case OP_GRTHNEQL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("GEI\n");
        sprintf(instructionList[instructionCounter++], "GEI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("GEF\n");
        sprintf(instructionList[instructionCounter++], "GEF");
      }
      break;

    case OP_EQUAL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("EQI\n");
        sprintf(instructionList[instructionCounter++], "EQI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("EQF\n");
        sprintf(instructionList[instructionCounter++], "EQF");
      }
      break;

    case OP_NEQUAL:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // printf("NEI\n");
        sprintf(instructionList[instructionCounter++], "NEI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("NEF\n");
        sprintf(instructionList[instructionCounter++], "NEF");
      }
      break;

    case OP_AND: // These Boolean instructions require a bit of clever faking
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // Boolean and = multiplication
        // printf("MLI\n");
        // printf("LLI 1\n");
        // printf("EQI\n"); // Check if l_op * r_op = 1
        sprintf(instructionList[instructionCounter++], "MLI");
        sprintf(instructionList[instructionCounter++], "LLI 1");
        sprintf(instructionList[instructionCounter++], "EQI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("MLF\n");
        // printf("LLF 1.0\n");
        // printf("EQF\n");
        sprintf(instructionList[instructionCounter++], "MLF");
        sprintf(instructionList[instructionCounter++], "LLF 1.0");
        sprintf(instructionList[instructionCounter++], "EQF");
      }
      break;

    case OP_OR:
      if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      if(exp->target->type == TYPE_INT) {
        // Boolean or = addition
        // printf("ADI\n");
        // printf("LLI 1\n");
        // printf("EQI\n");
        sprintf(instructionList[instructionCounter++], "ADI");
        sprintf(instructionList[instructionCounter++], "LLI 1");
        sprintf(instructionList[instructionCounter++], "EQI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("ADF\n");
        // printf("LLF 1.0\n");
        // printf("EQF\n");
        sprintf(instructionList[instructionCounter++], "ADF");
        sprintf(instructionList[instructionCounter++], "LLF 1.0");
        sprintf(instructionList[instructionCounter++], "EQF");
      }
      break;

    case OP_NOT:
      // if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgen(exp->r_operand);
      // printf("exp->type %d\n", exp->type);
      if(exp->target->type == TYPE_INT) {
        // Boolean not = complement
        // printf("LLI 0\n");
        // printf("NEI\n"); // If exp != 0, then exp is true
        sprintf(instructionList[instructionCounter++], "LLI 0");
        sprintf(instructionList[instructionCounter++], "NEI");
      }
      else if(exp->target->type == TYPE_REAL) {
        // printf("exp->type: %d\n", exp->value);
        // printf("LLF 0.0\n");
        // printf("NEF\n");
        sprintf(instructionList[instructionCounter++], "LLF 0.0");
        sprintf(instructionList[instructionCounter++], "NEF");
      }
      break;

  }
  if(DEBUG) printf("Finished exprgen\n");
}

struct ast_expression createExp(char kind, char operator, int value) {
  struct ast_expression expr;
  expr.kind = kind;
  expr.operator = operator;
  expr.value = value;
  return expr;
}
