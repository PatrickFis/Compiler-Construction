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

// Inserts statements into the abstract syntax tree.
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
 *  This function goes through the abstract syntax tree and calls various functions
 *  to generate GSTAL code for if statements, while statements, counting loops,
 *  and expressions. It stores this code in an array and will print the
 *  contents of the array after the final statement is found.
 */
void printList() {
  struct statement *next;
  next = list;
  if(next == NULL) { // If this is a program with no expressions just stop compiling.
    printf("Passed in a program with an empty algorithm section. Stopping.\n");
    return;
  }
  sprintf(instructionList[instructionCounter++], "ISP %d", table->memorySize);
  // printf("ISP %d\n", table->memorySize);
  while(next->link != NULL) {
    // printf("Calling exprgen\n"); // Debug
    // Section that generates code for if statements
    if(next->isCond == 1) {
      codeGenIfv2(next);
      next = next->link;
      continue;
  }
  // End if section

  // Section that generates code for while statements
  if(next->isWhile == 1) {
    codeGenWhile(next);
    next = next->link;
    continue;
  }
  // End while section

  // Section that generates code for counting statements
  if(next->isCount == 1) {
    codeGenCount(next);
    next = next->link;
    continue;
  }
  // End counting section

  exprgenv2(next->exp);
  next = next->link;
  }
  sprintf(instructionList[instructionCounter++], "HLT");
  for(int i = 0; i < instructionCounter; i++) {
    printf("%s\n", instructionList[i]);
  }
}

/*
 *  This function takes an if statement and generates GSTAL code.
 */
void codeGenIfv2(struct statement *next) {
  struct ast_if_stmt *nextCopy = malloc(sizeof(struct ast_if_stmt));
  nextCopy = next->if_stmt;
  int sawElse = 0;

  // Parse the conditional statement
  exprgenv2(nextCopy->conditional_stmt);
  int jumpLocation = instructionCounter;
  sprintf(instructionList[instructionCounter++], "JPF"); // Replace this

  // Need a structure to parse the body statement
  struct statement *bodyCopy = malloc(sizeof(struct statement));
  bodyCopy = nextCopy->body;
  while(bodyCopy->link != NULL) {
    // Parse this like a regular statement!
    if(bodyCopy->isCond == 1) { // If there is an if statement
      codeGenIfv2(bodyCopy);
      bodyCopy = bodyCopy->link;
      continue;
    }
    else if(bodyCopy->isWhile == 1) { // If there is a while statement
      codeGenWhile(bodyCopy);
      bodyCopy = bodyCopy->link;
    }
    else if(bodyCopy->isCount == 1) { // If there is a counting loop
      codeGenCount(bodyCopy);
      bodyCopy = bodyCopy->link;
    }
    else {
      exprgenv2(bodyCopy->exp);
      bodyCopy = bodyCopy->link;
    }
  }
  if(nextCopy->tempLink != NULL) {
    sawElse = 1;
    sprintf(instructionList[jumpLocation], "JPF %d", instructionCounter+1);
    struct statement *tempCopy = nextCopy->tempLink;
    int elseJump = instructionCounter;
    sprintf(instructionList[instructionCounter++], "JMP"); // Replace this
    while(tempCopy->link != NULL) {
      if(tempCopy->isCond == 1) { // If there is an if statement
        codeGenIfv2(tempCopy);
        tempCopy = tempCopy->link;
        continue;
      }
      else if(tempCopy->isWhile == 1) { // If there is a while statement
        codeGenWhile(tempCopy);
        tempCopy = tempCopy->link;
      }
      else if(tempCopy->isCount == 1) { // If there is a counting loop
        codeGenCount(tempCopy);
        tempCopy = tempCopy->link;
      }
      else {
        exprgenv2(tempCopy->exp);
        tempCopy = tempCopy->link;
      }
    }
    int elseAfter = instructionCounter;
    sprintf(instructionList[elseJump], "JMP %d", instructionCounter);
  }
  int iAfter = instructionCounter;
  if(!sawElse) {
    sprintf(instructionList[jumpLocation], "JPF %d", iAfter);
  }
}

/*
 *  This function takes a while statement and generates GSTAL code.
 */
