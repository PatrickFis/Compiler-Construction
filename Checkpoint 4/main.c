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

main()
{
   int n;

   n = yyparse();
   printf("\nyyparse returns %d\n", n);
   exit(0);
}
