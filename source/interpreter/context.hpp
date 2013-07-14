//
// context.hpp
//
// Copyright (c) 2013 Luis Garcia.
// This source file is subject to terms of the MIT License. (See accompanying file LICENSE)
//

#ifndef _NOVA_CONTEXT_HEADER_
#define _NOVA_CONTEXT_HEADER_

#include <map>
#include <string>

namespace Nova {
	class IdentifierInfo;

	class TypeInfo {
		bool _reference, _constant;
		const IdentifierInfo * _baseTypeIdentifier;

	public:
		inline TypeInfo(
			)
			: _baseTypeIdentifier(nullptr), _reference(false), _constant(false)
			{
			}

		inline TypeInfo(
			const IdentifierInfo * baseTypeIdentifier, bool reference, bool constant
			)
			: _baseTypeIdentifier(baseTypeIdentifier), _reference(reference), _constant(constant)
			{
			}

		inline const IdentifierInfo * GetBaseTypeIdentifier(
			) const {
				return _baseTypeIdentifier;
			}

		inline bool IsReference(
			) const {
				return _reference;
			}

		inline bool IsConstant(
			) const {
				return _constant;
			}
	};

	class IdentifierInfo {
		const char * _name;
		TypeInfo _typeInfo;

	public:
		inline IdentifierInfo(
			const char * name, const TypeInfo & typeInfo
			)
			: _name(name), _typeInfo(typeInfo)
			{
			}

		const char * GetName(
			) const {
				return _name;
			}

		TypeInfo GetTypeInfo(
			) const {
				return _typeInfo;
			}
	};

	class BytecodeOutputStream;
	class Signature;

	class IdentifierHandler {
	public:
		virtual bool Accept(
			const Signature & signature
			) const = 0;
	};

	class ExpressionCodeGenerator {
	public:
		virtual void GenerateCode(
			BytecodeOutputStream & outputStream, TypeInfo & typeInfo
			) const = 0;

		virtual bool Accept(
			) const = 0;
	};

	class SymbolTable {
		std::map<std::string, IdentifierInfo *> _identifiers;

		inline const IdentifierInfo * _RegisterIdentifier(
			const char * name, const TypeInfo & typeInfo
			) {
				IdentifierInfo * p = new IdentifierInfo(name, typeInfo);
				_identifiers[name] = p;
				return p;
			}

		inline void _startGlobalScope(
			) {
				_identifiers["type"] = nullptr;
				
				_RegisterIdentifier("int", TypeInfo(_identifiers["type"], false, true));
			}

	public:
		SymbolTable(
			) {
				_startGlobalScope();
			}

		const IdentifierInfo * Get(
			const char * name
			) const {
				auto it = _identifiers.find(name);
				if (it == _identifiers.cend()) return nullptr;
				return it->second;
			}

		SymbolTable & OpenScope(
			) {
				return * this;
			}

		SymbolTable & CloseScope(
			) {
				return * this;
			}

		const IdentifierInfo * Register(
			const char * name, const IdentifierHandler * identifierHandler
			) {
				return nullptr;
				//return _RegisterIdentifier(name, identifierHandler);
			}
	};

	class CompilationContext {
		SymbolTable _symbolTable;

	public:
		inline SymbolTable & GetSymbolTable(
			) {
				return _symbolTable;
			}
	};
}

#endif // !_NOVA_CONTEXT_HEADER_