void codeGenWhile(struct statement *next) {
  struct ast_while_stmt *whileCopy = malloc(sizeof(struct ast_while_stmt));
  whileCopy = next->while_stmt; // Copy the while_stmt into whileCopy, handle it like if statements.
  int conditional_location = instructionCounter; // This is the location of the conditional
  exprgenv2(whileCopy->conditional_stmt); // Parse the conditional statement
  int JPF_location = instructionCounter; // Location of jump statement
  sprintf(instructionList[instructionCounter++], "JPF"); // Replace this with the instruction after the while loop

  struct statement *bodyCopy = malloc(sizeof(struct statement)); // Copy the body of the while loop
  bodyCopy = whileCopy->body;
  while(bodyCopy->link != NULL) {
    // Generate code for the body of the loop
    if(bodyCopy->isCond == 1) { // If loop contains an if statement
      codeGenIfv2(bodyCopy);
      bodyCopy = bodyCopy->link;
    }
    else if(bodyCopy->isWhile == 1) { // If loop contains a while statement
      codeGenWhile(bodyCopy);
      bodyCopy = bodyCopy->link;
    }
    else if(bodyCopy->isCount == 1) { // If this is a nested counting loop
      codeGenCount(bodyCopy);
      bodyCopy = bodyCopy->link;
    }
    else {
      exprgenv2(bodyCopy->exp);
      bodyCopy = bodyCopy->link;
    }
  }
  sprintf(instructionList[JPF_location], "JPF %d", instructionCounter+1); // Put location in JPF
  sprintf(instructionList[instructionCounter++], "JMP %d", conditional_location); // Jump back to conditional
}

/*
 *  This function takes a counting loop and generates GSTAL code.
 */
