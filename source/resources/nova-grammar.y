%{
#include <cstdio>
#include <cstdlib>

#define YYERROR_VERBOSE
#define YYDEBUG 1

#include "ast-generator.hpp"

using namespace Nova;
using namespace AST;

namespace Internal {
	extern ObjectManager * objectManager;
	extern StatementList * astOutput;
	static FileContext fileContext;
}

extern int yylex();
void yyerror(const char *s) { std::printf("Error: %s\n", s); }

#define RESERVE(type, ...) Internal::objectManager->CreateObject<type>(__VA_ARGS__)
%}

/* Represents the many different ways we can access our data */
%union {
    Nova::AST::BaseNode * node;
}

%token END 0 "end of file"

/* String terminals */
%token <node> T_IDENTIFIER T_DOUBLE T_INTEGER

/* Assignment operators */
%token <node> T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MULT_ASSIGN T_DIV_ASSIGN T_LSFT_ASSIGN T_RSFT_ASSIGN

/* Expression operators */
%token <node> T_PLUS T_MINUS T_MUL T_DIV T_AND T_OR T_XOR T_NOT T_LSFT T_RSFT

/* Comparision operators */
%token <node> T_EQL_COMP T_NEQ_COMP T_LSS_COMP T_LEQ_COMP T_GTR_COMP T_GEQ_COMP

/* Unary operators */
%token <node> T_INCR T_DECR T_UNARY_LSFT T_UNARY_RSFT

/* Braces */
%token <node> T_OPEN_PAREN T_CLOSE_PAREN T_LEFT_BRACE T_RIGHT_BRACE T_LEFT_BRACKET T_RIGHT_BRACKET

/* Other operators */
%token T_VAR T_DOT T_COMMA T_SEMICOLON T_COLON T_ELLIPSIS

%type <node> program stmts stmt expression_stmt expression numeric_expression identifier_expression declaration_stmt declaration_list identifier_definition_list identifier_definition identifier

/* Operator precedence for mathematical operators */
%right T_ASSIGN T_ADD_ASSIGN T_SUB_ASSIGN T_MULT_ASSIGN T_DIV_ASSIGN T_LSFT_ASSIGN T_RSFT_ASSIGN
%left T_LSFT T_RSFT
%left T_EQL_COMP T_NEQ_COMP T_LSS_COMP T_LEQ_COMP T_GTR_COMP T_GEQ_COMP
%left T_AND T_OR T_XOR
%left T_PLUS T_MINUS
%left T_MUL T_DIV

%start program

%%

program
		: stmts
			{
				Internal::astOutput = reinterpret_cast<StatementList *>($1);
				$$ = $1;
			}
		;

stmts
		: stmt
			{
				StatementList * statements = RESERVE(StatementList);
				statements->Add(reinterpret_cast<Statement *>($1));
				$$ = statements;
			}
		| stmts stmt
			{
				StatementList * statements = reinterpret_cast<StatementList *>($1);
				statements->Add(reinterpret_cast<Statement *>($2));
				$$ = statements;
			}
		;

stmt
		: expression_stmt
		| declaration_stmt
		;

expression_stmt
		: expression T_SEMICOLON
			{
				$$ = RESERVE(ExpressionStatement, reinterpret_cast<Expression *>($1));
			}
		;

expression
		: T_OPEN_PAREN expression T_CLOSE_PAREN
			{ $$ = $2; }
		| expression T_ASSIGN expression
			{
				$$ = RESERVE(OperationExpression,
						(Expression *) $1, Operators::Assign, (Expression *) $3);
			}
		| expression T_PLUS expression
			{
				$$ = RESERVE(OperationExpression,
						(Expression *) $1, Operators::Addition, (Expression *) $3);
			}
		| identifier_expression
		| numeric_expression
		;

declaration_stmt
		: T_VAR declaration_list T_SEMICOLON
			{
				$$ = RESERVE(DeclarationStatement, reinterpret_cast<DeclarationList *>($2));
			}
		;

declaration_list
		: identifier_definition_list
			{
				DeclarationList * declarations = RESERVE(DeclarationList);
				declarations->Add(reinterpret_cast<IdentifierDefinitionList *>($1));
				$$ = declarations;
			}
		| identifier_definition_list T_COLON expression
			{
				DeclarationList * declarations = RESERVE(DeclarationList);
				declarations->Add(reinterpret_cast<IdentifierDefinitionList *>($1), reinterpret_cast<Expression *>($3));
				$$ = declarations;
			}
		| identifier_definition_list T_COLON expression T_COMMA declaration_list
			{
				DeclarationList * declarations = reinterpret_cast<DeclarationList *>($5);
				declarations->Add(reinterpret_cast<IdentifierDefinitionList *>($1), reinterpret_cast<Expression *>($3));
				$$ = declarations;
			}
		;

identifier_definition_list
		: identifier_definition
			{
				IdentifierDefinitionList * identifiers = RESERVE(IdentifierDefinitionList);
				identifiers->Add(reinterpret_cast<IdentifierDefinition *>($1));
				$$ = identifiers;
			}
		| identifier_definition_list T_COMMA identifier_definition
			{
				IdentifierDefinitionList * identifiers = reinterpret_cast<IdentifierDefinitionList *>($1);
				identifiers->Add(reinterpret_cast<IdentifierDefinition *>($3));
				$$ = identifiers;
			}
		;

identifier_definition
		: identifier T_ASSIGN expression
			{
				$$ = RESERVE(IdentifierDefinition,
						reinterpret_cast<const Identifier *>($1),
						reinterpret_cast<const Expression *>($3));
			}
		| identifier
			{
				$$ = RESERVE(IdentifierDefinition,
						reinterpret_cast<const Identifier *>($1), nullptr);
			}
		;

numeric_expression
		: T_INTEGER
			{
				int value = atol(reinterpret_cast<StringToken *>($1)->GetString());
				$$ = RESERVE(Constant<int>, value, Int);
			}
		;

identifier_expression
		: identifier
			{
				$$ = RESERVE(IdentifierExpression, reinterpret_cast<Identifier *>($1));
			}
		;

identifier
		: T_IDENTIFIER
			{
				const char * name = reinterpret_cast<StringToken *>($1)->GetString();
				char * buffer = Internal::objectManager->AllocateCharArray(strlen(name) + 1);
				memcpy(buffer, name, (strlen(name) + 1) * sizeof(char));
				$$ = RESERVE(Identifier, buffer);
			}
		;
%%