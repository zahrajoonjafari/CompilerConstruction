/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_MCC_PARSER_MCC_SHA_PARSER_TAB_H_INCLUDED
# define YY_MCC_PARSER_MCC_SHA_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef MCC_PARSER_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define MCC_PARSER_DEBUG 1
#  else
#   define MCC_PARSER_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define MCC_PARSER_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined MCC_PARSER_DEBUG */
#if MCC_PARSER_DEBUG
extern int mcc_parser_debug;
#endif
/* "%code requires" blocks.  */
#line 10 "../src/parser.y" /* yacc.c:1909  */

#include "mcc/parser.h"
#include <stdbool.h>

#line 57 "mcc@sha/parser.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef MCC_PARSER_TOKENTYPE
# define MCC_PARSER_TOKENTYPE
  enum mcc_parser_tokentype
  {
    TK_END = 0,
    TK_INT_LITERAL = 258,
    TK_FLOAT_LITERAL = 259,
    TK_STRING_LITERAL = 260,
    TK_BOOL_LITERAL = 261,
    TK_IDENTIFIER = 262,
    TK_LPARENTH = 263,
    TK_RPARENTH = 264,
    TK_LBRACKET = 265,
    TK_RBRACKET = 266,
    TK_LBRACE = 267,
    TK_RBRACE = 268,
    TK_PLUS = 269,
    TK_MINUS = 270,
    TK_ASTER = 271,
    TK_SLASH = 272,
    TK_ASSIGNMENT = 273,
    TK_LESS = 274,
    TK_GREATER = 275,
    TK_LESS_EQ = 276,
    TK_GREATER_EQ = 277,
    TK_EQUALS = 278,
    TK_NOT_EQUALS = 279,
    TK_SEMICOLON = 280,
    TK_COMMA = 281,
    TK_NOT = 282,
    TK_AND = 283,
    TK_OR = 284,
    TK_BOOL_TYPE = 285,
    TK_INT_TYPE = 286,
    TK_FLOAT_TYPE = 287,
    TK_STRING_TYPE = 288,
    TK_VOID = 289,
    TK_IF = 290,
    TK_ELSE = 291,
    TK_WHILE = 292,
    TK_FOR = 293,
    TK_RETURN = 294
  };
#endif

/* Value type.  */
#if ! defined MCC_PARSER_STYPE && ! defined MCC_PARSER_STYPE_IS_DECLARED

union MCC_PARSER_STYPE
{

  /* "bool literal"  */
  bool TK_BOOL_LITERAL;
  /* "string literal"  */
  char* TK_STRING_LITERAL;
  /* "identifier"  */
  char* TK_IDENTIFIER;
  /* "float literal"  */
  double TK_FLOAT_LITERAL;
  /* binary_op  */
  enum mcc_ast_binary_op TK_binary_op;
  /* unary_op  */
  enum mcc_ast_unary_op TK_unary_op;
  /* "integer literal"  */
  long TK_INT_LITERAL;
  /* arguments  */
  struct mcc_ast_arguments * TK_arguments;
  /* expression  */
  struct mcc_ast_expression * TK_expression;
  /* call_expression  */
  struct mcc_ast_expression * TK_call_expression;
  /* literal  */
  struct mcc_ast_literal * TK_literal;
  /* single_expression  */
  struct mcc_ast_single_expression * TK_single_expression;
#line 135 "mcc@sha/parser.tab.h" /* yacc.c:1909  */
};

typedef union MCC_PARSER_STYPE MCC_PARSER_STYPE;
# define MCC_PARSER_STYPE_IS_TRIVIAL 1
# define MCC_PARSER_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined MCC_PARSER_LTYPE && ! defined MCC_PARSER_LTYPE_IS_DECLARED
typedef struct MCC_PARSER_LTYPE MCC_PARSER_LTYPE;
struct MCC_PARSER_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define MCC_PARSER_LTYPE_IS_DECLARED 1
# define MCC_PARSER_LTYPE_IS_TRIVIAL 1
#endif



int mcc_parser_parse (void *scanner, struct mcc_ast_expression** result);

#endif /* !YY_MCC_PARSER_MCC_SHA_PARSER_TAB_H_INCLUDED  */