void codeGenCount(struct statement *next) {
  struct ast_counting_stmt *countCopy = malloc(sizeof(struct ast_counting_stmt));
  countCopy = next->count_stmt; // Copy the counting statement into countCopy like if and while statements
  int baseAddr = countCopy->target_assignment->address; // Store the address of the counting variable
  exprgenv2(countCopy->target_assignment); // Assign the target variable the value stored in startexpr
  int loop_location = instructionCounter; // Store the location that should be jumped to after the loop iterates
  // Load the counting variable
  for(int i = 0; i < table->count; i++) {
    // So if an array is used as the counting variable it is entirely possible that
    // the target address will not be in the symbol table. This should fix any
    // loading issues.
    struct symbol_table_entry *tempTableEntry = malloc(sizeof(struct symbol_table_entry));
    tempTableEntry = &table->table[i];
    if(baseAddr == tempTableEntry->address) {
      // printf("i = %d\n", i);
      baseAddr = i;
      // countCopy->target_assignment->address = i;
      break;
    }
  }
  if(table->table[baseAddr].kind == KIND_ARRAY) {
    // This is executed if the counting variable is an array reference
    baseAddr = countCopy->target_assignment->address;
    sprintf(instructionList[instructionCounter++], "LLI %d", baseAddr);
    exprgenv2(countCopy->target_assignment->l_operand);
    sprintf(instructionList[instructionCounter++], "ADI");
    sprintf(instructionList[instructionCounter++], "LOD");
  }
  else {
    sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
    sprintf(instructionList[instructionCounter++], "LOD");
  }
  // Assign the counting variable as a target for endexpr, this just makes generating an expression easier
  countCopy->endexpr->target = &table->table[countCopy->target_assignment->address];
  // Generate code for the end expression
  exprgenv2(countCopy->endexpr);
  if(countCopy->direction == 1) { // If counting upwards, do this
    sprintf(instructionList[instructionCounter++], "LEI");
  }
  else if(countCopy->direction == 0) { // If counting downwards, do this
    sprintf(instructionList[instructionCounter++], "GEI");
  }
  int JPF_loc = instructionCounter;
  sprintf(instructionList[instructionCounter++], "JPF"); // Replace this
  struct statement *bodyCopy = malloc(sizeof(struct statement)); // Copy the body of the counting statement
  bodyCopy = countCopy->body;
  while(bodyCopy->link != NULL) {
    // Generate code for the body of the loop
    if(bodyCopy->isCond == 1) { // If loop contains an if statement
      codeGenIfv2(bodyCopy);
      bodyCopy = bodyCopy->link;
    }
    else if(bodyCopy->isWhile == 1) { // If loop contains a while statement
      codeGenWhile(bodyCopy);
      bodyCopy = bodyCopy->link;
    }
    else if(bodyCopy->isCount == 1) { // If this is a nested counting loop
      codeGenCount(bodyCopy);
      bodyCopy = bodyCopy->link;
    }
    else { // If the next thing in the loop is an expression
      exprgenv2(bodyCopy->exp);
      bodyCopy = bodyCopy->link;
    }
  }
  if(countCopy->direction == 1) {
    // Add one to the target variable
    if(table->table[baseAddr].kind == KIND_ARRAY) {
      // This must be done to properly increment the counting variable if it is
      // an array reference
      sprintf(instructionList[instructionCounter++], "LLI %d", baseAddr);
      exprgenv2(countCopy->target_assignment->l_operand);
      sprintf(instructionList[instructionCounter++], "ADI");
      sprintf(instructionList[instructionCounter++], "LLI %d", baseAddr);
      exprgenv2(countCopy->target_assignment->l_operand);
      sprintf(instructionList[instructionCounter++], "ADI");
      sprintf(instructionList[instructionCounter++], "LOD");
    }
    else {
      sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
      sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
      sprintf(instructionList[instructionCounter++], "LOD");
    }
    if(table->table[countCopy->target_assignment->address].type == 1) {
      // If counting variable is a real, convert it.
      sprintf(instructionList[instructionCounter++], "FTI");
    }
    sprintf(instructionList[instructionCounter++], "LLI 1");
    sprintf(instructionList[instructionCounter++], "ADI");
    if(table->table[countCopy->target_assignment->address].type == 1) {
      // If counting variable is a real, convert it back to a floating point number.
      sprintf(instructionList[instructionCounter++], "ITF");
    }
    sprintf(instructionList[instructionCounter++], "STO");
    sprintf(instructionList[instructionCounter++], "JMP %d", loop_location);
    sprintf(instructionList[JPF_loc], "JPF %d", instructionCounter);
  }
  else if(countCopy->direction == 0) {
    // Subtract one from the target variable
    if(table->table[baseAddr].kind == KIND_ARRAY) {
      // This must be done to properly increment the counting variable if it is
      // an array reference
      sprintf(instructionList[instructionCounter++], "LLI %d", baseAddr);
      exprgenv2(countCopy->target_assignment->l_operand);
      sprintf(instructionList[instructionCounter++], "ADI");
      sprintf(instructionList[instructionCounter++], "LLI %d", baseAddr);
      exprgenv2(countCopy->target_assignment->l_operand);
      sprintf(instructionList[instructionCounter++], "ADI");
      sprintf(instructionList[instructionCounter++], "LOD");
    }
    else {
      sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
      sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
      sprintf(instructionList[instructionCounter++], "LOD");
    }
    if(table->table[countCopy->target_assignment->address].type == 1) {
      // If counting variable is a real, convert it.
      sprintf(instructionList[instructionCounter++], "FTI");
    }
    sprintf(instructionList[instructionCounter++], "LLI 1");
    sprintf(instructionList[instructionCounter++], "SBI");
    if(table->table[countCopy->target_assignment->address].type == 1) {
      // If counting variable is a real, convert it back to a floating point number.
      sprintf(instructionList[instructionCounter++], "ITF");
    }
    sprintf(instructionList[instructionCounter++], "STO");
    sprintf(instructionList[instructionCounter++], "JMP %d", loop_location);
    sprintf(instructionList[JPF_loc], "JPF %d", instructionCounter);
  }
}

