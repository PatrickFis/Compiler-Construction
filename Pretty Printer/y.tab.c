/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "parse.y"


/*
 * ========================================================================
 *
 * parse.y - Bison parser for Slic.
 * Fixed my symbol table code to allow for insertion of a comma separated list.
 * Added abstract syntax tree and code generation in ast.h and ast.c.
 *
 * Add print statements, if statements, and if/else statements. Also implement
 * code to parse array references.
 *
 * Programmer --- Patrick Fischer
 *
 * ========================================================================
 */

#include "stable.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>
#define DEBUG 0
struct symbol_table *table;
struct statement *list;
int entry_count = 0; // Used to keep track of what symbols are being inserted.
// Variable type will be changed based on entry_count.


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 29 "parse.y"
{
   char *sval;
   int ival;
   double rval;
   struct symbol_table *tableptr;
   struct symbol_table_entry *entry;
   struct statement *stmt;
   struct ast_expression *expr;
   struct ast_if_stmt *ifstmt;
   struct ast_while_stmt *while_loop;
}
/* Line 193 of yacc.c.  */
#line 226 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 239 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   154

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNRULES -- Number of states.  */
#define YYNSTATES  135

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     7,    10,    13,    17,    20,    22,    26,
      30,    34,    37,    39,    44,    49,    52,    54,    57,    59,
      62,    64,    67,    69,    72,    74,    79,    87,    89,    93,
      97,   100,   102,   106,   110,   114,   118,   122,   126,   128,
     132,   136,   138,   142,   146,   148,   151,   153,   155,   157,
     161,   163,   168,   171,   174,   178,   181,   185,   189,   192,
     194,   198,   206,   217,   225
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    48,    49,    54,    -1,     3,    25,    -1,
      10,    26,    -1,    10,    26,    50,    -1,    50,    51,    -1,
      51,    -1,    14,    26,    52,    -1,    15,    26,    52,    -1,
      53,    24,    52,    -1,    53,    25,    -1,    44,    -1,    44,
      29,    12,    30,    -1,    11,    26,    55,    69,    -1,    56,
      55,    -1,    56,    -1,    67,    55,    -1,    67,    -1,    64,
      55,    -1,    64,    -1,    66,    55,    -1,    66,    -1,    68,
      55,    -1,    68,    -1,    44,    57,    58,    25,    -1,    44,
      29,    12,    30,    57,    58,    25,    -1,    23,    -1,    58,
      41,    59,    -1,    58,    42,    59,    -1,    43,    59,    -1,
      59,    -1,    59,    35,    60,    -1,    59,    37,    60,    -1,
      59,    36,    60,    -1,    59,    38,    60,    -1,    59,    39,
      60,    -1,    59,    40,    60,    -1,    60,    -1,    60,    31,
      61,    -1,    60,    32,    61,    -1,    61,    -1,    61,    33,
      62,    -1,    61,    34,    62,    -1,    62,    -1,    32,    63,
      -1,    63,    -1,    12,    -1,    13,    -1,    27,    58,    28,
      -1,    44,    -1,    44,    29,    12,    30,    -1,    20,    65,
      -1,    21,    65,    -1,    21,    24,    65,    -1,    22,    65,
      -1,    22,    24,    65,    -1,    58,    24,    65,    -1,    58,
      25,    -1,    25,    -1,    19,    53,    25,    -1,     5,    58,
      25,    55,     9,     5,    25,    -1,     5,    58,    25,    55,
       6,    25,    55,     9,     5,    25,    -1,     7,    58,    25,
      55,     9,     7,    25,    -1,     9,     3,    25,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   103,   103,   105,   107,   108,   111,   112,   115,   132,
     149,   167,   180,   187,   197,   201,   207,   217,   239,   263,
     269,   279,   285,   295,   303,   315,   329,   342,   344,   352,
     360,   367,   373,   381,   389,   397,   405,   413,   422,   431,
     439,   447,   457,   465,   473,   482,   489,   496,   505,   514,
     519,   540,   569,   594,   602,   613,   620,   627,   639,   656,
     662,   703,   710,   724,   731
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "RWMAIN", "RWEXIT", "RWIF", "RWELSE",
  "RWWHILE", "RWCOUNTING", "RWEND", "RWDATA", "RWALG", "LITINT", "LITREAL",
  "RWINT", "RWREAL", "RWUPWARD", "RWDOWNWARD", "RWTO", "RWREAD", "RWPRINT",
  "CHARSTRING", "CARRETURN", "ASSIGNOP", "COMMA", "SEMICOLON", "COLON",
  "LPAREN", "RPAREN", "LBRACK", "RBRACK", "ADD", "MINUS", "MULT", "DIV",
  "LESS", "GREATER", "LESSEQU", "GREATEQU", "EQUAL", "NOTEQUAL", "AND",
  "OR", "NOT", "VAR", "VARIABLE", "$accept", "program", "headingstmt",
  "datasection", "decstmtlist", "decstmt", "varlist", "varref",
  "algsection", "programbody", "assignstmt", "assign", "bexp", "rexp",
  "exp", "term", "factor", "unit", "outputstmt", "printlist", "inputstmt",
  "controlstmt", "whilestmt", "endmainstmt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    48,    49,    49,    50,    50,    51,    51,
      52,    52,    53,    53,    54,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    56,    56,    57,    58,    58,
      58,    58,    59,    59,    59,    59,    59,    59,    59,    60,
      60,    60,    61,    61,    61,    62,    62,    63,    63,    63,
      63,    63,    64,    65,    65,    65,    65,    65,    65,    65,
      66,    67,    67,    68,    69
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     2,     2,     3,     2,     1,     3,     3,
       3,     2,     1,     4,     4,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     4,     7,     1,     3,     3,
       2,     1,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     2,     1,     1,     1,     3,
       1,     4,     2,     2,     3,     2,     3,     3,     2,     1,
       3,     7,    10,     7,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     3,     1,     0,     0,     4,     0,
       2,     0,     0,     5,     7,     0,     0,     0,     6,     0,
       0,     0,     0,     0,     0,    16,    20,    22,    18,    24,
      12,     8,     0,     9,    47,    48,     0,     0,     0,    50,
       0,    31,    38,    41,    44,    46,     0,     0,     0,     0,
      59,     0,    52,    27,     0,     0,     0,    14,    15,    19,
      21,    17,    23,     0,     0,    11,     0,    45,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,     0,    53,     0,    55,     0,
      58,     0,     0,     0,     0,    10,    49,     0,     0,    28,
      29,    32,    34,    33,    35,    36,    37,    39,    40,    42,
      43,     0,    54,    56,    57,     0,    25,    64,    13,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      63,    26,     0,     0,    62
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     7,    13,    14,    31,    32,    10,    24,
      25,    55,    51,    41,    42,    43,    44,    45,    26,    52,
      27,    28,    29,    57
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -40
static const yytype_int16 yypact[] =
{
      10,     4,    57,    49,   -40,   -40,    35,    53,     5,    47,
     -40,    62,    71,     5,   -40,     7,    34,    34,   -40,    73,
      73,    34,    81,    -1,    72,     7,     7,     7,     7,     7,
      78,   -40,    46,   -40,   -40,   -40,    73,    18,    83,    80,
      -7,    93,    52,    56,   -40,   -40,    -4,    89,    31,    55,
     -40,    -9,   -40,   -40,   114,    73,   131,   -40,   -40,   -40,
     -40,   -40,   -40,   123,    34,   -40,   -17,   -40,    93,   124,
       7,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,     7,   -40,    81,   -40,    81,   -40,    81,
     -40,   107,    -2,   113,   109,   -40,   -40,   110,    63,    93,
      93,    52,    52,    52,    52,    52,    52,    56,    56,   -40,
     -40,   132,   -40,   -40,   -40,   119,   -40,   -40,   -40,   -40,
     118,   139,   138,    73,     7,   121,   122,    24,   140,   -40,
     -40,   -40,   143,   125,   -40
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -40,   -40,   -40,   -40,   -40,   141,   -10,   130,   -40,   -23,
     -40,    37,   -19,   -30,    45,    12,    30,   116,   -40,   -39,
     -40,   -40,   -40,   -40
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      40,    46,    58,    59,    60,    61,    62,    33,    68,    86,
      88,    96,    19,     1,    20,    89,    90,    66,    70,    11,
      12,    83,    53,   116,    71,    72,    21,    22,    54,     4,
      34,    35,    71,    72,    71,    72,    92,    71,    72,    71,
      72,    99,   100,    34,    35,    36,   112,    98,   113,   131,
     114,    23,    48,    49,    95,    85,    50,     5,    36,     6,
     111,     8,    39,    37,     9,    71,    72,    34,    35,   120,
      64,    65,   121,    15,    38,    39,    48,    49,    30,    87,
      50,    56,    36,    79,    80,    34,    35,    37,    16,    81,
      82,   107,   108,    34,    35,    34,    35,    17,    38,    39,
      36,   128,    48,    49,   127,    37,    50,    63,    36,    69,
      36,   109,   110,    37,    84,    37,    38,    39,   101,   102,
     103,   104,   105,   106,    38,    39,    91,    39,    73,    74,
      75,    76,    77,    78,    93,    94,    97,   115,   117,   118,
     119,   122,    53,   124,   125,   126,   129,   130,   133,   132,
     134,    47,   123,    67,    18
};

