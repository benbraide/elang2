#pragma once

#ifndef ELANG_LANG_OPERAND_INFO_H
#define ELANG_LANG_OPERAND_INFO_H

#include <variant>

#include "../common/constant_value.h"

#include "../memory/memory_register.h"

#include "../asm/asm_immediate_instruction_operand.h"
#include "../asm/asm_register_instruction_operand.h"
#include "../asm/asm_label_instruction_operand.h"
#include "../asm/asm_memory_instruction_operand.h"
#include "../asm/asm_offset_instruction_operand.h"

#include "lang_primitive_type_info.h"
#include "lang_symbol_table.h"
#include "lang_register_stack.h"

namespace elang::lang{
	using memory_operand_value_start_type = std::variant<
		unsigned __int64 **,
		register_stack::item_ptr_type
	>;

	struct memory_operand_value_info{
		memory_operand_value_start_type start;
		__int64 offset;
	};

	using operand_value_type = std::variant<
		common::constant_value,
		char,
		__int64,
		long double,
		unsigned __int64 **,
		memory_operand_value_info,
		register_stack::item_ptr_type,
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

		__int64 operator ()(char value) const{
			return static_cast<__int64>(value);
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

	struct get_asm_operand{
		easm::instruction_operand_object::ptr_type operator ()(common::constant_value value) const{
			switch (value){
			case elang::common::constant_value::false_:
				return std::make_shared<easm::immediate_instruction_operand<__int64>>(0);
			case elang::common::constant_value::true_:
				return std::make_shared<easm::immediate_instruction_operand<__int64>>(1);
			case elang::common::constant_value::indeterminate:
				return std::make_shared<easm::immediate_instruction_operand<__int64>>(-1);
			case elang::common::constant_value::nullptr_:
				return std::make_shared<easm::immediate_instruction_operand<__int64>>(0);
			default:
				break;
			}

			throw common::error::lang_bad_operand;

		}

		easm::instruction_operand_object::ptr_type operator ()(__int64 value) const{
			return std::make_shared<easm::immediate_instruction_operand<__int64>>(value);
		}

		easm::instruction_operand_object::ptr_type operator ()(long double value) const{
			return std::make_shared<easm::immediate_instruction_operand<long double>>(value);
		}

		easm::instruction_operand_object::ptr_type operator ()(unsigned __int64 **value) const{
			return std::make_shared<easm::label_instruction_operand>(*value);
		}

		easm::instruction_operand_object::ptr_type operator ()(const memory_operand_value_info &value) const{
			std::vector<easm::offset_instruction_operand::item_info> offset_list;
			offset_list.reserve(2);

			offset_list.push_back(easm::offset_instruction_operand::item_info{
				byte_code::operand_info::offset_op_type::add,
				std::visit(*this, value.start)
			});

			if (value.offset != 0){//Add offset
				offset_list.push_back(easm::offset_instruction_operand::item_info{
					byte_code::operand_info::offset_op_type::add,
					std::make_shared<easm::immediate_instruction_operand<__int64>>(value.offset)
				});
			}

			auto expr = std::make_shared<easm::offset_instruction_operand>(std::move(offset_list));
			return std::make_shared<easm::memory_instruction_operand>(expr);
		}

		easm::instruction_operand_object::ptr_type operator ()(register_stack::item_ptr_type value) const{
			return std::make_shared<easm::register_instruction_operand>(value->value());
		}

		easm::instruction_operand_object::ptr_type operator ()(easm::instruction_operand_object::ptr_type value) const{
			return value;
		}

		static easm::instruction_operand_object::ptr_type get(const operand_info &info){
			auto value = std::visit(get_asm_operand(), info.value);
			if (std::holds_alternative<memory_operand_value_info>(info.value))
				dynamic_cast<easm::memory_instruction_operand *>(value.get())->set_size(info.type->size());
			return value;
		}
	};
}

#endif /* !ELANG_LANG_OPERAND_INFO_H */