/*
*  This function recursively assigns targets to expressions, which allows the
*  code generator to know what kind of instructions to print.
*/
void assignTarget(struct ast_expression *exp, struct symbol_table_entry target) {
  if(exp->type == TYPE_VAR) {
    // printf("I returned here\n");
    return;
  }
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
    if(DEBUG) printf("exp->type = %d\n", exp->type);
    if(exp->kind == KIND_OP) return;
    exp->type = target.type;
    // printf("exp->type = %d\n", exp->type);
  }
  if(exp->r_operand == NULL && exp->type != TYPE_VAR) {
    if(DEBUG) printf("exp->kind: %d\n", exp->kind);
    if(exp->kind == KIND_OP) return;
    exp->type = target.type;
    // printf("exp->type = %d\n", exp->type);
  }
}

void parsePrintv3(struct ast_expression *exp) {
  struct ast_expression *x = exp->r_operand;
  if(x != NULL) {
    if(x->charString != NULL) {
      int i;
      // Print a newline character
      if(strcmp(x->charString, "!,") == 0 || strcmp(x->charString, "!") == 0) {
        sprintf(instructionList[instructionCounter++], "PTL");
      }
      // Print characters enclosed by quotation marks. Will add a check for "" later.
      for(i = 1; i < strlen(x->charString)-1; i++) {
        sprintf(instructionList[instructionCounter++], "LLI %d", (int)x->charString[i]);
        sprintf(instructionList[instructionCounter++], "PTC");
        if(strcmp(instructionList[instructionCounter-2], "LLI 34") == 0) {
          // If a quotation mark was just printed, skip over the next character
          // in the input.
          i++;
        }
      }
    }
    /*
    *  Tree is structed like this... exp->r_operand->r_operand->r_operand...
    *  If r_operand has a charString, then it should be printed.
    *  If the charString is NULL, then r_operand has a non-NULL l_operand.
    *  l_operand is a bexp, which should be passed to exprgen for code generation.
    */
    if(x->charString == NULL) {
      // x->l_operand->target = &table->table[0]; // This line is no longer necessary after some modifications
      int iCounterBefore = instructionCounter;
      exprgenv2(x->l_operand);
      int iCounterAfter = instructionCounter;
      if(DEBUG) printf("iB: %d, iA: %d\n", iCounterBefore, iCounterAfter);

      /* Get the first instruction inserted by the above call to exprgen and
       * find the address used by it. May need to change this to scan all
       * instructions generated between iCounterBefore and iCounterAfter.
       * Getting the tokens from a copy because apparently using strtok modifies
       * the target of the function.
       */
      char *iListCopy = malloc(sizeof(strlen(instructionList[iCounterBefore])));
      strcpy(iListCopy, instructionList[iCounterBefore]);
      char *targetToken = strtok(iListCopy, " ");
      targetToken = strtok(NULL, " ");
      int tar = atoi(targetToken);
      /* So if there are multiple arrays in a program tar may get the wrong number,
       * since it actually get's the address of something.
       * The for loop should fix that issue.
       */
      struct symbol_table_entry *findTarget = malloc(sizeof(struct symbol_table_entry));
      for(int i = 0; i < table->count; i++) {
        findTarget = &table->table[i];
        if(tar == findTarget->address) {
          tar = i;
          break;
        }
      }
      int tarType = table->table[tar].type;
      if(tarType == 1) {
        sprintf(instructionList[instructionCounter++], "PTF");
      }
      else if(tarType == 0) {
        sprintf(instructionList[instructionCounter++], "PTI");
      }
    }
    parsePrintv3(x);
  }
}

/*
 *  This code goes through expressions and generates code based on an operator
 *  supplied by the parser. It does some error checking in case an expression
 *  with an array is passed in as an argument.
 */
