
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 0,
     T_IDENTIFIER = 258,
     T_DOUBLE = 259,
     T_INTEGER = 260,
     T_ASSIGN = 261,
     T_ADD_ASSIGN = 262,
     T_SUB_ASSIGN = 263,
     T_MULT_ASSIGN = 264,
     T_DIV_ASSIGN = 265,
     T_LSFT_ASSIGN = 266,
     T_RSFT_ASSIGN = 267,
     T_PLUS = 268,
     T_MINUS = 269,
     T_MUL = 270,
     T_DIV = 271,
     T_AND = 272,
     T_OR = 273,
     T_XOR = 274,
     T_NOT = 275,
     T_LSFT = 276,
     T_RSFT = 277,
     T_EQL_COMP = 278,
     T_NEQ_COMP = 279,
     T_LSS_COMP = 280,
     T_LEQ_COMP = 281,
     T_GTR_COMP = 282,
     T_GEQ_COMP = 283,
     T_INCR = 284,
     T_DECR = 285,
     T_UNARY_LSFT = 286,
     T_UNARY_RSFT = 287,
     T_OPEN_PAREN = 288,
     T_CLOSE_PAREN = 289,
     T_LEFT_BRACE = 290,
     T_RIGHT_BRACE = 291,
     T_LEFT_BRACKET = 292,
     T_RIGHT_BRACKET = 293,
     T_VAR = 294,
     T_DOT = 295,
     T_COMMA = 296,
     T_SEMICOLON = 297,
     T_COLON = 298,
     T_ELLIPSIS = 299
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 26 "source\\resources\\nova-grammar.y"

    Nova::AST::BaseNode * node;



/* Line 1676 of yacc.c  */
#line 103 "C:\\Development\\Projects\\Nova\\Source\\n-interpreter\\source\\interpreter\\parser.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


