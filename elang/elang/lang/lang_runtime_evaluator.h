#pragma once

#ifndef ELANG_LANG_RUNTIME_EVALUATOR_H
#define ELANG_LANG_RUNTIME_EVALUATOR_H

#include <list>

#include "../asm/asm_arithmetic_instruction.h"
#include "../asm/asm_cmp_instruction.h"
#include "../asm/asm_set_instruction.h"

#include "lang_asm_operand.h"

namespace elang::lang{
	struct runtime_evaluator{
		typedef std::list<std::shared_ptr<easm::instruction_object>> instruction_list_type;

		template <typename value_type>
		static void set_value(operand_info &info, value_type value, type_info::ptr_type type){
			info.type = type;
			info.value = value;
			info.is_primitive_constant = false;
		}

		static operand_info *multiply_address(operand_info &info, operand_info &other, instruction_list_type &list, register_stack &reg_stack){
			other.is_primitive_constant = true;
			other.type = type_store::int64_type;
			other.value = static_cast<__int64>(sizeof(void *));
			evaluate(common::operator_id::times, info, other, false, list, reg_stack);
			return &other;
		}

		static void evaluate(common::operator_id op, operand_info &left, operand_info &right, bool is_boolean,
			instruction_list_type &list, register_stack &reg_stack){
			if (left.is_primitive_constant && right.is_primitive_constant){
				if (left.type->is_numeric() && right.type->is_numeric()){
					if (left.type->is_integral() && right.type->is_integral())
						static_evaluator::numeric<__int64>(op, left, right, is_boolean);
					else//Float
						static_evaluator::numeric<long double>(op, left, right, is_boolean);
				}
				else if (left.type->is_pointer() || left.type->is_null_pointer() || right.type->is_pointer() || right.type->is_null_pointer())
					static_evaluator::pointer(op, left, right, is_boolean);
				else//Error
					throw common::error::lang_invalid_operation;

				return;//Handled
			}
			else if (left.type->is_numeric() && right.type->is_numeric())
				numeric(op, left, right, is_boolean, list, reg_stack);
			else if (left.type->is_pointer() || left.type->is_null_pointer() || right.type->is_pointer() || right.type->is_null_pointer())
				pointer(op, left, right, is_boolean, list, reg_stack);
			else//Error
				throw common::error::lang_invalid_operation;
		}

		static void boolean(common::operator_id op, operand_info &left, operand_info &right, bool is_numeric,
			instruction_list_type &list, register_stack &reg_stack){
			auto type = left.type;
			if (is_numeric && dynamic_cast<primitive_type_info *>(type.get())->id() < dynamic_cast<primitive_type_info *>(right.type.get())->id())
				type = right.type;//Use higher type

			std::vector<easm::instruction_operand_object::ptr_type> operands({
				asm_operand::get_primitive_operand(left, *type, list, reg_stack, true),
				asm_operand::get_primitive_operand(right, *type, list, reg_stack, false)
			});

			right.value = 0i64;
			list.push_back(std::make_shared<easm::cmp_instruction>(std::move(operands)));

			auto reg = reg_stack.pop_integral(sizeof(char));
			operands.push_back(std::make_shared<easm::register_instruction_operand>(reg->value()));

			switch (op){
			case common::operator_id::less_or_equal:
				list.push_back(std::make_shared<easm::setle_instruction>(std::move(operands)));
				break;
			case common::operator_id::equality:
				list.push_back(std::make_shared<easm::sete_instruction>(std::move(operands)));
				break;
			case common::operator_id::inverse_equality:
				list.push_back(std::make_shared<easm::setne_instruction>(std::move(operands)));
				break;
			case common::operator_id::greater_or_equal:
				list.push_back(std::make_shared<easm::setge_instruction>(std::move(operands)));
				break;
			case common::operator_id::greater:
				list.push_back(std::make_shared<easm::setg_instruction>(std::move(operands)));
				break;
			default:
				list.push_back(std::make_shared<easm::setl_instruction>(std::move(operands)));
				break;
			}

			set_value(left, reg, type_store::bool_type);
		}