void exprgenv2(struct ast_expression *exp) {
  struct symbol_table_entry *tempInt = malloc(sizeof(struct symbol_table_entry));
  struct symbol_table_entry *tempReal = malloc(sizeof(struct symbol_table_entry));
  tempInt->type = TYPE_INT;
  tempReal->type = TYPE_REAL;
  if(exp->target == NULL) {
    // If the expression doesn't have a target assigned, look back through the symbol table for recent loads.
    int i;
    for(i = instructionCounter-1; i > 0; i--) {
      char *strCpy = malloc(sizeof(instructionList[i]));
      strcpy(strCpy, instructionList[i]);
      char *ins = strtok(strCpy, " "); // Get the instruction
      if(strcmp(ins, "ITF") == 0) {
        exp->target = tempReal;
        break;
      }
      else if(strcmp(ins, "FTI") == 0) {
        exp->target = tempInt;
        break;
      }
      else if(strcmp(ins, "LAA") == 0) {
        ins = strtok(NULL, " "); // Get address
        if(table->table[atoi(ins)].type == TYPE_INT) {
          exp->target = tempInt;
          break;
        }
        else if(table->table[atoi(ins)].type == TYPE_REAL) {
          exp->target = tempReal;
          break;
        }
      }
    }
  }
  if(exp->operator == OP_EXIT) {
    if(DEBUG) printf("Found an OP_EXIT\n");
    sprintf(instructionList[instructionCounter++], "HLT");
    return;
  }
  if(exp->operator == OP_PRINT) { // This occurs if an expression is a print statement
    parsePrintv3(exp);
    return;
  }
  if(exp->operator == OP_READ) {
    sprintf(instructionList[instructionCounter++], "LAA %d", exp->address);
    if(table->table[exp->address].type == 0) {
      sprintf(instructionList[instructionCounter++], "INI");
      sprintf(instructionList[instructionCounter++], "STO");
    }
    else if(table->table[exp->address].type == 1) {
      sprintf(instructionList[instructionCounter++], "INF");
      sprintf(instructionList[instructionCounter++], "STO");
    }
    return;
  }
  if(exp->kind == KIND_INT && (exp->type != TYPE_VAR && exp->type != TYPE_ARRAY)) { // If expression involves integers
    // Had to add another check to make sure that exp->type is not an array.
    if(DEBUG) printf("Got to load int\n");
    // printf("LLI %d\n", exp->value);
    // printf("exp->type = %d\n", exp->type);
    sprintf(instructionList[instructionCounter++], "LLI %d", exp->value);
    if(exp->target->type == 1 /*&& exp->target->kind != KIND_ARRAY*/) sprintf(instructionList[instructionCounter++], "ITF");
  }
  else if(exp->kind == KIND_REAL && (exp->type != TYPE_VAR && exp->type != TYPE_ARRAY)) { // If expression involves reals
    if(DEBUG) printf("Got to load real\n");
    sprintf(instructionList[instructionCounter++], "LLF %f", exp->rvalue);
    // if(exp->target->kind == KIND_ARRAY) sprintf(instructionList[instructionCounter++], "FTI");
    if(exp->target->type == 0) sprintf(instructionList[instructionCounter++], "FTI");
  }
  if(exp->type == TYPE_VAR) {
    if(DEBUG) printf("Got to variable type\n");
    // printf("LAA %d\n",exp->l_operand->target->address);
    // printf("LOD\n");
    sprintf(instructionList[instructionCounter++], "LAA %d", exp->l_operand->target->address + exp->l_operand->arrayOffset);
    sprintf(instructionList[instructionCounter++], "LOD");
    if(exp->l_operand->target->type != exp->target->type) {
      if(exp->target->type == 0) { // Real to integer
        sprintf(instructionList[instructionCounter++], "FTI");
      }
      else if(exp->target->type == 1) { // Integer to real
        sprintf(instructionList[instructionCounter++], "ITF");
      }
    }
    if(DEBUG) printf("Finished variable type\n"); // Debug
    return; // Just stop the recursion when you reach a variable reference
  }
  if(exp->type == TYPE_ARRAY) {
    // Had to add a check to a conditional above this to ensure that random loads
    // were not getting printed. Note that this will most likely only be
    // reached if the statement is not an assignment statement.
    if(DEBUG) printf("Got to array type\n");
    if(exp->r_operand != NULL) { // The array index is stored in r_operand
      if(DEBUG) printf("R op not null\n");
      // Load the base address of the array, evaluate the r_operand, and then
      // add them together to get the correct memory address.
      sprintf(instructionList[instructionCounter++], "LLI %d", exp->l_operand->target->address);
      exprgenv2(exp->r_operand);
      if(strcmp(instructionList[instructionCounter - 1], "ITF") == 0) {
        instructionCounter--;
      }
      sprintf(instructionList[instructionCounter++], "ADI");
      sprintf(instructionList[instructionCounter++], "LOD");
      return;
    }
  }
  if(DEBUG) printf("Got to switch statement\n");
  switch(exp->operator) {
    case OP_ASGN:
      if(DEBUG) printf("Got to OP_ASGN\n");
      // Load values
      if(exp->target->kind == KIND_ARRAY) {
        // This portion will be used if an array reference is found.
        // I noticed that my array's required integer values to be used as the
        // reference, and this is obviously not the most optimal solution.
        // l_operand will store the array location, which will have to be
        // evaluated and then added to the base address of the array.
        // After this is done, the array location can be used for loading and
        // storing values.
        exprgenv2(exp->l_operand); // Evaluate array location
        if(strcmp(instructionList[instructionCounter - 1], "ITF") == 0) {
          instructionCounter--;
        }
        sprintf(instructionList[instructionCounter++], "LLI %d", exp->address); // Load base address
        sprintf(instructionList[instructionCounter++], "ADI"); // Add the array location and base address together
        exprgenv2(exp->r_operand); // Evaluate the assignment portion of this expression
        sprintf(instructionList[instructionCounter++], "STO");
        return;
      }
      if(exp->r_operand != NULL) {// Load the address used for assignment
        if(DEBUG) printf("r_operand != NULL\n");
        sprintf(instructionList[instructionCounter++], "LAA %d", exp->address);
        exprgenv2(exp->r_operand);
      }
      if(exp->l_operand != NULL) {// This check is probably unnecessary
        // This check may actually be useful for array references
        if(DEBUG) printf("l_operand != NULL\n");
        exprgenv2(exp->l_operand);
      }
      if(exp->l_operand == NULL && exp->r_operand != NULL) {
        if(DEBUG) printf("Got to STO\n");
        // Surprisingly, checking if r_operand is not NULL seems to have fixed
        // an issue where the STO instruction was being printed more than
        // once.
        sprintf(instructionList[instructionCounter++], "STO");
      }
      break;

    case OP_UMIN:
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "NGI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "NGF");
      }
      break;

    case OP_ADD:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "ADI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "ADF");
      }
      break;

    case OP_SUB:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "SBI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "SBF");
      }
      break;

    case OP_MUL:
      if(DEBUG) printf("Got to OP_MUL\n");
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "MLI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "MLF");
      }
      break;

    case OP_DIV:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "DVI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "DVF");
      }
      break;

    case OP_LSTHN:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "LTI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "LTF");
      }
      break;

    case OP_LSTHNEQL:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "LEI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "LEF");
      }
      break;

    case OP_GRTHN:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "GTI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "GTF");
      }
      break;

    case OP_GRTHNEQL:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "GEI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "GEF");
      }
      break;

    case OP_EQUAL:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "EQI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "EQF");
      }
      break;

    case OP_NEQUAL:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "NEI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "NEF");
      }
      break;

    case OP_AND: // These Boolean instructions require a bit of clever faking
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        // Boolean and = multiplication
        // If the result is != 0, then the result is true.
        sprintf(instructionList[instructionCounter++], "MLI");
        sprintf(instructionList[instructionCounter++], "LLI 0");
        sprintf(instructionList[instructionCounter++], "NEI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "MLF");
        sprintf(instructionList[instructionCounter++], "LLF 0.0");
        sprintf(instructionList[instructionCounter++], "NEF");
      }
      break;

    case OP_OR:
      if(exp->l_operand != NULL) exprgenv2(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        // Boolean or = addition
        sprintf(instructionList[instructionCounter++], "ADI");
        sprintf(instructionList[instructionCounter++], "LLI 0");
        sprintf(instructionList[instructionCounter++], "NEI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "ADF");
        sprintf(instructionList[instructionCounter++], "LLF 0.0");
        sprintf(instructionList[instructionCounter++], "NEF");
      }
      break;

    case OP_NOT:
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        // Boolean not = complement
         // If exp != 0, then exp is true
        sprintf(instructionList[instructionCounter++], "LLI 0");
        sprintf(instructionList[instructionCounter++], "NEI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "LLF 0.0");
        sprintf(instructionList[instructionCounter++], "NEF");
      }
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
