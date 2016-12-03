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
*
*  It now also will parse if statements, with if else statements being worked on.
*  I am considering moving the parsing of if and if else statements to another
*  function so that printList is cleaner.
*  A nested if statement can be found at next->link->if_stmt->if_link->body->link...
*/
void printList() {
  struct statement *next;
  next = list;
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

  // Section that generates code for exit statements
  // if(next->isExit == 1) {
  //   printf("FOUND AN EXIT?\n");
  //   sprintf(instructionList[instructionCounter++], "HLT");
  //   next = next->link;
  //   continue;
  // }
  // End exit section

  int iBefore = instructionCounter;
  // exprgen(next->exp);
  exprgenv2(next->exp);
  int iAfter = instructionCounter;
  // checkInstructionsv2(iBefore, iAfter);
  next = next->link;
  }
  sprintf(instructionList[instructionCounter++], "HLT");
  for(int i = 0; i < instructionCounter; i++) {
    printf("%s\n", instructionList[i]);
  }
}

// So this seems to be working.
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
  // if(nextCopy->isIfElse == 1) {
  //   // printf("Houston, we have an else statement\n");
  // }
  if(nextCopy->tempLink != NULL) {
    sawElse = 1;
    sprintf(instructionList[jumpLocation], "JPF %d", instructionCounter+1);
    struct statement *tempCopy = nextCopy->tempLink;
    int elseJump = instructionCounter;
    sprintf(instructionList[instructionCounter++], "JMP"); // Replace this
    while(tempCopy->link != NULL) {
      // exprgen(tempCopy->exp);
      // tempCopy = tempCopy->link;
      // if(tempCopy->isCond == 1) {
      //   codeGenIfv2(tempCopy);
      //   tempCopy = tempCopy->link;
      //   continue;
      // }
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

void codeGenCount(struct statement *next) {
  struct ast_counting_stmt *countCopy = malloc(sizeof(struct ast_counting_stmt));
  countCopy = next->count_stmt; // Copy the counting statement into countCopy like if and while statements
  exprgenv2(countCopy->target_assignment); // Assign the target variable the value stored in startexpr
  int loop_location = instructionCounter; // Store the location that should be jumped to after the loop iterates
  // Load the counting variable
  sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
  sprintf(instructionList[instructionCounter++], "LOD");
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
    sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
    sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
    sprintf(instructionList[instructionCounter++], "LOD");
    sprintf(instructionList[instructionCounter++], "LLI 1");
    sprintf(instructionList[instructionCounter++], "ADI");
    sprintf(instructionList[instructionCounter++], "STO");
    sprintf(instructionList[instructionCounter++], "JMP %d", loop_location);
    sprintf(instructionList[JPF_loc], "JPF %d", instructionCounter);
  }
  else if(countCopy->direction == 0) {
    // Subtract one from the target variable
    sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
    sprintf(instructionList[instructionCounter++], "LAA %d", countCopy->target_assignment->address);
    sprintf(instructionList[instructionCounter++], "LOD");
    sprintf(instructionList[instructionCounter++], "LLI 1");
    sprintf(instructionList[instructionCounter++], "SBI");
    sprintf(instructionList[instructionCounter++], "STO");
    sprintf(instructionList[instructionCounter++], "JMP %d", loop_location);
    sprintf(instructionList[JPF_loc], "JPF %d", instructionCounter);
  }
}

void checkInstructionsv2(int iBefore, int iAfter) {
  int i;
  int seenReal = 0;
  // This portion will check the first instruction for a load and the last
  // instruction for a store.
  char *beforeCopy = malloc(sizeof(strlen(instructionList[iBefore])));
  strcpy(beforeCopy, instructionList[iBefore]);
  char *instBefore = strtok(beforeCopy, " ");
  if(strcmp(instBefore, "LAA") == 0) {
    // First instruction is a load, so grab address and check last instruction
    char *tar = strtok(NULL, " "); // This is the address
    int type = table->table[atoi(tar)].type; // Get the type from the symbol table
    char *lastCopy = malloc(sizeof(strlen(instructionList[iAfter-1]))); // Last instruction
    strcpy(lastCopy, instructionList[iAfter-1]);
    char *instAfter = strtok(lastCopy, " ");
    if(strcmp(instAfter, "STO") == 0) {
      printf("Target name: %s, target type: %d, tar variable: %s\n", table->table[atoi(tar)].name, table->table[atoi(tar)].type, tar);
      printf("This is an assignment statement. Check the instructions and do conversions.\n");
      if(type == 0) {
        // Integer conversions, don't need to check first or last instruction
        for(i = iBefore+1; i < iAfter-1; i++) {
          char *iCopy = malloc(sizeof(strlen(instructionList[i])));
          strcpy(iCopy, instructionList[i]);
          char *tokens = strtok(iCopy, " "); // Instruction
          if(strcmp(tokens, "LLF") == 0) { // If this is a real number being loaded
            strcat(instructionList[i], "\nFTI");
          }
          if(strcmp(tokens, "LAA") == 0) { // If a load is about to be performed
            tokens = strtok(NULL, " "); // Address of load
            if(table->table[atoi(tokens)].type == 1) { // Loading a real
              strcat(instructionList[i+1], "\nFTI");
            }
          }
        }
      }
    }
  }
  // for(i = iBefore; i < iAfter; i++) {
  //   // Copy the instruction at i into iCopy so that strtok doesn't modify
  //   // the original instruction.
  //   char *iCopy = malloc(sizeof(strlen(instructionList[i])));
  //   strcpy(iCopy, instructionList[i]);
  //   // tar will hold the instruction
  //   char *tar = strtok(iCopy, " "); // This is the instruction
  //   if()
  // }
}
/*
*  This function is called after exprgen in printList. It makes sure that
*  the correct instruction types are printed. May need to add another check
*  to do conversions of loaded variables that are of the wrong type.
*/
void checkInstructions(int iBefore, int iAfter) {
  int i;
  int seenReal = 0;
  for(i = iBefore; i < iAfter; i++) {
    if(instructionList[i][0] == 'L' && instructionList[i][1] == 'A' && instructionList[i][2] == 'A') {
      // Using iCopy like the below iListCopy.
      char *iCopy = malloc(sizeof(strlen(instructionList[i])));
      strcpy(iCopy, instructionList[i]);
      char *tar = strtok(iCopy, " "); // = LAA
      tar = strtok(NULL, " "); // = Number
      int tarType = table->table[atoi(tar)].type;
      if(tarType == 1) {
        seenReal = 1; // If these instructions contain a real, all the instructions need to change
      }
    }
    if(DEBUG) printf("iList: %s\n", instructionList[i]);
  }
  if(seenReal == 1) { // Need to check if any other instructions besides operations end with I.
    for(i = iBefore; i < iAfter; i++) {
      // printf("i = %d\n", i);
      if(i+1 < iAfter) {
        char *iCopy = malloc(sizeof(strlen(instructionList[i])));
        strcpy(iCopy, instructionList[i]);
        char *inst = strtok(iCopy, " "); // Checking if this is LLI
        // printf("inst = %s\n", inst);
        if(strcmp(inst, "LLI")) {
          char *iCopy2 = malloc(sizeof(strlen(instructionList[i+1])));
          strcpy(iCopy2, instructionList[i+1]);
          char *ptcCheck = strtok(iCopy2, " "); // Checking if this is PTC
          if(strcmp(ptcCheck, "PTC")) {
            // printf("Hi");
            // printf("continue\n");
            i++;
            continue;
          }
        }
      }
      if(instructionList[i][2] == 'I') {
        instructionList[i][2] = 'F'; // Change to floating point instructions
        // If the instruction ends with a number, append a .0 to it.
        if(strlen(instructionList[i]) > 3) {
          strcat(instructionList[i], ".0");
        }
      }
    }
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
*  Checkpoint 7 - Found a bug in this that caused expressions with more than one
*  operation to fail. Fixed by returning from this function if exp->kind
*  is equal to KIND_OP.
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
  // if(x->charString == NULL) printf("NULL\n");
  // char *giantString = malloc(sizeof(char));
  if(x != NULL) {
    if(x->charString != NULL) {
      // printf("%s\n", x->charString);
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
        if(strcmp(instructionList[instructionCounter-2], "LLI 34") == 0) {
          // If a quotation mark was just printed, skip over the next character
          // in the input.
          i++;
        }
      }
    }
    // if(x->r_operand != NULL) {
    //   if(strcmp(instructionList[instructionCounter-1], "PTL") == 0) {
    //     sprintf(instructionList[instructionCounter++], "LLI 34");
    //     sprintf(instructionList[instructionCounter++], "PTC");
    //   }
    // }
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

      // Get the first instruction inserted by the above call to exprgen and
      // find the address used by it. May need to change this to scan all
      // instructions generated between iCounterBefore and iCounterAfter.
      // Getting the tokens from a copy because apparently using strtok modifies
      // the target of the function.
      char *iListCopy = malloc(sizeof(strlen(instructionList[iCounterBefore])));
      strcpy(iListCopy, instructionList[iCounterBefore]);
      char *targetToken = strtok(iListCopy, " ");
      targetToken = strtok(NULL, " ");
      int tar = atoi(targetToken);
      int tarType = table->table[tar].type;
      // printf("tar = %d\n", tar);
      // printf("tar type = %d\n", table->table[tar].type);
      // printf("%s\n", targetToken);
      // This loops replaces instructions with the correct version for their
      // target's type.
      for(int i = iCounterBefore; i < iCounterAfter; i++) {
        if(tarType == 1) {
          // If the target is a real number
          // if(instructionList[i][2] == 'I') {
          //   instructionList[i][2] = 'F';
          // }
        }
        else if(tarType == 0) {
          // If the target is an integer number
          // if(instructionList[i][2] == 'F') {
          //   instructionList[i][2] = 'I';
          // }
        }
        // printf("%s\n", instructionList[i]);
      }
      // printf("\n\n\n\n\n");
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

  // printf("iCounter = %d\n", instructionCounter);
  while(x != NULL) {
    if(x->charString != NULL) {
      printf("not null x->charString %s\n", x->charString);
    }
    if(x->charString == NULL) {
      x->target = &table->table[0];
      // This, and iCounterAfter, will be used to determine if the correct instructions were printed
      int iCounterBefore = instructionCounter;
      exprgen(x);
      int iCounterAfter = instructionCounter;
      // printf("iB %d iA %d\n", iCounterBefore, iCounterAfter);
      // Get the first instruction inserted by the above call to exprgen and
      // find the address used by it. May need to change this to scan all
      // instructions generated between iCounterBefore and iCounterAfter.
      // Getting the tokens from a copy because apparently using strtok modifies
      // the target of the function.
      char *iListCopy = malloc(sizeof(strlen(instructionList[iCounterBefore])));
      strcpy(iListCopy, instructionList[iCounterBefore]);
      char *targetToken = strtok(iListCopy, " ");
      targetToken = strtok(NULL, " ");
      int tar = atoi(targetToken);
      int tarType = table->table[tar].type;
      // printf("tar = %d\n", tar);
      // printf("tar type = %d\n", table->table[tar].type);
      // printf("%s\n", targetToken);
      // This loops replaces instructions with the correct version for their
      // target's type.
      for(int i = iCounterBefore; i < iCounterAfter; i++) {
        if(tarType == 1) {
          // If the target is a real number
          if(instructionList[i][2] == 'I') {
            instructionList[i][2] = 'F';
          }
        }
        else if(tarType == 0) {
          // If the target is an integer number
          if(instructionList[i][2] == 'F') {
            instructionList[i][2] = 'I';
          }
        }
        // printf("%s\n", instructionList[i]);
      }
      // printf("\n\n\n\n\n");
      if(tarType == 1) {
        sprintf(instructionList[instructionCounter++], "PTF");
      }
      else if(tarType == 0) {
        sprintf(instructionList[instructionCounter++], "PTI");
      }
      break;
    }
    // printf("x->charString, %s\n", x->charString);
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
*  Getting really tired of debugging problems with output of instructions.
*  Since all instructions are stored in an array, this instruction will check
*  the last instruction for a LLI/LLF or a LOD. If it finds LLI/LLI, it will
*  return 0 or 1. If it finds LOD, it will check the previous instruction for
*  an address and then check the symbol table for the type.
*/
int getPreviousInstructionType(int iCount) {
  char *iCopy = malloc(sizeof(strlen(instructionList[iCount - 1])));
  strcpy(iCopy, instructionList[iCount - 1]);
  char *ins = strtok(iCopy, " "); // Get instruction
  // String comparisons to get previous type.
  if(strcmp(ins, "LLI") == 0
  ||strcmp(ins, "ADI") == 0
  ||strcmp(ins, "SBI") == 0
  ||strcmp(ins, "MLI") == 0
  ||strcmp(ins, "DVI") == 0
  ||strcmp(ins, "NGI") == 0
  ||strcmp(ins, "EQI") == 0
  ||strcmp(ins, "NEI") == 0
  ||strcmp(ins, "LTI") == 0
  ||strcmp(ins, "LEI") == 0
  ||strcmp(ins, "GTI") == 0
  ||strcmp(ins, "GEI") == 0
  ||strcmp(ins, "FTI") == 0
  ||strcmp(ins, "INI") == 0
  ) {
  return 0;
    }
  else if(strcmp(ins, "LLF") == 0
  ||strcmp(ins, "ADF") == 0
  ||strcmp(ins, "SBF") == 0
  ||strcmp(ins, "MLF") == 0
  ||strcmp(ins, "DVF") == 0
  ||strcmp(ins, "NGF") == 0
  ||strcmp(ins, "EQF") == 0
  ||strcmp(ins, "NEF") == 0
  ||strcmp(ins, "LTF") == 0
  ||strcmp(ins, "LEF") == 0
  ||strcmp(ins, "GTF") == 0
  ||strcmp(ins, "GEF") == 0
  ||strcmp(ins, "ITF") == 0
  ||strcmp(ins, "INF") == 0
    ) {
    return 1;
    }
  else if(strcmp(ins, "LOD") == 0) {
    iCopy = malloc(sizeof(strlen(instructionList[iCount - 2])));
    strcpy(iCopy, instructionList[iCount - 2]);
    ins = strtok(iCopy, " ");
    ins = strtok(NULL, " ");
    return table->table[atoi(ins)].type;
    }
  return -1;
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
    // parsePrintStatementv2(exp);
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
  if(exp->kind == KIND_INT && exp->type != TYPE_VAR) { // If expression involves integers
    if(DEBUG) printf("Got to load int\n");
    // printf("LLI %d\n", exp->value);
    // printf("exp->type = %d\n", exp->type);
    sprintf(instructionList[instructionCounter++], "LLI %d", exp->value);
  }
  else if(exp->kind == KIND_REAL && exp->type != TYPE_VAR) { // If expression involves reals
    if(DEBUG) printf("Got to load real\n");
    // printf("LLF %f\n", exp->rvalue);
    // printf("target = %s type = %d\n", exp->target->name, exp->target->type);
    sprintf(instructionList[instructionCounter++], "LLF %f", exp->rvalue);
    // if(exp->target->type == 0) sprintf(instructionList[instructionCounter++], "FTI");
  }
  if(exp->type == TYPE_VAR) {
    if(DEBUG) printf("Got to variable type\n");
    // printf("LAA %d\n",exp->l_operand->target->address);
    // printf("LOD\n");
    sprintf(instructionList[instructionCounter++], "LAA %d", exp->l_operand->target->address + exp->l_operand->arrayOffset);
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
      // exprgen(exp->l_operand);
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
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "NGI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "NGF");
    }
    break;

    case OP_ADD:
    // printf("GOT TO OP_ADD PORTION!!!\n");
    // printf("exp->type: %d\n", exp->type);
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    // if(exp->target->type == 0) {
    //   sprintf(instructionList[instructionCounter++], "ADI");
    // }
    // else if(exp->target->type == 1) {
    //   sprintf(instructionList[instructionCounter++], "ADF");
    // }
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "ADI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "ADF");
    }
    break;

    case OP_SUB:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "SBI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "SBF");
    }
    break;

    case OP_MUL:
    if(DEBUG) printf("Got to OP_MUL\n");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "MLI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "MLF");
    }
    break;

    case OP_DIV:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "DVI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "DVF");
    }
    break;

    case OP_LSTHN:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "LTI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "LTF");
    }
    break;

    case OP_LSTHNEQL:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "LEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "LEF");
    }
    break;

    case OP_GRTHN:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "GTI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "GTF");
    }
    break;

    case OP_GRTHNEQL:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "GEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "GEI");
    }
    break;

    case OP_EQUAL:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "EQI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "EQF");
    }
    break;

    case OP_NEQUAL:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "NEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "NEF");
    }
    break;

    case OP_AND: // These Boolean instructions require a bit of clever faking
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      // Boolean and = multiplication
      // printf("MLI\n");
      // printf("LLI 1\n");
      // printf("EQI\n"); // Check if l_op * r_op = 1
      sprintf(instructionList[instructionCounter++], "MLI");
      sprintf(instructionList[instructionCounter++], "LLI 0");
      sprintf(instructionList[instructionCounter++], "NEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      // printf("MLF\n");
      // printf("LLF 1.0\n");
      // printf("EQF\n");
      sprintf(instructionList[instructionCounter++], "MLF");
      sprintf(instructionList[instructionCounter++], "LLF 0.0");
      sprintf(instructionList[instructionCounter++], "NEF");
    }
    break;

    case OP_OR:
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      // Boolean or = addition
      sprintf(instructionList[instructionCounter++], "ADI");
      sprintf(instructionList[instructionCounter++], "LLI 0");
      sprintf(instructionList[instructionCounter++], "NEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "ADF");
      sprintf(instructionList[instructionCounter++], "LLF 0.0");
      sprintf(instructionList[instructionCounter++], "NEI");
    }
    break;

    case OP_NOT:
    // if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    // printf("exp->type %d\n", exp->type);
    if(getPreviousInstructionType(instructionCounter) == 0) {
      // Boolean not = complement
      // printf("LLI 0\n");
      // printf("NEI\n"); // If exp != 0, then exp is true
      sprintf(instructionList[instructionCounter++], "LLI 0");
      sprintf(instructionList[instructionCounter++], "NEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "LLF 0.0");
      sprintf(instructionList[instructionCounter++], "NEF");
    }
    break;

  }
  if(DEBUG) printf("Finished exprgen\n");
}

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
        // printf("assigned int\n");
        break;
      }
      else if(strcmp(ins, "FTI") == 0) {
        exp->target = tempInt;
        // printf("assigned real\n");
        break;
      }
      else if(strcmp(ins, "LAA") == 0) {
        ins = strtok(NULL, " "); // Get address
        if(table->table[atoi(ins)].type == TYPE_INT) {
          exp->target = tempInt;
          // printf("assigned int\n");
          break;
        }
        else if(table->table[atoi(ins)].type == TYPE_REAL) {
          exp->target = tempReal;
          // printf("assigned real\n");
          break;
        }
      }
    }
  }
  // if(exp->target == NULL) {
  //   printf("FOUND A NULL TARGET, THIS IS A PROBLEM\n");
  // }
  if(exp->operator == OP_EXIT) {
    if(DEBUG) printf("Found an OP_EXIT\n");
    sprintf(instructionList[instructionCounter++], "HLT");
    return;
  }
  if(exp->operator == OP_PRINT) { // This occurs if an expression is a print statement
    // printf("Got to OP_PRINT\n");
    // if(exp->r_operand != NULL)exprgen(exp->r_operand);
    // struct ast_expression *x = exp->r_operand;
    // while(x != NULL) {
    //   printf("x->charString, %s\n", x->charString);
    //   x = x->r_operand;
    // }
    // printf("exp->charString, %s\n", exp->charString);
    // parsePrintStatementv2(exp);
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
    if(exp->target->type == 1 && exp->target->kind != KIND_ARRAY) sprintf(instructionList[instructionCounter++], "ITF");
  }
  else if(exp->kind == KIND_REAL && (exp->type != TYPE_VAR && exp->type != TYPE_ARRAY)) { // If expression involves reals
    if(DEBUG) printf("Got to load real\n");
    sprintf(instructionList[instructionCounter++], "LLF %f", exp->rvalue);
    if(exp->target->kind == KIND_ARRAY) sprintf(instructionList[instructionCounter++], "FTI");
    else if(exp->target->type == 0) sprintf(instructionList[instructionCounter++], "FTI");
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
      // exp->r_operand->target = exp->l_operand->target;
      exprgenv2(exp->r_operand);
      sprintf(instructionList[instructionCounter++], "ADI");
      sprintf(instructionList[instructionCounter++], "LOD");
      return;
    }
    // if(exp->l_operand->r_operand != NULL) {
    //   // This will be executed if the array index is an expression
    //   printf("Possible array reference?\n");
    //   // Load the base address of the array
    //   sprintf(instructionList[instructionCounter++], "LLI %d", exp->l_operand->target->address);
    //   // Evaluate the array index
    //   printf("Above exp\n");
    //   exprgenv2(exp->l_operand->r_operand);
    //   // Add the value of the index to the base address and load the correct memory location
    //   sprintf(instructionList[instructionCounter++], "ADI");
    //   sprintf(instructionList[instructionCounter++], "LOD");
    //   return;
    // }
  }
  if(DEBUG) printf("Got to switch statement\n");
  switch(exp->operator) {
    case OP_ASGN:
      if(DEBUG) printf("Got to OP_ASGN\n");
      // assignTarget(exp, *exp->target);
      // Load values
      // printf("OP_ASGN FOUND\n");
      if(exp->target->kind == KIND_ARRAY) {
        // This portion will be used if an array reference is found.
        // I noticed that my array's required integer values to be used as the
        // reference, and this is obviously not the most optimal solution.
        // l_operand will store the array location, which will have to be
        // evaluated and then added to the base address of the array.
        // After this is done, the array location can be used for loading and
        // storing values.
        exprgenv2(exp->l_operand); // Evaluate array location
        sprintf(instructionList[instructionCounter++], "LLI %d", exp->address); // Load base address
        sprintf(instructionList[instructionCounter++], "ADI"); // Add the array location and base address together
        exprgenv2(exp->r_operand); // Evaluate the assignment portion of this expression
        sprintf(instructionList[instructionCounter++], "STO");
        return;
      }
      if(exp->r_operand != NULL) {// Load the address used for assignment
        if(DEBUG) printf("r_operand != NULL\n");
        // printf("LAA %d\n", exp->address);
        sprintf(instructionList[instructionCounter++], "LAA %d", exp->address);
        exprgenv2(exp->r_operand);
        // exprgen(exp->l_operand);
      }
      if(exp->l_operand != NULL) {// This check is probably unnecessary
        // This check may actually be useful for array references
        if(DEBUG) printf("l_operand != NULL\n");
        exprgenv2(exp->l_operand);
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
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      if(exp->target->type == 0) {
        sprintf(instructionList[instructionCounter++], "NGI");
      }
      else if(exp->target->type == 1) {
        sprintf(instructionList[instructionCounter++], "NGF");
      }
      break;

    case OP_ADD:
      // printf("GOT TO OP_ADD PORTION!!!\n");
      // printf("exp->type: %d\n", exp->type);
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
        // printf("MLI\n");
        // printf("LLI 1\n");
        // printf("EQI\n"); // Check if l_op * r_op = 1
        sprintf(instructionList[instructionCounter++], "MLI");
        sprintf(instructionList[instructionCounter++], "LLI 0");
        sprintf(instructionList[instructionCounter++], "NEI");
      }
      else if(exp->target->type == 1) {
        // printf("MLF\n");
        // printf("LLF 1.0\n");
        // printf("EQF\n");
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
      // if(exp->l_operand != NULL) exprgen(exp->l_operand);
      if(exp->r_operand != NULL) exprgenv2(exp->r_operand);
      // printf("exp->type %d\n", exp->type);
      if(exp->target->type == 0) {
        // Boolean not = complement
        // printf("LLI 0\n");
        // printf("NEI\n"); // If exp != 0, then exp is true
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