		static void numeric(common::operator_id op, operand_info &left, operand_info &right, bool is_boolean,
			instruction_list_type &list, register_stack &reg_stack){
			if (is_boolean)
				return boolean(op, left, right, true, list, reg_stack);

			auto type = left.type;
			if (dynamic_cast<primitive_type_info *>(type.get())->id() < dynamic_cast<primitive_type_info *>(right.type.get())->id())
				type = right.type;//Use higher type

			std::vector<easm::instruction_operand_object::ptr_type> operands({
				asm_operand::get_primitive_operand(left, *type, list, reg_stack, true),
				asm_operand::get_primitive_operand(right, *type, list, reg_stack, false)
			});

			auto handled = true;
			switch (op){
			case common::operator_id::plus:
				list.push_back(std::make_shared<easm::add_instruction>(std::move(operands)));
				break;
			case common::operator_id::minus:
				list.push_back(std::make_shared<easm::sub_instruction>(std::move(operands)));
				break;
			case common::operator_id::times:
				list.push_back(std::make_shared<easm::mult_instruction>(std::move(operands)));
				break;
			case common::operator_id::divide:
				list.push_back(std::make_shared<easm::div_instruction>(std::move(operands)));
				break;
			default:
				handled = false;
				break;
			}

			if (!handled && type->is_integral()){
				handled = true;
				switch (op){
				case common::operator_id::modulus:
					list.push_back(std::make_shared<easm::mod_instruction>(std::move(operands)));
					break;
				case common::operator_id::left_shift:
					list.push_back(std::make_shared<easm::sal_instruction>(std::move(operands)));
					break;
				case common::operator_id::right_shift:
					list.push_back(std::make_shared<easm::sar_instruction>(std::move(operands)));
					break;
				case common::operator_id::bitwise_and:
					list.push_back(std::make_shared<easm::and_instruction>(std::move(operands)));
					break;
				case common::operator_id::bitwise_xor:
					list.push_back(std::make_shared<easm::xor_instruction>(std::move(operands)));
					break;
				case common::operator_id::bitwise_or:
					list.push_back(std::make_shared<easm::or_instruction>(std::move(operands)));
					break;
				default:
					handled = false;
					break;
				}
			}

			if (!handled)
				throw common::error::lang_invalid_operation;
		}

		static void pointer(elang::common::operator_id op, operand_info &left, operand_info &right, bool is_boolean,
			instruction_list_type &list, register_stack &reg_stack){
			if (is_boolean){
				if (left.type->score(*right.type) == ELANG_TYPE_INFO_MIN_SCORE && right.type->score(*left.type) == ELANG_TYPE_INFO_MIN_SCORE)
					throw common::error::lang_invalid_operation;//Incompatible types
				return boolean(op, left, right, false, list, reg_stack);
			}

			if (!left.type->is_integral() && !right.type->is_integral())
				throw common::error::lang_invalid_operation;

			if (left.type->is_null_pointer() || right.type->is_null_pointer())
				throw common::error::lang_invalid_operation;

			if (op == elang::common::operator_id::minus && left.type->is_integral())
				throw common::error::lang_invalid_operation;

			auto type = left.type;
			operand_info other, *left_ptr, *right_ptr;

			if (left.type->is_integral()){
				type = right.type;
				left_ptr = multiply_address(left, other, list, reg_stack);
				right_ptr = &right;
			}
			else{//Right is integral
				left_ptr = &left;
				right_ptr = multiply_address(right, other, list, reg_stack);
			}

			std::vector<easm::instruction_operand_object::ptr_type> operands({
				asm_operand::get_primitive_operand(*left_ptr, *type, list, reg_stack, true),
				asm_operand::get_primitive_operand(*right_ptr, *type, list, reg_stack, false)
			});

			switch (op){
			case elang::common::operator_id::plus:
				list.push_back(std::make_shared<easm::add_instruction>(std::move(operands)));
				break;
			case elang::common::operator_id::minus:
				list.push_back(std::make_shared<easm::sub_instruction>(std::move(operands)));
				break;
			default:
				throw common::error::lang_invalid_operation;
				break;
			}
		}
	};
}

#endif /* !ELANG_LANG_RUNTIME_EVALUATOR_H */
