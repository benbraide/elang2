#pragma once

#ifndef ELANG_LANG_LITERAL_VALUE_AST_H
#define ELANG_LANG_LITERAL_VALUE_AST_H

#include <climits>

#include "../../common/utilities.h"

#include "../../asm/asm_string_instruction_operand.h"
#include "../../asm/asm_decl_instruction.h"

#include "../../lang/lang_pointer_type_info.h"
#include "../../lang/lang_type_store.h"
#include "../../lang/lang_operand_info.h"
#include "../../lang/lang_thread_info.h"

#include "../ast.h"

namespace elang::grammar{
	enum class lang_integer_literal_suffix{
		int8,
		uint8,
		int16,
		uint16,
		int32,
		uint32,
		int64,
		uint64,
	};

	ELANG_AST_DECLARE_PAIR(lang_integer_literal_value, __int64, boost::optional<lang_integer_literal_suffix>)
	ELANG_AST_DECLARE_SINGLE(lang_float_literal_value, long double)

	ELANG_AST_DECLARE_PAIR(lang_char_literal_value, boost::optional<char>, std::string)
	ELANG_AST_DECLARE_PAIR(lang_string_literal_value, boost::optional<char>, std::string)

	ELANG_AST_DECLARE_SINGLE_VARIANT(
		lang_literal_value,
		ELANG_AST_NAME(lang_integer_literal_value),
		ELANG_AST_NAME(lang_float_literal_value),
		ELANG_AST_NAME(lang_char_literal_value),
		ELANG_AST_NAME(lang_string_literal_value)
	)

	struct lang_constant_value_ast_visitor{
		template <typename target_type, typename value_type>
		static void check_numeric_size(value_type value){
			if (value < static_cast<value_type>(std::numeric_limits<target_type>::min()))
				throw common::error::lang_number_too_small;

			if (static_cast<value_type>(std::numeric_limits<target_type>::max()) < value)
				throw common::error::lang_number_too_big;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_integer_literal_value) &ast) const{
			auto value = std::make_shared<lang::operand_info>();
			switch (ast.second.get_value_or(lang_integer_literal_suffix::int32)){
			case lang_integer_literal_suffix::int8:
				check_numeric_size<__int8>(ast.first);
				value->type = lang::type_store::int8_type;
				break;
			case lang_integer_literal_suffix::uint8:
				check_numeric_size<unsigned __int8>(ast.first);
				value->type = lang::type_store::uint8_type;
				break;
			case lang_integer_literal_suffix::int16:
				check_numeric_size<__int16>(ast.first);
				value->type = lang::type_store::int16_type;
				break;
			case lang_integer_literal_suffix::uint16:
				check_numeric_size<unsigned __int16>(ast.first);
				value->type = lang::type_store::uint16_type;
				break;
			case lang_integer_literal_suffix::uint32:
				check_numeric_size<unsigned __int32>(ast.first);
				value->type = lang::type_store::uint32_type;
				break;
			case lang_integer_literal_suffix::int64:
				value->type = lang::type_store::int64_type;
				break;
			case lang_integer_literal_suffix::uint64:
				value->type = lang::type_store::uint64_type;
				break;
			default:
				check_numeric_size<__int32>(ast.first);
				value->type = lang::type_store::int32_type;
				break;
			}

			value->is_primitive_constant = true;
			value->value = ast.first;

			return value;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_float_literal_value) &ast) const{
			auto value = std::make_shared<lang::operand_info>();

			value->type = lang::type_store::float_type;
			value->is_primitive_constant = true;
			value->value = ast.value;

			return value;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_char_literal_value) &ast) const{
			auto value = std::make_shared<lang::operand_info>();
			if (ast.first.is_initialized())
				value->type = lang::type_store::wchar_type;
			else//Narrow
				value->type = lang::type_store::char_type;

			std::string escaped_value;
			common::utils::escape_string(ast.second, escaped_value, ast.first.is_initialized());

			if (escaped_value.size() == value->type->size()){
				if (value->type->is_char())
					value->value = static_cast<__int64>(escaped_value[0]);
				else//Wide
					value->value = static_cast<__int64>(*reinterpret_cast<__int16 *>(escaped_value.data()));
			}
			else//Error
				throw common::error::lang_bad_char;

			value->is_primitive_constant = true;
		}

		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_string_literal_value) &ast) const{
			auto value = std::make_shared<lang::operand_info>();
			if (ast.first.is_initialized())
				value->type = lang::type_store::wchar_type->clone(lang::type_info::attribute_type::const_);
			else//Narrow
				value->type = lang::type_store::char_type->clone(lang::type_info::attribute_type::const_);

			std::string escaped_value;
			ast.second.push_back('\0');
			common::utils::escape_string(ast.second, escaped_value, ast.first.is_initialized());

			auto label = lang::thread_info::lbl_store.generate(lang::label_store::target_type::constant);
			std::vector<easm::instruction_operand_object::ptr_type> operands({
				std::make_shared<easm::string_instruction_operand>(std::move(escaped_value))
			});

			lang::thread_info::add(easm::section_id::rodata, label);
			lang::thread_info::add(easm::section_id::rodata, std::make_shared<easm::db_instruction>(std::move(operands)));

			value->value = &lang::thread_info::ins_table.find_label(label);
			value->type = std::make_shared<lang::pointer_type_info>(value->type, lang::type_info::attribute_type::nil);

			return value;
		}
	};
}

ELANG_AST_ADAPT_PAIR(lang_integer_literal_value)
ELANG_AST_ADAPT_SINGLE(lang_float_literal_value)
ELANG_AST_ADAPT_PAIR(lang_char_literal_value)
ELANG_AST_ADAPT_PAIR(lang_string_literal_value)

#endif /* !ELANG_LANG_LITERAL_VALUE_AST_H */
