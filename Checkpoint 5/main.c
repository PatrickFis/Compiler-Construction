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
extern int yydebug; // Can be used when bison is called with the --debug flag
struct symbol_table *table;
int main()
{
  // Initalize the symbol table
  table = malloc(sizeof(struct symbol_table));
  table->count = 0;
  table->memorySize = 0;
  int n;
  //  yydebug = 1;
  n = yyparse();
  struct symbol_table_entry y;
  for(int i = 0; i < table->count; i++) {
    y = table->table[i];
    printf("name = %s, address = %d, kind = %d, type = %d, size = %d\n", y.name, y.address, y.kind, y.type, y.size);
  }
  printf("\nyyparse returns %d\n", n);
  exit(0);
}
