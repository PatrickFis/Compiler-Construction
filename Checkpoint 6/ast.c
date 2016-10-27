#include "ast.h"
#include <stdio.h>

struct statement *list; // Extern struct declared in ast.h. Used as a linked list.
struct statement *head;

void insertStmt(struct statement *stmt) {
  if(head == NULL) {
    // Initalize head here if it has not already been done.
    return;
  }
  struct statement *next;
  next = head;
  while(next != NULL) {
    next = next->link;
  }
}
