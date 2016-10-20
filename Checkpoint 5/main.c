/*
 * ========================================================================
 *
 * main.c ------- Calls yyparse. Change yydebug to 1 to debug parse.y.
 *
 * Programmer --- Patrick Fischer
 * Due ---------- 10/13/2016
 * ========================================================================
 */

#include "y.tab.h"
#include "stable.h"
#include <stdio.h>
#include <stdlib.h>
extern int yydebug;
struct symbol_table *table;
int main()
{
  table = malloc(sizeof(struct symbol_table));
  table->count = 0;
  struct symbol_table_entry x;
  x = createEntry("name", 0, 0, 0, 4);
  printf("name = %s, address = %d, kind = %d, type = %d, size = %d\n", x.name, x.address, x.kind, x.type, x.size);
  insert(x);
  //  int n;
  //  yydebug = 0;
  //  n = yyparse();
  //  printf("\nyyparse returns %d\n", n);
  //  exit(0);
}
