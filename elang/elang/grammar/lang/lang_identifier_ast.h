#pragma once

#ifndef ELANG_LANG_IDENTIFIER_AST_H
#define ELANG_LANG_IDENTIFIER_AST_H

#include "../../common/raii.h"
#include "../../common/operator_id.h"

#include "../../lang/lang_operand_info.h"
#include "../../lang/lang_thread_info.h"

#include "../ast.h"

namespace elang::grammar{
	ELANG_AST_DECLARE_SINGLE(lang_global_qualified_identifier, ELANG_AST_NAME(elang_identifier))

	using qualified_identifier_variant = boost::variant<ELANG_AST_NAME(elang_identifier), ELANG_AST_NAME(lang_global_qualified_identifier)>;
	ELANG_AST_DECLARE_PAIR(lang_qualified_identifier, qualified_identifier_variant, std::vector<ELANG_AST_NAME(elang_identifier)>)

	ELANG_AST_DECLARE_SINGLE_VARIANT(
		lang_identifier,
		ELANG_AST_NAME(elang_identifier),
		ELANG_AST_NAME(lang_global_qualified_identifier),
		ELANG_AST_NAME(lang_qualified_identifier)
	)

	ELANG_AST_DECLARE_SINGLE(lang_operator_identifier, ELANG_AST_NAME(lang_identifier))
	ELANG_AST_DECLARE_SINGLE(lang_operator_symbol, common::operator_id)

	using qualified_operator_variant = boost::variant<ELANG_AST_NAME(lang_operator_identifier), ELANG_AST_NAME(lang_operator_symbol)>;
	ELANG_AST_DECLARE_PAIR(lang_qualified_operator, ELANG_AST_NAME(lang_identifier), qualified_operator_variant)

	struct lang_ast_table_finder{
		template <typename ast_type>
		lang::symbol_table *operator ()(ast_type &ast){
			return nullptr;
		}

		lang::symbol_table *operator ()(ELANG_AST_NAME(elang_identifier) &ast) const{
			auto context = ((lang::thread_info::search_context == nullptr) ? lang::thread_info::context : lang::thread_info::search_context);
			if (context == nullptr)//Use global context
				context = &lang::thread_info::sym_table;

			for (; context != nullptr; context = context->parent()){
				auto entry = context->find_table(ast.value);
				if (entry != nullptr || lang::thread_info::search_context != nullptr)//Found
					return entry;
			}

			return nullptr;
		}

		lang::symbol_table *operator ()(ELANG_AST_NAME(lang_global_qualified_identifier) &ast) const{
			common::raii_value<lang::symbol_table *> context(lang::thread_info::search_context, &lang::thread_info::sym_table);
			return operator ()(ast.value);
		}

		lang::symbol_table *operator ()(ELANG_AST_NAME(lang_qualified_identifier) &ast) const{
			auto value = boost::apply_visitor(*this, ast.first);
			if (value == nullptr)//Not found
				return nullptr;

			common::raii_value<lang::symbol_table *> context(lang::thread_info::search_context, value);
			for (auto &id_ast : ast.second){
				if ((value = operator()(id_ast)) == nullptr)
					break;
			}

			return value;
		}

		lang::symbol_table *operator ()(ELANG_AST_NAME(lang_identifier) &ast) const{
			return boost::apply_visitor(*this, ast.value);
		}
	};

	struct lang_identifier_ast_visitor{
		static lang::symbol_table::entry_info *find(const std::string &name){
			auto context = ((lang::thread_info::search_context == nullptr) ? lang::thread_info::context : lang::thread_info::search_context);
			if (context == nullptr)//Use global context
				context = &lang::thread_info::sym_table;

			for (; context != nullptr; context = context->parent()){
				auto entry = context->find(name);
				if (entry != nullptr || lang::thread_info::search_context != nullptr)//Found
					return entry;
			}

			return nullptr;
		}

		static lang::symbol_table::variable_entry_info *find_variable(const std::string &name){
			auto context = ((lang::thread_info::search_context == nullptr) ? lang::thread_info::context : lang::thread_info::search_context);
			if (context == nullptr)//Use global context
				context = &lang::thread_info::sym_table;

			for (; context != nullptr; context = context->parent()){
				auto entry = context->find_variable(name);
				if (entry != nullptr || lang::thread_info::search_context != nullptr)//Found
					return entry;
			}

			return nullptr;
		}

		template <typename key_type>
		static lang::symbol_table::operator_list_entry_info *find_operator(key_type key){
			auto context = ((lang::thread_info::search_context == nullptr) ? lang::thread_info::context : lang::thread_info::search_context);
			if (context == nullptr)//Use global context
				context = &lang::thread_info::sym_table;

			for (; context != nullptr; context = context->parent()){
				auto entry = context->find_operator(key);
				if (entry != nullptr || lang::thread_info::search_context != nullptr)//Found
					return entry;
			}

			return nullptr;
		}

		static lang::symbol_table *find_table(const std::string &name){
			auto context = ((lang::thread_info::search_context == nullptr) ? lang::thread_info::context : lang::thread_info::search_context);
			if (context == nullptr)//Use global context
				context = &lang::thread_info::sym_table;

			for (; context != nullptr; context = context->parent()){
				auto entry = context->find_table(name);
				if (entry != nullptr || lang::thread_info::search_context != nullptr)//Found
					return entry;
			}

			return nullptr;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(elang_identifier) &ast) const{
			auto entry = find_variable(ast.value);
			if (entry == nullptr)
				throw common::error::lang_symbol_not_found;

			auto value = std::make_shared<lang::operand_info>();
			value->type = entry->type;
			value->value = entry;

			return value;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_global_qualified_identifier) &ast) const{
			common::raii_value<lang::symbol_table *> context(lang::thread_info::search_context, &lang::thread_info::sym_table);
			return operator ()(ast.value);
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_qualified_identifier) &ast) const{
			return nullptr;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_identifier) &ast) const{
			return boost::apply_visitor(*this, ast.value);
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_operator_identifier) &ast) const{
			return nullptr;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_operator_symbol) &ast) const{
			return nullptr;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_qualified_operator) &ast) const{
			auto table = boost::apply_visitor(lang_ast_table_finder(), ast.first.value);
			if (table == nullptr)
				throw common::error::lang_symbol_not_found;

			common::raii_value<lang::symbol_table *> context(lang::thread_info::search_context, table);
			return boost::apply_visitor(*this, ast.second);
		}
	};
}

ELANG_AST_ADAPT_SINGLE(lang_global_qualified_identifier)
ELANG_AST_ADAPT_PAIR(lang_qualified_identifier)

ELANG_AST_ADAPT_SINGLE(lang_identifier)

ELANG_AST_ADAPT_SINGLE(lang_operator_identifier)
ELANG_AST_ADAPT_SINGLE(lang_operator_symbol)

ELANG_AST_ADAPT_PAIR(lang_qualified_operator)

#endif /* !ELANG_LANG_IDENTIFIER_AST_H */
