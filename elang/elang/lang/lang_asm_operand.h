#pragma once

#ifndef ELANG_LANG_ASM_OPERAND_H
#define ELANG_LANG_ASM_OPERAND_H

#include <list>

#include "../asm/asm_mov_instruction.h"
#include "../asm/asm_cnvt_instruction.h"

#include "lang_static_evaluator.h"

namespace elang::lang{
	struct asm_operand{
		typedef std::list<std::shared_ptr<easm::instruction_object>> instruction_list_type;

		static easm::instruction_operand_object::ptr_type get_primitive_operand(operand_info &info, type_info &type,
			instruction_list_type &list, register_stack &reg_stack, bool convert_immediate){
			easm::instruction_operand_object::ptr_type operand;
			if (!info.is_primitive_constant){
				operand = get_asm_operand::get(info);
				if (info.type.get() != &type){//Do conversion
					auto reg = (info.type->is_float() ? reg_stack.pop_float(type.size()) : reg_stack.pop_integral(type.size()));

					info.type = type.reflect();
					info.value = reg;

					std::vector<easm::instruction_operand_object::ptr_type> operands({
						std::make_shared<easm::register_instruction_operand>(reg->value()),
						operand
					});

					list.push_back(std::make_shared<easm::cnvt_instruction>(std::move(operands)));
				}
				else//No conversion needed
					info.value = operand;
			}
			else if (type.is_integral() || type.is_null_pointer() || type.is_pointer()){
				operand = std::make_shared<easm::immediate_instruction_operand<__int64>>(
					std::visit(get_operand_value<__int64>(), info.value));
			}
			else if (type.is_float()){
				operand = std::make_shared<easm::immediate_instruction_operand<long double>>(
					std::visit(get_operand_value<long double>(), info.value));
			}
			else//Error
				throw common::error::lang_bad_operand;

			return (convert_immediate ? immediate_to_register(operand, info, list, reg_stack) : operand);
		}

		static easm::instruction_operand_object::ptr_type immediate_to_register(easm::instruction_operand_object::ptr_type value,
			operand_info &info, instruction_list_type &list, register_stack &reg_stack){
			if (dynamic_cast<easm::immediate_instruction_operand<__int64> *>(value.get()) != nullptr)
				return immediate_to_register(value, reg_stack.pop_integral(info.type->size()), info, list);

			if (dynamic_cast<easm::immediate_instruction_operand<long double> *>(value.get()) != nullptr)
				return immediate_to_register(value, reg_stack.pop_float(info.type->size()), info, list);

			return value;
		}

		static easm::instruction_operand_object::ptr_type immediate_to_register(easm::instruction_operand_object::ptr_type from,
			register_stack::item_ptr_type to, operand_info &info, instruction_list_type &list){
			std::vector<easm::instruction_operand_object::ptr_type> operands({
				std::make_shared<easm::register_instruction_operand>(to->value()),
				from
			});

			info.value = to;
			list.push_back(std::make_shared<easm::mov_instruction>(std::move(operands)));

			return operands[0];
		}
	};
}

#endif /* !ELANG_LANG_ASM_OPERAND_H */
