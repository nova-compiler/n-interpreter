%option never-interactive

%{
#include <cstring>
#include <new>

#include "ast-generator.hpp"
#include "parser.hpp"

namespace Internal {
	extern Nova::AST::ObjectManager * objectManager;
	static Nova::AST::FileContext fileContext;
}

using namespace Nova::AST;

#define STRING_TOKEN(t) { \
		char * buffer = Internal::objectManager->AllocateCharArray(yyleng + 1); \
		std::memcpy(buffer, yytext, yyleng), buffer[yyleng] = 0; \
		yylval.node = Internal::objectManager->CreateObject<StringToken>(buffer, t, Internal::fileContext); \
		Internal::fileContext.ColumnNumber += yyleng; \
		return t; \
	}

#define STATIC_TOKEN(t) { \
		yylval.node = Internal::objectManager->CreateObject<StaticToken>(t, Internal::fileContext); \
		Internal::fileContext.ColumnNumber += yyleng; \
		return t; \
	}

extern "C" int yywrap() { return 1; }
%}

%%
[ \t]                     { ++Internal::fileContext.ColumnNumber; }
[\n\r]                    { ++Internal::fileContext.LineNumber, Internal::fileContext.ColumnNumber = 0; }
var                       STATIC_TOKEN(T_VAR);
[a-zA-Z][a-zA-Z0-9]*      STRING_TOKEN(T_IDENTIFIER);
[0-9]+\.[0-9]*            STRING_TOKEN(T_DOUBLE);
[0-9]+                    STRING_TOKEN(T_INTEGER);
"="                       STATIC_TOKEN(T_ASSIGN);
"+="                      STATIC_TOKEN(T_ADD_ASSIGN);
"-="                      STATIC_TOKEN(T_SUB_ASSIGN);
"*="                      STATIC_TOKEN(T_MULT_ASSIGN);
"/="                      STATIC_TOKEN(T_DIV_ASSIGN);
"<<="                     STATIC_TOKEN(T_LSFT_ASSIGN);
">>="                     STATIC_TOKEN(T_RSFT_ASSIGN);
"=="                      STATIC_TOKEN(T_EQL_COMP);
"!="                      STATIC_TOKEN(T_NEQ_COMP);
"<"                       STATIC_TOKEN(T_LSS_COMP);
"<="                      STATIC_TOKEN(T_LEQ_COMP);
">"                       STATIC_TOKEN(T_GTR_COMP);
">="                      STATIC_TOKEN(T_GEQ_COMP);
"("                       STATIC_TOKEN(T_OPEN_PAREN);
")"                       STATIC_TOKEN(T_CLOSE_PAREN);
"{"                       STATIC_TOKEN(T_LEFT_BRACE);
"}"                       STATIC_TOKEN(T_RIGHT_BRACE);
"["                       STATIC_TOKEN(T_LEFT_BRACKET);
"]"                       STATIC_TOKEN(T_RIGHT_BRACKET);
"."                       STATIC_TOKEN(T_DOT);
","                       STATIC_TOKEN(T_COMMA);
";"                       STATIC_TOKEN(T_SEMICOLON);
":"                       STATIC_TOKEN(T_COLON);
"..."                     STATIC_TOKEN(T_ELLIPSIS);
"+"                       STATIC_TOKEN(T_PLUS);
"-"                       STATIC_TOKEN(T_MINUS);
"*"                       STATIC_TOKEN(T_MUL);
"/"                       STATIC_TOKEN(T_DIV);
"&"                       STATIC_TOKEN(T_AND);
"|"                       STATIC_TOKEN(T_OR);
"^"                       STATIC_TOKEN(T_XOR);
"!"                       STATIC_TOKEN(T_NOT);
"<<"                      STATIC_TOKEN(T_LSFT);
">>"                      STATIC_TOKEN(T_RSFT);
"++"                      STATIC_TOKEN(T_INCR);
"--"                      STATIC_TOKEN(T_DECR);
"<<<"                     STATIC_TOKEN(T_UNARY_LSFT);
">>>"                     STATIC_TOKEN(T_UNARY_RSFT);
<<EOF>>                   yyterminate();
.                         printf("Unknown token!\n"); yyterminate();

%%