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
#include <stdio.h>
#include <stdlib.h>
extern int yydebug;
int main()
{
   int n;
   yydebug = 0;
   n = yyparse();
   printf("\nyyparse returns %d\n", n);
   exit(0);
}
