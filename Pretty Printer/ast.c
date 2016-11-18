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

void codeGenIfv2(struct statement *next, int nested);
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

  printf("main;\n"); // Start of program.
  // Print the symbol table:
  int i;
  // Four spaces for data section.
  printf("    data:\n");
  for(i = 0; i < table->count; i++) {
    // Four more spaces for each variable reference.
    if(table->table[i].type == TYPE_INT) {
      printf("        integer: %s;\n", table->table[i].name);
    }
    else if(table->table[i].type == TYPE_REAL) {
      printf("        real: %s;\n", table->table[i].name);
    }
  }

  // Four spaces for the algorithm section.
  printf("    algorithm:\n");
  while(next->link != NULL) {
    // printf("Calling exprgen\n"); // Debug
    // Section that generates code for if statements
    if(next->isCond == 1) {
      codeGenIfv2(next, 2);
      if(next->isIfElse == 1) {
        next = next->link;
        // printf("Found an if else...\n");
        goto label;
      }
    next = next->link;
    continue;
  }
  if(next->isIfElse == 1) {
    // printf("isIfElse = 1\n");
  }
  // End if section

  // Section that generates code for while statements
  if(next->isWhile == 1) {
    // printf("Hi\n");
    next = next->link;
    continue;
  }

  int iBefore = instructionCounter;
  label: exprgen(next->exp);
  int iAfter = instructionCounter;
  // printf("iB = %d, iA = %d\n", iBefore, iAfter);
  // printf("exprgen finished\n"); // Debug
  // checkInstructions(iBefore, iAfter);
  // label: // REMOVE THE GOTO
  next = next->link;
  }
  printf("end main;\n");
  sprintf(instructionList[instructionCounter++], "HLT");
  // for(int i = 0; i < instructionCounter; i++) {
  //   printf("%s\n", instructionList[i]);
  // }
}


