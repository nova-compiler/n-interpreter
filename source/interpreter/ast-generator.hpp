//
// ast-generator.hpp
//
// Copyright (c) 2013 Luis Garcia.
// This source file is subject to terms of the MIT License. (See accompanying file LICENSE)
//

#ifndef _NOVA_AST_GENERATOR_HEADER_
#define _NOVA_AST_GENERATOR_HEADER_

#include <vector>
#include <utility>
#include <stdexcept>

#include "context.hpp"

namespace Nova {

	enum BasicType {
		Char, Short, Int, Long, Double, String
	};

	namespace Operators {
		enum Values {
			Assign, Addition
		};
	}

namespace AST {

	/*
	 * Utility structure to handle positions in the source files. The FilePath
	 * field is expected to be valid during all the compilation phase.
	 */
	struct FileContext {
		const char * FilePath;
		int LineNumber, ColumnNumber;
	};

	/*
	 * Base class for every node in the AST.
	 */
	class BaseNode {
	public:
		virtual ~BaseNode(
			) {
			}
	};

	/*
	 * Used for non-constant tokens, as identifiers and numeric literals.
	 */
	class StringToken
		: public BaseNode
	{
		const char * _string;
		int _id;
		FileContext _fileContext;

	public:
		StringToken(
			const char * string, int id, const FileContext & fileContext
			)
			: _string(string), _id(id), _fileContext(fileContext)
			{
			}

		const char * GetString(
			) const {
				return _string;
			}
	};

	/*
	 * Used for constant tokens, as operators and keywords.
	 */
	class StaticToken
		: public BaseNode
	{
		int _id;
		FileContext _fileContext;

	public:
		StaticToken(
			int id, const FileContext & fileContext
			)
			: _id(id), _fileContext(fileContext)
			{
			}
	};

	/*
	 * Represents a expression in the file. This class is inhereted for any
	 * expression type.
	 */
	class Expression
		: public BaseNode
	{
	public:
		virtual void Generate(
			CompilationContext & context, TypeInfo & resultType
			) const = 0;
	};

	/*
	 * Represents a non-string constant in the file.
	 */
	template <
		typename _Ty
	>
	class Constant
		: public Expression
	{
		_Ty _value;
		BasicType _type;

	public:
		Constant(
			_Ty value, BasicType type
			)
			: _value(value), _type(type)
			{
			}

		virtual void Generate(
			CompilationContext & context, TypeInfo & resultType
			) const {
			}
	};

	/*
	 * Indicates an operation expression.
	 */
	class OperationExpression
		: public Expression
	{
		const Expression * _leftSide, * _rightSide;
		Operators::Values _operator;

	public:
		OperationExpression(
			const Expression * leftSide, Operators::Values oper, const Expression * rightSide
			)
			: _leftSide(leftSide), _operator(oper), _rightSide(rightSide)
			{
			}

		void Generate(
			CompilationContext & context, TypeInfo & resultType
			) const {
				TypeInfo leftType, rightType;
				_leftSide->Generate(context, leftType);
				_rightSide->Generate(context, rightType);
			}
	};

	class Identifier
		: public BaseNode
	{
		const char * _name;

	public:
		Identifier(
			const char * name
			)
			: _name(name)
			{
			}

		inline const char * GetName(
			) const {
				return _name;
			}
	};
	
	class IdentifierExpression
		: public Expression
	{
		const Identifier * _identifier;

	public:
		IdentifierExpression(
			const Identifier * identifier
			)
			: _identifier(identifier)
			{
			}

		void Generate(
			CompilationContext & context, TypeInfo & resultType
			) const {
			}
	};

	class IdentifierDefinition
		: public BaseNode
	{
		const Identifier * _identifier;
		const Expression * _expr;

	public:
		IdentifierDefinition(
			const Identifier * identifier, const Expression * expr
			)
			: _identifier(identifier), _expr(expr)
			{
			}

		void Generate(
			CompilationContext & context
			) const {
			}
	};

