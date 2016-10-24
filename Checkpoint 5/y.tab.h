/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     RWMAIN = 258,
     RWEXIT = 259,
     RWIF = 260,
     RWELSE = 261,
     RWWHILE = 262,
     RWCOUNTING = 263,
     RWEND = 264,
     RWDATA = 265,
     RWALG = 266,
     LITINT = 267,
     LITREAL = 268,
     RWINT = 269,
     RWREAL = 270,
     RWUPWARD = 271,
     RWDOWNWARD = 272,
     RWTO = 273,
     RWREAD = 274,
     RWPRINT = 275,
     CHARSTRING = 276,
     CARRETURN = 277,
     ASSIGNOP = 278,
     COMMA = 279,
     SEMICOLON = 280,
     COLON = 281,
     LPAREN = 282,
     RPAREN = 283,
     LBRACK = 284,
     RBRACK = 285,
     ADD = 286,
     MINUS = 287,
     MULT = 288,
     DIV = 289,
     LESS = 290,
     GREATER = 291,
     LESSEQU = 292,
     GREATEQU = 293,
     EQUAL = 294,
     NOTEQUAL = 295,
     AND = 296,
     OR = 297,
     NOT = 298,
     VAR = 299,
     VARIABLE = 300
   };
#endif
/* Tokens.  */
#define RWMAIN 258
#define RWEXIT 259
#define RWIF 260
#define RWELSE 261
#define RWWHILE 262
#define RWCOUNTING 263
#define RWEND 264
#define RWDATA 265
#define RWALG 266
#define LITINT 267
#define LITREAL 268
#define RWINT 269
#define RWREAL 270
#define RWUPWARD 271
#define RWDOWNWARD 272
#define RWTO 273
#define RWREAD 274
#define RWPRINT 275
#define CHARSTRING 276
#define CARRETURN 277
#define ASSIGNOP 278
#define COMMA 279
#define SEMICOLON 280
#define COLON 281
#define LPAREN 282
#define RPAREN 283
#define LBRACK 284
#define RBRACK 285
#define ADD 286
#define MINUS 287
#define MULT 288
#define DIV 289
#define LESS 290
#define GREATER 291
#define LESSEQU 292
#define GREATEQU 293
#define EQUAL 294
#define NOTEQUAL 295
#define AND 296
#define OR 297
#define NOT 298
#define VAR 299
#define VARIABLE 300




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 18 "parse.y"
{
   char *sval;
   struct symbol_table *tableptr;
   struct symbol_table_entry *entry;
}
/* Line 1529 of yacc.c.  */
#line 145 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

