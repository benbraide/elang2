#pragma once

#ifndef ELANG_LANG_OPERAND_INFO_H
#define ELANG_LANG_OPERAND_INFO_H

#include <variant>
#include <optional>

#include "../common/constant_value.h"

#include "../memory/memory_register.h"

#include "../asm/asm_instruction_operand_object.h"

#include "lang_primitive_type_info.h"
#include "lang_symbol_table.h"

namespace elang::lang{
	struct memory_operand_value_info{
		std::optional<std::string> label;
		__int64 offset;
	};

	using operand_value_type = std::variant<
		common::constant_value,
		__int64,
		long double,
		std::string,
		memory_operand_value_info,
		memory::memory_register *,
		easm::instruction_operand_object::ptr_type
	>;

	struct operand_info{
		operand_value_type value;
		type_info::ptr_type type;
		bool is_primitive_constant;
	};

	template <class target_type>
	struct get_operand_value;

	template <>
	struct get_operand_value<__int64>{
		__int64 operator ()(common::constant_value value) const{
			switch (value){
			case elang::common::constant_value::false_:
				return 0;
			case elang::common::constant_value::true_:
				return 1;
			case elang::common::constant_value::indeterminate:
				return -1;
			case elang::common::constant_value::nullptr_:
				return 0;
			default:
				break;
			}

			throw common::error::lang_bad_operand;

		}

		__int64 operator ()(__int64 value) const{
			return value;
		}

		__int64 operator ()(long double value) const{
			return static_cast<__int64>(value);
		}

		template <typename value_type>
		__int64 operator ()(const value_type &value) const{
			throw common::error::lang_bad_operand;
		}
	};

	template <>
	struct get_operand_value<long double>{
		long double operator ()(__int64 value) const{
			return static_cast<long double>(value);
		}

		long double operator ()(long double value) const{
			return value;
		}

		template <typename value_type>
		long double operator ()(const value_type &value) const{
			throw common::error::lang_bad_operand;
		}
	};
}

#endif /* !ELANG_LANG_OPERAND_INFO_H */