	class IdentifierDefinitionList
		: public BaseNode
	{
		std::vector<const IdentifierDefinition *> _identifiers;

	public:
		IdentifierDefinitionList(
			) {
			}

		inline void Add(
			const IdentifierDefinition * identifier
			) {
				_identifiers.push_back(identifier);
			}

		void Generate(
			CompilationContext & context
			) const {
			}
	};

	class Statement
		: public BaseNode
	{
	public:
		virtual void Generate(
			CompilationContext & context
			) const = 0;
	};

	class StatementList
		: public BaseNode
	{
		std::vector<const Statement *> _statements;

	public:
		inline StatementList(
			) {
			}

		inline void Add(
			const Statement * statement
			) {
				_statements.push_back(statement);
			}

		void Generate(
			CompilationContext & context
			) const {
				for (size_t i = 0; i < _statements.size(); ++i)
					_statements[i]->Generate(context);
			}
	};

	class ExpressionStatement
		: public Statement
	{
		const Expression * _expr;

	public:
		inline ExpressionStatement(
			const Expression * expr
			)
			: _expr(expr)
			{
			}

		void Generate(
			CompilationContext & context
			) const {
				TypeInfo typeInfo;
				_expr->Generate(context, typeInfo);
			}
	};

	class DeclarationList
		: public BaseNode
	{
		std::vector<std::pair<const IdentifierDefinitionList *, const Expression *>> _definitionLists;

	public:
		inline DeclarationList(
			) {
			}

		inline void Add(
			const IdentifierDefinitionList * definitionList
			) {
				_definitionLists.push_back(std::make_pair(definitionList, nullptr));
			}

		inline void Add(
			const IdentifierDefinitionList * definitionList, const Expression * typeExpression
			) {
				_definitionLists.push_back(std::make_pair(definitionList, typeExpression));
			}

		inline void Generate(
			CompilationContext & context
			) const {
				for (size_t i = _definitionLists.size(); i > 0; --i)
					_definitionLists[i - 1].first->Generate(context);
			}
	};

	class DeclarationStatement
		: public Statement
	{
		const DeclarationList * _declarations;

	public:
		inline DeclarationStatement(
			const DeclarationList * declarations
			)
			: _declarations(declarations)
			{
			}

		inline void Generate(
			CompilationContext & context
			) const {
				_declarations->Generate(context);
			}
	};

	class ObjectManager {
		std::vector<BaseNode *> _objects;
		std::vector<char *> _arrays;

	public:
		inline ~ObjectManager(
			) {
				Clear();
			}

		template <
			typename _Ty
		>
		inline _Ty * CreateObject(
			) {
				_Ty * p = new _Ty();
				_objects.push_back(p);
				return p;
			}

		template <
			typename _Ty, typename _Arg1Ty
		>
		inline _Ty * CreateObject(
			const _Arg1Ty & arg1
			) {
				_Ty * p = new _Ty(arg1);
				_objects.push_back(p);
				return p;
			}

		template <
			typename _Ty, typename _Arg1Ty, typename _Arg2Ty
		>
		inline _Ty * CreateObject(
			const _Arg1Ty & arg1, const _Arg2Ty & arg2
			) {
				_Ty * p = new _Ty(arg1, arg2);
				_objects.push_back(p);
				return p;
			}

		template <
			typename _Ty, typename _Arg1Ty, typename _Arg2Ty, typename _Arg3Ty
		>
		inline _Ty * CreateObject(
			const _Arg1Ty & arg1, const _Arg2Ty & arg2, const _Arg3Ty & arg3
			) {
				_Ty * p = new _Ty(arg1, arg2, arg3);
				_objects.push_back(p);
				return p;
			}

		inline char * AllocateCharArray(
			size_t size
			) {
				char * p = new char[size];
				_arrays.push_back(p);
				return p;
			}

		inline void Clear(
			) {
				for (auto object : _objects) delete object;
				_objects.clear();
				for (auto array : _arrays) delete[] array;
				_arrays.clear();
			}
	};
}
}


#endif // !_NOVA_AST_GENERATOR_HEADER_