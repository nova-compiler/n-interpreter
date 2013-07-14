//
// main.cpp
//
// Copyright (c) 2013 Luis Garcia.
// This source file is subject to terms of the MIT License. (See accompanying file LICENSE)
//

#include "interpreter\ast-generator.hpp"
#include "interpreter\parser.hpp"

#include <iostream>

using namespace Nova;

namespace Internal {
	AST::ObjectManager * objectManager = new AST::ObjectManager();
	AST::StatementList * astOutput = nullptr;
}

using namespace std;

extern int yylex();
extern FILE * yyin;

int main(
	int argc, char* argv[]
	) {
		if (argc != 2)
			cout << "No input file." << endl;

		FILE * file = fopen(argv[1], "r");
		yyin = file;
		yylex();
		fclose(file);

		delete Internal::objectManager;
	}