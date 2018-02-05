#pragma once

#ifndef ELANG_LANG_STATIC_EVALUATOR_H
#define ELANG_LANG_STATIC_EVALUATOR_H

#include "../common/operator_id.h"

#include "lang_type_store.h"
#include "lang_operand_info.h"

namespace elang::lang{
	struct static_evaluator{
		template <typename value_type>
		static void set_value(operand_info &info, value_type value, type_info::ptr_type type){
			info.type = type;
			info.value = value;
		}

		static void set_boolean_value(operand_info &info, bool value){
			set_value(info, (value ? common::constant_value::true_ : common::constant_value::false_), type_store::bool_type);
		}

		template <typename value_type>
		static void set_numeric_value(operand_info &left, operand_info &right, value_type value){
			auto left_primitive_type_id = dynamic_cast<primitive_type_info *>(left.type.get())->id();
			auto right_primitive_type_id = dynamic_cast<primitive_type_info *>(right.type.get())->id();
			set_value(left, value, ((left_primitive_type_id < right_primitive_type_id) ? right.type : left.type));
		}

		template <typename target_type>
		static target_type get(const operand_value_type &info){
			return std::visit(get_operand_value<target_type>(), info);
		}

		static unsigned __int64 get_unsigned(const operand_value_type &info){
			return static_cast<unsigned __int64>(get<__int64>(info));
		}

		template <typename target_type>
		static target_type get_non_zero(const operand_value_type &info){
			auto value = get(info);
			if (value == static_cast<target_type>(0))
				throw common::error::lang_division_by_zero;
			return value;
		}

		template <typename target_type>
		static void boolean(common::operator_id op, operand_info &left, operand_info &right){
			switch (op){
			case common::operator_id::less_or_equal:
				return set_boolean_value(left, (get<target_type>(left.value) <= get<target_type>(right.value)));
			case common::operator_id::equality:
				return set_boolean_value(left, (get<target_type>(left.value) == get<target_type>(right.value)));
			case common::operator_id::inverse_equality:
				return set_boolean_value(left, (get<target_type>(left.value) != get<target_type>(right.value)));
			case common::operator_id::greater_or_equal:
				return set_boolean_value(left, (get<target_type>(left.value) >= get<target_type>(right.value)));
			case common::operator_id::greater:
				return set_boolean_value(left, (get<target_type>(left.value) > get<target_type>(right.value)));
			default:
				break;
			}

			set_boolean_value(left, (get<target_type>(left.value) < get<target_type>(right.value)));
		}

		template <typename target_type>
		static void numeric(common::operator_id op, operand_info &left, operand_info &right, bool is_boolean){
			if (is_boolean)
				return boolean<__int64>(op, left, right);

			switch (op){
			case common::operator_id::plus:
				return set_numeric_value(left, right, (get<target_type>(left.value) + get<target_type>(right.value)));
			case common::operator_id::minus:
				return set_numeric_value(left, right, (get<target_type>(left.value) - get<target_type>(right.value)));
			case common::operator_id::times:
				return set_numeric_value(left, right, (get<target_type>(left.value) * get<target_type>(right.value)));
			case common::operator_id::divide:
				return set_numeric_value(left, right, (get<target_type>(left.value) / get_non_zero<target_type>(right.value)));
			default:
				break;
			}

			if (std::is_integral_v<target_type>){
				switch (op){
				case common::operator_id::modulus:
					return set_numeric_value(left, right, (get<__int64>(left.value) % get_non_zero<__int64>(right.value)));
				case common::operator_id::left_shift:
					return set_numeric_value(left, right, (get_unsigned(left.value) << get_unsigned(right.value)));
				case common::operator_id::right_shift:
					return set_numeric_value(left, right, (get_unsigned(left.value) >> get_unsigned(right.value)));
				case common::operator_id::bitwise_and:
					return set_numeric_value(left, right, (get_unsigned(left.value) & get_unsigned(right.value)));
				case common::operator_id::bitwise_xor:
					return set_numeric_value(left, right, (get_unsigned(left.value) ^ get_unsigned(right.value)));
				case common::operator_id::bitwise_or:
					return set_numeric_value(left, right, (get_unsigned(left.value) | get_unsigned(right.value)));
				default:
					break;
				}
			}

			throw common::error::lang_invalid_operation;
		}

		static void pointer(elang::common::operator_id op, operand_info &left, operand_info &right, bool is_boolean){
			if (is_boolean){
				if (!right.type->is_pointer())
					throw common::error::lang_invalid_operation;

				if (!left.type->is_pointer()){

				}

				return boolean<__int64>(op, left, right);
			}

			if (!right.type->is_integral())
				throw common::error::lang_invalid_operation;

			switch (op){
			case elang::common::operator_id::plus:
				return set_value(left, (get<__int64>(left.value) + (get<__int64>(right.value) * sizeof(void *))), left.type);
			case elang::common::operator_id::minus:
				return set_value(left, (get<__int64>(left.value) - (get<__int64>(right.value) * sizeof(void *))), left.type);
			default:
				break;
			}

			throw common::error::lang_invalid_operation;
		}
	};
}

#endif /* !ELANG_LANG_STATIC_EVALUATOR_H */