static const yytype_uint8 yycheck[] =
{
      19,    20,    25,    26,    27,    28,    29,    17,    38,    48,
      49,    28,     5,     3,     7,    24,    25,    36,    25,    14,
      15,    25,    23,    25,    41,    42,    19,    20,    29,    25,
      12,    13,    41,    42,    41,    42,    55,    41,    42,    41,
      42,    71,    72,    12,    13,    27,    85,    70,    87,    25,
      89,    44,    21,    22,    64,    24,    25,     0,    27,    10,
      83,    26,    44,    32,    11,    41,    42,    12,    13,     6,
      24,    25,     9,    26,    43,    44,    21,    22,    44,    24,
      25,     9,    27,    31,    32,    12,    13,    32,    26,    33,
      34,    79,    80,    12,    13,    12,    13,    26,    43,    44,
      27,   124,    21,    22,   123,    32,    25,    29,    27,    29,
      27,    81,    82,    32,    25,    32,    43,    44,    73,    74,
      75,    76,    77,    78,    43,    44,    12,    44,    35,    36,
      37,    38,    39,    40,     3,    12,    12,    30,    25,    30,
      30,     9,    23,    25,     5,     7,    25,    25,     5,     9,
      25,    21,   115,    37,    13
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    47,    48,    25,     0,    10,    49,    26,    11,
      54,    14,    15,    50,    51,    26,    26,    26,    51,     5,
       7,    19,    20,    44,    55,    56,    64,    66,    67,    68,
      44,    52,    53,    52,    12,    13,    27,    32,    43,    44,
      58,    59,    60,    61,    62,    63,    58,    53,    21,    22,
      25,    58,    65,    23,    29,    57,     9,    69,    55,    55,
      55,    55,    55,    29,    24,    25,    58,    63,    59,    29,
      25,    41,    42,    35,    36,    37,    38,    39,    40,    31,
      32,    33,    34,    25,    25,    24,    65,    24,    65,    24,
      25,    12,    58,     3,    12,    52,    28,    12,    55,    59,
      59,    60,    60,    60,    60,    60,    60,    61,    61,    62,
      62,    55,    65,    65,    65,    30,    25,    25,    30,    30,
       6,     9,     9,    57,    25,     5,     7,    58,    55,    25,
      25,    25,     9,     5,    25
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 8:
#line 115 "parse.y"
    {  //$$ = malloc(sizeof(struct symbol_table_entry));
                                  (yyvsp[(3) - (3)].entry) = malloc(sizeof(struct symbol_table_entry));
                                  (yyvsp[(3) - (3)].entry)->type = TYPE_INT;
                                  if(DEBUG) printf("$3->type: %d\n",(yyvsp[(3) - (3)].entry)->type);
                                  int i;
                                  for(i = table->count - entry_count; i < table->count; i++) {
                                    table->table[i].type = TYPE_INT;
                                  }
                                  entry_count = 0;
                                  if(DEBUG) printf("Finished integer varlist\n");
                                //$$->type=TYPE_INT;
                                //$$->name=$3->name;
                                //$$->kind = $3->kind;
                                //$$->address = $3->address;
                                //$$->size = $3->size;
                                //insert(*$$);
                                }
    break;

  case 9:
#line 132 "parse.y"
    { (yyvsp[(3) - (3)].entry) = malloc(sizeof(struct symbol_table_entry));
                                (yyvsp[(3) - (3)].entry)->type=TYPE_REAL;
                                if(DEBUG) printf("$3->type: %d\n",(yyvsp[(3) - (3)].entry)->type);
                                int i;
                                for(i = table->count - entry_count; i < table->count; i++) {
                                  table->table[i].type = TYPE_REAL;
                                }
                                entry_count = 0;
                                if(DEBUG) printf("Finished real varlist\n");
                                //$$->name=$3->name;
                                //$$->kind = $3->kind;
                                //$$->address = $3->address;
                                //$$->size = $3->size;
                                //insert(*$$);
                                }
    break;

  case 10:
#line 149 "parse.y"
    {
                                (yyval.entry)->name = (yyvsp[(1) - (3)].entry)->name;
                                (yyval.entry)->kind = (yyvsp[(1) - (3)].entry)->kind;
                                (yyvsp[(1) - (3)].entry)->type = (yyval.entry)->type;
                                // $1->type = 0; // So without this line of code
                                // // my program will seg fault on OS X, but not
                                // // Windows or Linux. It produces the correct
                                // // output without this being set correctly.
                                // // I have a feeling the output might not be fully
                                // // correct for later expressions, as this relies
                                // // on the parser to recognize real and integer
                                // // values for later instructions.
                                (yyval.entry)->address = 0;
                                (yyval.entry)->size = (yyvsp[(1) - (3)].entry)->size;
                                insert(*(yyval.entry));
                                entry_count++;
                                if(DEBUG) printf("$$->type: %d\n", (yyval.entry)->type);
                              }
    break;

  case 11:
#line 167 "parse.y"
    {
          (yyval.entry)->name = (yyvsp[(1) - (2)].entry)->name;
          (yyval.entry)->kind = (yyvsp[(1) - (2)].entry)->kind;
          (yyvsp[(1) - (2)].entry)->type = (yyval.entry)->type;
          // $1->type = 0; // Debug
          (yyval.entry)->address = 0;
          (yyval.entry)->size = (yyvsp[(1) - (2)].entry)->size;
          insert(*(yyval.entry));
          entry_count++;
          if(DEBUG) printf("$$->type: %d\n", (yyval.entry)->type);
        }
    break;

  case 12:
#line 180 "parse.y"
    { (yyval.entry) = malloc(sizeof(struct symbol_table_entry));
              (yyval.entry)->name = (yyvsp[(1) - (1)].sval);
              if(DEBUG) printf("$1 = %s\n", (yyvsp[(1) - (1)].sval));
              (yyval.entry)->address = 0;
              (yyval.entry)->kind = KIND_SCALAR;
              (yyval.entry)->size = 1;
              }
    break;

  case 13:
#line 187 "parse.y"
    {
                                 if(DEBUG) printf("Got to array declaration\n");
                                 (yyval.entry) = malloc(sizeof(struct symbol_table_entry));
                                 (yyval.entry)->name = (yyvsp[(1) - (4)].sval);
                                 (yyval.entry)->address = 0;
                                 (yyval.entry)->kind = KIND_ARRAY;
                                 (yyval.entry)->size = (yyvsp[(3) - (4)].ival);
                                 }
    break;

  case 14:
#line 197 "parse.y"
    {
            list = (yyvsp[(3) - (4)].stmt);
            }
    break;

  case 15:
#line 201 "parse.y"
    { // Multiple assignments, removed endmainstmt from programbody to stop a segfault
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->exp = (yyvsp[(1) - (2)].expr);
              (yyval.stmt)->link = (yyvsp[(2) - (2)].stmt);
              //insertStmt($$);
             }
    break;

  case 16:
#line 207 "parse.y"
    { // Only one assignment
              // printf("Only one assignment");
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->exp = (yyvsp[(1) - (1)].expr);
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->exp = (yyval.stmt)->exp;
              temp->link = NULL;
              (yyval.stmt)->link = temp;
            }
    break;

  case 17:
#line 217 "parse.y"
    { // If/else statements
              // Note that if else statements will be parsed using this rule...
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->if_stmt = (yyvsp[(1) - (2)].ifstmt);
              // if($2->link != NULL) {
              //   $$->link = $2->link;
              //   printf("MADE IT HERE\n");
              // }
              // else {
              //   $$->link = $2;
              // }
              (yyval.stmt)->link = (yyvsp[(2) - (2)].stmt);
              (yyval.stmt)->isCond = 1;
              // if($$->if_stmt->tempLink != NULL) {
              //   $$->link->link = $$->if_stmt->tempLink;
              // }
              if((yyvsp[(1) - (2)].ifstmt)->isIfElse == 1) {
                (yyval.stmt)->isIfElse = 1;
              }
              // printf("Controlstmt\n");
              if(DEBUG) printf("Got to controlstmt programbody\n");
            }
    break;

  case 18:
#line 239 "parse.y"
    {
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->if_stmt = (yyvsp[(1) - (1)].ifstmt);
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->if_stmt = (yyval.stmt)->if_stmt;
              // if($1->body->link != NULL) {
              //   printf("Got here temp link\n");
              //   temp->link = $1->body->link;
              // }
              // else {
              //   temp->link = NULL;
              // }
              // temp->link = NULL;
              // if($$->if_stmt->tempLink != NULL) {
              //   temp->link->link = $$->if_stmt->tempLink;
              // }
              (yyval.stmt)->link = temp;
              (yyval.stmt)->isCond = 1;
              if((yyvsp[(1) - (1)].ifstmt)->isIfElse == 1) {
                (yyval.stmt)->isIfElse = 1;
              }
              if(DEBUG) printf("Got to controlstmt\n");
            }
    break;

  case 19:
#line 263 "parse.y"
    {
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->exp = (yyvsp[(1) - (2)].expr);
              (yyval.stmt)->link = (yyvsp[(2) - (2)].stmt);
              if(DEBUG) printf("Got to outputstmt programbody\n");
            }
    break;

  case 20:
#line 269 "parse.y"
    {
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->exp = (yyvsp[(1) - (1)].expr);
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->exp = (yyval.stmt)->exp;
              temp->link = NULL;
              (yyval.stmt)->link = temp;
              if(DEBUG) printf("Got to outputstmt\n");
            }
    break;

  case 21:
#line 279 "parse.y"
    {
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->exp = (yyvsp[(1) - (2)].expr);
              (yyval.stmt)->link = (yyvsp[(2) - (2)].stmt);
              if(DEBUG) printf("Got to inputstmt programbody\n");
            }
    break;

  case 22:
#line 285 "parse.y"
    {
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->exp = (yyvsp[(1) - (1)].expr);
              // Added the temp struct to insert the last expressions into the linked list.
              struct statement *temp = malloc(sizeof(struct statement));
              temp->exp = (yyval.stmt)->exp;
              temp->link = NULL;
              (yyval.stmt)->link = temp;
              if(DEBUG) printf("Got to inputstmt\n");
            }
    break;

  case 23:
#line 295 "parse.y"
    {
              if(DEBUG) printf("Got to whilestmt programbody\n");
              (yyval.stmt) = malloc(sizeof(struct statement));
              (yyval.stmt)->while_stmt = (yyvsp[(1) - (2)].while_loop);
              (yyval.stmt)->link = (yyvsp[(2) - (2)].stmt);
              (yyval.stmt)->isWhile = 1;
              if(DEBUG) printf("Finished whilestmt programbody\n");
            }
    break;

  case 24:
#line 303 "parse.y"
    {
              if(DEBUG) printf("Got to whilestmt \n");
              (yyval.stmt) = malloc(sizeof(struct statement));
              struct statement *temp = malloc(sizeof(struct statement));
              temp->while_stmt = (yyval.stmt)->while_stmt;
              temp->link = NULL;
              (yyval.stmt)->link = temp;
              (yyval.stmt)->isWhile = 1;
              if(DEBUG) printf("Finished whilestmt\n");
            }
    break;

  case 25:
#line 316 "parse.y"
    {
              // This appears to to be working
              if(DEBUG) printf("Got to assignstmt\n");
              (yyval.expr) = malloc(sizeof(struct ast_expression));
              (yyval.expr)->kind = KIND_OP;
              (yyval.expr)->operator = OP_ASGN;
              // $$->l_operand = NULL; // Gonna set this to NULL and use r_operand for the exp
              (yyval.expr)->r_operand = (yyvsp[(3) - (4)].expr);
              (yyval.expr)->target = &table->table[isPresent((yyvsp[(1) - (4)].sval))]; // Get target from symbol table
              (yyval.expr)->address = (yyval.expr)->target->address;
              if(DEBUG) printf("Address %d\n", (yyval.expr)->address);
              (yyval.expr)->arrayOffset = 0;
            }
    break;

  case 26:
#line 330 "parse.y"
    {
              if(DEBUG) printf("Got to array assignment\n");
              (yyval.expr) = malloc(sizeof(struct ast_expression));
              (yyval.expr)->kind = KIND_OP;
              (yyval.expr)->operator = OP_ASGN;
              (yyval.expr)->target = &table->table[isPresent((yyvsp[(1) - (7)].sval))];
              (yyval.expr)->address = (yyval.expr)->target->address + (yyvsp[(3) - (7)].ival);
              (yyval.expr)->r_operand = (yyvsp[(6) - (7)].expr);
              if(DEBUG) printf("Address: %d\n", (yyval.expr)->address);
              (yyval.expr)->arrayOffset = (yyval.expr)->address;
            }
    break;

  case 28:
#line 344 "parse.y"
    { // Code to parse & booleans, bexp = Boolean Expression
                    if(DEBUG) printf("Got to AND\n");
                    (yyval.expr) = malloc(sizeof(struct ast_expression));
                    (yyval.expr)->kind = KIND_OP;
                    (yyval.expr)->operator = OP_AND;
                    (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                    (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                  }
    break;

  case 29:
#line 352 "parse.y"
    { // Code to parse | booleans
                  if(DEBUG) printf("Got to OR\n");
                  (yyval.expr) = malloc(sizeof(struct ast_expression));
                  (yyval.expr)->kind = KIND_OP;
                  (yyval.expr)->operator = OP_OR;
                  (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                  (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                 }
    break;

  case 30:
#line 360 "parse.y"
    { // Code to parse ~ booleans.
        if(DEBUG) printf("Got to NOT\n");
        (yyval.expr) = malloc(sizeof(struct ast_expression));
        (yyval.expr)->kind = KIND_OP;
        (yyval.expr)->operator = OP_NOT;
        (yyval.expr)->r_operand = (yyvsp[(2) - (2)].expr);
      }
    break;

  case 31:
#line 367 "parse.y"
    {
        (yyval.expr) = (yyvsp[(1) - (1)].expr);
        // $$->type = $1->type;
      }
    break;

  case 32:
#line 373 "parse.y"
    { // Code to parse < booleans, rexp = Relation Expression
                      if(DEBUG) printf("Got to LESS\n");
                      (yyval.expr) = malloc(sizeof(struct ast_expression));
                      (yyval.expr)->kind = KIND_OP;
                      (yyval.expr)->operator = OP_LSTHN;
                      (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                      (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                   }
    break;

  case 33:
#line 381 "parse.y"
    { // Code to parse <= booleans
                        if(DEBUG) printf("Got to LESSTHN\n");
                        (yyval.expr) = malloc(sizeof(struct ast_expression));
                        (yyval.expr)->kind = KIND_OP;
                        (yyval.expr)->operator = OP_LSTHNEQL;
                        (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                        (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                      }
    break;

  case 34:
#line 389 "parse.y"
    { // Code to parse > booleans
                        if(DEBUG) printf("Got to GREATER\n");
                        (yyval.expr) = malloc(sizeof(struct ast_expression));
                        (yyval.expr)->kind = KIND_OP;
                        (yyval.expr)->operator = OP_GRTHN;
                        (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                        (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                      }
    break;

  case 35:
#line 397 "parse.y"
    { // Code to parse >= booleans
                        if(DEBUG) printf("Got to GREATEQU\n");
                        (yyval.expr) = malloc(sizeof(struct ast_expression));
                        (yyval.expr)->kind = KIND_OP;
                        (yyval.expr)->operator = OP_GRTHNEQL;
                        (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                        (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                       }
    break;

  case 36:
#line 405 "parse.y"
    { // Code to parse = booleans
                      if(DEBUG) printf("Got to EQUAL\n");
                      (yyval.expr) = malloc(sizeof(struct ast_expression));
                      (yyval.expr)->kind = KIND_OP;
                      (yyval.expr)->operator = OP_EQUAL;
                      (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                      (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                    }
    break;

  case 37:
#line 413 "parse.y"
    { // Code to parse <> booleans
                        if(DEBUG) printf("Got to NOTEQUAL\n");
                        (yyval.expr) = malloc(sizeof(struct ast_expression));
                        (yyval.expr)->kind = KIND_OP;
                        (yyval.expr)->operator = OP_NEQUAL;
                        (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                        (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                       }
    break;

  case 38:
#line 422 "parse.y"
    {
      if(DEBUG) printf("Got to exp\n");
      //$$ = malloc(sizeof(struct ast_expression));
      //$$->kind = KIND_OP;
      //$$->l_operand = $1;
      (yyval.expr) = (yyvsp[(1) - (1)].expr);
      // $$->type = $1->type;
    }
    break;

  case 39:
#line 431 "parse.y"
    {// Code to parse expressions
                    if(DEBUG) printf("Got to add\n");
                    (yyval.expr) = malloc(sizeof(struct ast_expression));
                    (yyval.expr)->kind = KIND_OP;
                    (yyval.expr)->operator = OP_ADD;
                    (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                    (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                  }
    break;

  case 40:
#line 439 "parse.y"
    {
                      if(DEBUG) printf("Got to subtract\n");
                      (yyval.expr) = malloc(sizeof(struct ast_expression));
                      (yyval.expr)->kind = KIND_OP;
                      (yyval.expr)->operator = OP_SUB;
                      (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                      (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                    }
    break;

  case 41:
#line 447 "parse.y"
    {
            if(DEBUG) printf("Got to term\n");
            //$$ = malloc(sizeof(struct ast_expression));
            //$$->kind = KIND_OP;
            //$$->l_operand = $1;
            (yyval.expr) = (yyvsp[(1) - (1)].expr);
            // $$->type = $1->type;
          }
    break;

  case 42:
#line 457 "parse.y"
    {
                          if(DEBUG) printf("Got to mult\n");
                          (yyval.expr) = malloc(sizeof(struct ast_expression));
                          (yyval.expr)->kind = KIND_OP;
                          (yyval.expr)->operator = OP_MUL;
                          (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                          (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                       }
    break;

  case 43:
#line 465 "parse.y"
    {
                          if(DEBUG) printf("Got to div\n");
                          (yyval.expr) = malloc(sizeof(struct ast_expression));
                          (yyval.expr)->kind = KIND_OP;
                          (yyval.expr)->operator = OP_DIV;
                          (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
                          (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
                      }
    break;

  case 44:
#line 473 "parse.y"
    {
              if(DEBUG) printf("Got to factor\n");
                //$$ = malloc(sizeof(struct ast_expression));
                //$$->kind = KIND_OP;
                //$$->l_operand = $1;
                (yyval.expr) = (yyvsp[(1) - (1)].expr);
                // $$->type = $1->type;
             }
    break;

  case 45:
#line 482 "parse.y"
    {
                    if(DEBUG) printf("Got to negation\n");
                    (yyval.expr) = malloc(sizeof(struct ast_expression));
                    (yyval.expr)->kind = KIND_OP;
                    (yyval.expr)->operator = OP_UMIN;
                    (yyval.expr)->r_operand = (yyvsp[(2) - (2)].expr);
                   }
    break;

  case 46:
#line 489 "parse.y"
    {
              if(DEBUG) printf("Got to unit\n");
              (yyval.expr) = (yyvsp[(1) - (1)].expr);
              // $$->type = $1->type;
             }
    break;

  case 47:
#line 496 "parse.y"
    { // Parses integers
                if(DEBUG) printf("%d\n", (yyvsp[(1) - (1)].ival));
                  (yyval.expr) = malloc(sizeof(struct ast_expression));
                  (yyval.expr)->kind = KIND_INT;
                  (yyval.expr)->value = (yyvsp[(1) - (1)].ival);
                  (yyval.expr)->type = TYPE_INT;
                  (yyval.expr)->r_operand = NULL; // Last ditch effort
                  (yyval.expr)->l_operand = NULL;
               }
    break;

  case 48:
#line 505 "parse.y"
    { // Parses reals
                  if(DEBUG) printf("%f\n", (yyvsp[(1) - (1)].rval));
                  (yyval.expr) = malloc(sizeof(struct ast_expression));
                  (yyval.expr)->kind = KIND_REAL;
                  (yyval.expr)->rvalue = (yyvsp[(1) - (1)].rval);
                  (yyval.expr)->type = TYPE_REAL;
                  (yyval.expr)->r_operand = NULL;
                  (yyval.expr)->l_operand = NULL;
                }
    break;

  case 49:
#line 514 "parse.y"
    {
                            if(DEBUG) printf("Got to parenthesized expression\n");
                            // $2 = malloc(sizeof(struct ast_expression));
                            (yyval.expr) = (yyvsp[(2) - (3)].expr);
                          }
    break;

  case 50:
#line 519 "parse.y"
    {
         if(DEBUG) printf("Got to VAR\n");
         (yyval.expr) = malloc(sizeof(struct ast_expression));
         (yyval.expr)->type = TYPE_VAR;
         int tableLoc = isPresent((yyvsp[(1) - (1)].sval));
         if(DEBUG) printf("tableLoc: %d\n", tableLoc);
         int varType = table->table[tableLoc].type;
         if(DEBUG) printf("varType: %d\n", varType);
         if(varType == TYPE_INT) {
           (yyval.expr)->l_operand = malloc(sizeof(struct ast_expression)); // Trying to hack together a solution with this
          //  $$->l_operand->kind = KIND_INT;
           (yyval.expr)->l_operand->type = TYPE_VAR;
           (yyval.expr)->l_operand->target = &table->table[tableLoc];
         }
         if(varType == TYPE_REAL) {
           (yyval.expr)->l_operand = malloc(sizeof(struct ast_expression)); // Trying to hack together a solution with this
          //  $$->l_operand->kind = KIND_REAL;
           (yyval.expr)->l_operand->type = TYPE_VAR;
           (yyval.expr)->l_operand->target = &table->table[tableLoc];
         }
       }
    break;

  case 51:
#line 540 "parse.y"
    {
         if(DEBUG) printf("Got to VAR LBRACK LITINT RBRACK\n");
         (yyval.expr) = malloc(sizeof(struct ast_expression));
         (yyval.expr)->type = TYPE_VAR;
         int tableLoc = isPresent((yyvsp[(1) - (4)].sval));
         if(DEBUG) printf("tableLoc: %d\n", tableLoc);
         int varType = table->table[tableLoc].type;
         if(DEBUG) printf("varType: %d\n", varType);
         if(varType == TYPE_INT) {
           (yyval.expr)->l_operand = malloc(sizeof(struct ast_expression));
           (yyval.expr)->l_operand->type = TYPE_VAR;
           (yyval.expr)->l_operand->target = &table->table[tableLoc];
          //  $$->l_operand->address = table->table[tableLoc].address + $3;
           (yyval.expr)->l_operand->arrayOffset = (yyvsp[(3) - (4)].ival);
         }
         if(varType == TYPE_REAL) {
           (yyval.expr)->l_operand = malloc(sizeof(struct ast_expression));
           (yyval.expr)->l_operand->type = TYPE_VAR;
           (yyval.expr)->l_operand->target = &table->table[tableLoc];
          //  $$->l_operand->address = table->table[tableLoc].address + $3;
           (yyval.expr)->l_operand->arrayOffset = (yyvsp[(3) - (4)].ival);
         }
       }
    break;

  case 52:
#line 569 "parse.y"
    {
              if(DEBUG) printf("Got to outputstmt: RWPRINT printlist\n");
              // printf("printlist = %s\n", $2); // Doesn't work with carriage returns
              (yyval.expr) = malloc(sizeof(struct ast_expression));
              (yyval.expr)->operator = OP_PRINT;
              (yyval.expr)->r_operand = (yyvsp[(2) - (2)].expr);
              // printf("%d\n",strlen($$->charString));
              // char buf[1024];
              // struct ast_expression *tempExp = $$->r_operand;
              // while(tempExp != NULL) {
              //   // Parse the entire charstring
              //   // printf("GOT HERE\n");
              //   if(tempExp->l_operand != NULL) {
              //     tempExp = tempExp->r_operand;
              //     continue;
              //   }
              //   strcat(buf, tempExp->charString);
              //   printf("%s\n",buf);
              //   tempExp = tempExp->r_operand;
              // }
            // if(DEBUG) printf("buf = %s\n", buf);
            //   $$->charString = strdup(buf);
            // if(DEBUG) printf("Got out of loop\n");
            }
    break;

  case 53:
#line 594 "parse.y"
    {
            if(DEBUG) printf("Got to CHARSTRING printlist\n");
            if(DEBUG) printf("CHARSTRING: %s\n", (yyvsp[(1) - (2)].sval));
            (yyval.expr) = malloc(sizeof(struct ast_expression));
            (yyval.expr)->r_operand = malloc(sizeof(struct ast_expression));
            (yyval.expr)->charString = (yyvsp[(1) - (2)].sval);
            (yyval.expr)->r_operand = (yyvsp[(2) - (2)].expr);
          }
    break;

  case 54:
#line 602 "parse.y"
    {
            if(DEBUG) printf("Got to CHARSTRING COMMA printlist\n");
            if(DEBUG) printf("CHARSTRING: %s\n", (yyvsp[(1) - (3)].sval));
            (yyval.expr) = malloc(sizeof(struct ast_expression));
            (yyval.expr)->r_operand = malloc(sizeof(struct ast_expression));
            // Temporarily getting rid of this comma to see if it makes printing easier
            // char *temp = ",";
            // strncat($1, temp, 1); // Append a comma to this string.
            (yyval.expr)->charString = (yyvsp[(1) - (3)].sval);
            (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
          }
    break;

  case 55:
#line 613 "parse.y"
    {
            (yyval.expr) = malloc(sizeof(struct ast_expression));
            (yyval.expr)->r_operand = malloc(sizeof(struct ast_expression));
            (yyval.expr)->charString = (yyvsp[(1) - (2)].sval);
            (yyval.expr)->r_operand = (yyvsp[(2) - (2)].expr);
            if(DEBUG) printf("Got to CARRETURN printlist\n");
          }
    break;

  case 56:
#line 620 "parse.y"
    {
            (yyval.expr) = malloc(sizeof(struct ast_expression));
            (yyval.expr)->r_operand = malloc(sizeof(struct ast_expression));
            (yyval.expr)->charString = "!,";
            (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
            if(DEBUG) printf("Got to CARRETURN COMMA printlist\n");
          }
    break;

  case 57:
#line 627 "parse.y"
    {
            if(DEBUG) printf("Got to bexp COMMA printlist\n");
            (yyval.expr) = malloc(sizeof(struct ast_expression));
            // $$->r_operand = $3;
            // What if I set $$->l_operand to $1? Then I could just parse that part for bexp's...
            // $$ = $1;
            // $$->charString = $3->charString;
            (yyval.expr)->l_operand = malloc(sizeof(struct ast_expression));
            (yyval.expr)->r_operand = malloc(sizeof(struct ast_expression));
            (yyval.expr)->l_operand = (yyvsp[(1) - (3)].expr);
            (yyval.expr)->r_operand = (yyvsp[(3) - (3)].expr);
          }
    break;

  case 58:
#line 639 "parse.y"
    {
            (yyval.expr) = malloc(sizeof(struct ast_expression));
            if(DEBUG) printf("Got to bexp SEMICOLON\n");
            // $$->charString = NULL;
            // Added for debug...
            (yyval.expr)->l_operand = malloc(sizeof(struct ast_expression));
            (yyval.expr)->l_operand = (yyvsp[(1) - (2)].expr);
            // $$->r_operand = malloc(sizeof(struct ast_expression));
            // $$->r_operand = $1;
            // Variable references are stored on the l_operand side of an exp
            // There's a segfault under this line... probably because the address isn't being assigned if a literal is passed in?
            // $$->address = $$->l_operand->target->address;
            // $$->target = &table->table[isPresent($1->l_operand->target->name)];
            // printf("$1->l_operand->target->name: %s\n", $1->l_operand->target->name);
            // $$->r_operand->charString = NULL;
            // $$->charString = "Found_a_vari";
          }
    break;

  case 59:
#line 656 "parse.y"
    {
            (yyval.expr) = NULL;
            // $$->r_operand = NULL;
          }
    break;

  case 60:
#line 662 "parse.y"
    {
              if(DEBUG) printf("Got to RWREAD varref SEMICOLON\n");
              (yyval.expr) = malloc(sizeof(struct ast_expression));
              (yyval.expr)->address = table->table[isPresent((yyvsp[(2) - (3)].entry)->name)].address;
              (yyval.expr)->operator = OP_READ;
           }
    break;

  case 61:
#line 703 "parse.y"
    {
              if(DEBUG) printf("Found an if\n");
              (yyval.ifstmt) = malloc(sizeof(struct ast_if_stmt));
              (yyval.ifstmt)->conditional_stmt = (yyvsp[(2) - (7)].expr);
              (yyval.ifstmt)->body = (yyvsp[(4) - (7)].stmt);
              (yyval.ifstmt)->isIfElse = 0;
             }
    break;

  case 62:
#line 710 "parse.y"
    {
               if(DEBUG) printf("Found an if else\n");
               (yyval.ifstmt) = malloc(sizeof(struct ast_if_stmt));
               (yyval.ifstmt)->conditional_stmt = (yyvsp[(2) - (10)].expr);
               (yyval.ifstmt)->body = (yyvsp[(4) - (10)].stmt);
               // Added the tempLink structure to actually get nested if statements to work when else statements are present.
               (yyval.ifstmt)->tempLink = malloc(sizeof(struct statement));
               (yyval.ifstmt)->tempLink = (yyvsp[(7) - (10)].stmt);
              //  $$->body->link = malloc(sizeof(struct statement));
              //  $$->body->link = $7;
              //  $$->body->link->isCond = 1;
              (yyval.ifstmt)->isIfElse = 1;
             }
    break;

  case 63:
#line 724 "parse.y"
    {
              if(DEBUG) printf("Got to whilestmt: RWWHILE bexp SEMICOLON programbody RWEND RWWHILE SEMICOLON\n");
              (yyval.while_loop) = malloc(sizeof(struct ast_while_stmt));
              (yyval.while_loop)->conditional_stmt = (yyvsp[(2) - (7)].expr);
              (yyval.while_loop)->body = (yyvsp[(4) - (7)].stmt);
           }
    break;

  case 64:
#line 731 "parse.y"
    { if(DEBUG) printf("Got to endmainstmt\n");}
    break;


/* Line 1267 of yacc.c.  */
#line 2327 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 733 "parse.y"

int yyerror() {
  printf("Called yyerror()\n");
  return 0;
}

