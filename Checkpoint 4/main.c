/*
 * ========================================================================
 *
 * main.c ------- In-class example of a main program file to be used in
 *                conjunction with a bison parser.
 *
 * Programmer --- Bryan Crawley
 *
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