// So this actually finds nested if statements, but if there's an else block it
// skips over the nested statements. So the problem is from assigning $$->body->link
// in the parser. This will be overwritten by the programbody portion of rules.
void codeGenIfv2(struct statement *next, int nested) {
  struct ast_if_stmt *nextCopy = malloc(sizeof(struct ast_if_stmt));
  nextCopy = next->if_stmt;
  // printf("        ");
  char *ifSpace = malloc(sizeof(4*nested));
  int i;
  for(i = 0; i < 4*nested; i++) {
    strcat(ifSpace, " ");
  }
  printf("%s", ifSpace);
  printf("if");
  // Parse the conditional statement
  exprgen(nextCopy->conditional_stmt);
  printf("\b;\n");
  int jumpLocation = instructionCounter;
  sprintf(instructionList[instructionCounter++], "JPF"); // Replace this
  // Need a structure to parse the body statement
  struct statement *bodyCopy = malloc(sizeof(struct statement));
  bodyCopy = nextCopy->body;
  while(bodyCopy->link != NULL) {
    // Parse this like a regular statement!
    // printf("Hi from bodyCopy\n");
    char *spaces = malloc(sizeof(char));
    // int i;
    for(i = 0; i < 4*nested; i++) {
      strcat(spaces, " ");
    }
    printf("%s", spaces);
    if(bodyCopy->isCond == 1) {
      // printf("Hi from bodyCopy!\n");
      // nested++;
      codeGenIfv2(bodyCopy, nested);
      nested++;
      bodyCopy = bodyCopy->link;
      continue;
    }
    // printf("        ");
    exprgen(bodyCopy->exp);
    bodyCopy = bodyCopy->link;
  }
  // printf("%s", ifSpace);
  for(i = 0; i < 4*nested; i++) {
    ifSpace[i] = ' ';
  }
  ifSpace[4*nested] = '\0';
  printf("%s", ifSpace);
  printf("end if;\n");
  if(nextCopy->tempLink != NULL) {
    printf("%s", ifSpace);
    printf("else;\n");
    struct statement *tempCopy = nextCopy->tempLink;

    while(tempCopy->link != NULL) {
      printf("%s", ifSpace);
      exprgen(tempCopy->exp);
      tempCopy = tempCopy->link;
      if(tempCopy->isCond == 1) {
        nested++;
        codeGenIfv2(tempCopy, nested);
        tempCopy = tempCopy->link;
        continue;
      }
      nested--;
    }
    for(i = 0; i < strlen(ifSpace); i++) {
      ifSpace[i] = ' ';
    }
    printf("%s", ifSpace);
    printf("end if;\n");
  }
  // printf("        end if;\n");
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

/*
 *  Parses print statements and prints out instructions. Currently it does some
 *  backtracking to correct instructions, but more checks will be needed in the future.
 */
void parsePrintv3(struct ast_expression *exp) {
  struct ast_expression *x = exp->r_operand;
  // if(x->charString == NULL) printf("NULL\n");
  // char *giantString = malloc(sizeof(char));
  if(x != NULL) {
    if(x->charString != NULL) {
      // printf("%s\n", x->charString);
      printf("%s ", x->charString);
      if(strcmp(x->charString, "!,") != 0 && strcmp(x->charString, "!") != 0) printf("\b, ");
      // strcat(buffer, " ");

      // sprintf(buffer, "%s", x->charString);
      int i;
      // Print a newline character
      if(strcmp(x->charString, "!,") == 0 || strcmp(x->charString, "!") == 0) {
        if(strcmp(x->charString, "!,") == 0) {
          // printf("!,");
        }
        else if(strcmp(x->charString, "!") == 0) {
          // printf("! ");
        }
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

    /*
    *  Tree is structed like this... exp->r_operand->r_operand->r_operand...
    *  If r_operand has a charString, then it should be printed.
    *  If the charString is NULL, then r_operand has a non-NULL l_operand.
    *  l_operand is a bexp, which should be passed to exprgen for code generation.
    */
    if(x->charString == NULL) {
      x->l_operand->target = &table->table[0];
      int iCounterBefore = instructionCounter;
      exprgen(x->l_operand);
      if(x->r_operand != NULL) printf("\b, "); // Places a comma after an expression
      else printf("\b"); // Gets rid of space after expression
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
    }
    parsePrintv3(x);
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
    printf("        print ");
    // Why not just put each part of the string into a character array and use
    // strcat to put them together? A check could happen after the function
    // call to fix any comma issues.
    // Looks like I found a different way to fix the issue.
    parsePrintv3(exp);
    printf(";\n");
    return;
  }
  if(exp->operator == OP_READ) {
    printf("        read %s;\n", table->table[exp->address].name);
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
    printf("%d ", exp->value);
    sprintf(instructionList[instructionCounter++], "LLI %d", exp->value);
  }
  else if(exp->kind == KIND_REAL && exp->type != TYPE_VAR) { // If expression involves reals
    if(DEBUG) printf("Got to load real\n");
    // printf("LLF %f\n", exp->rvalue);

    // Print the number plus a space.
    printf("%f ", exp->rvalue);
    sprintf(instructionList[instructionCounter++], "LLF %f", exp->rvalue);
  }
  if(exp->type == TYPE_VAR) {
    if(DEBUG) printf("Got to variable type\n");
    // printf("LAA %d\n",exp->l_operand->target->address);
    // printf("LOD\n");
    printf("%s ", exp->l_operand->target->name);
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

    // 8 Spaces for assignment statements
    printf("        %s := ", exp->target->name);
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

      // Remove the last space from an assignment statement, and then append a
      // semicolon to it. Then print a newline character.
      printf("\b;\n");
    }
    break;

    case OP_UMIN:
    printf("( -");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
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

    // Wrap this expression in parentheses. Space added so the \b doesn't mess
    // up the parentheses.
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    // Print a plus sign and a space.
    printf("+ ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    // Wrap this expression in parentheses. Space added so the \b doesn't mess
    // up the parentheses.
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "ADI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "ADF");
    }
    break;

    case OP_SUB:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("- ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "SBI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "SBF");
    }
    break;

    case OP_MUL:
    if(DEBUG) printf("Got to OP_MUL\n");
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("* ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "MLI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "MLF");
    }
    break;

    case OP_DIV:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("/ ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "DVI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "DVF");
    }
    break;

    case OP_LSTHN:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("< ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "LTI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "LTF");
    }
    break;

    case OP_LSTHNEQL:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("<= ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "LEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "LEF");
    }
    break;

    case OP_GRTHN:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("> ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "GTI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "GTF");
    }
    break;

    case OP_GRTHNEQL:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf(">= ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "GEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "GEI");
    }
    break;

    case OP_EQUAL:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("= ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "EQI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "EQF");
    }
    break;

    case OP_NEQUAL:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("<> ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      sprintf(instructionList[instructionCounter++], "NEI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "NEF");
    }
    break;

    case OP_AND: // These Boolean instructions require a bit of clever faking
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("& ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      // Boolean and = multiplication
      // printf("MLI\n");
      // printf("LLI 1\n");
      // printf("EQI\n"); // Check if l_op * r_op = 1
      sprintf(instructionList[instructionCounter++], "MLI");
      sprintf(instructionList[instructionCounter++], "LLI 1");
      sprintf(instructionList[instructionCounter++], "EQI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      // printf("MLF\n");
      // printf("LLF 1.0\n");
      // printf("EQF\n");
      sprintf(instructionList[instructionCounter++], "MLF");
      sprintf(instructionList[instructionCounter++], "LLF 1.0");
      sprintf(instructionList[instructionCounter++], "EQF");
    }
    break;

    case OP_OR:
    printf("( ");
    if(exp->l_operand != NULL) exprgen(exp->l_operand);
    printf("| ");
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
    if(getPreviousInstructionType(instructionCounter) == 0) {
      // Boolean or = addition
      sprintf(instructionList[instructionCounter++], "ADI");
      sprintf(instructionList[instructionCounter++], "LLI 1");
      sprintf(instructionList[instructionCounter++], "EQI");
    }
    else if(getPreviousInstructionType(instructionCounter) == 1) {
      sprintf(instructionList[instructionCounter++], "ADF");
      sprintf(instructionList[instructionCounter++], "LLF 1.0");
      sprintf(instructionList[instructionCounter++], "EQF");
    }
    break;

    case OP_NOT:
    printf("( ~");
    // if(exp->l_operand != NULL) exprgen(exp->l_operand);
    if(exp->r_operand != NULL) exprgen(exp->r_operand);
    printf(") ");
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

struct ast_expression createExp(char kind, char operator, int value) {
  struct ast_expression expr;
  expr.kind = kind;
  expr.operator = operator;
  expr.value = value;
  return expr;
}
