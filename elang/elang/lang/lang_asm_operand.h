#pragma once

#ifndef ELANG_LANG_ASM_OPERAND_H
#define ELANG_LANG_ASM_OPERAND_H

#include <list>

#include "../asm/asm_string_instruction_operand.h"
#include "../asm/asm_mov_instruction.h"
#include "../asm/asm_cnvt_instruction.h"

#include "lang_thread_info.h"
#include "lang_static_evaluator.h"

namespace elang::lang{
	struct asm_operand{
		typedef std::list<std::shared_ptr<easm::instruction_object>> instruction_list_type;

		static easm::instruction_operand_object::ptr_type get_primitive_operand(operand_info &info, type_info &type, bool convert_immediate){
			easm::instruction_operand_object::ptr_type operand;
			if (!info.is_primitive_constant){
				operand = get_asm_operand::get(info);
				if (info.type.get() != &type){//Do conversion
					auto reg = (info.type->is_float() ? thread_info::reg_stack.pop_float(type.size()) :
						thread_info::reg_stack.pop_integral(type.size()));

					info.type = type.reflect();
					info.value = reg;

					std::vector<easm::instruction_operand_object::ptr_type> operands({
						std::make_shared<easm::register_instruction_operand>(reg->value()),
						operand
					});

					thread_info::add(easm::section_id::text, std::make_shared<easm::cnvt_instruction>(std::move(operands)));
				}
				else//No conversion needed
					info.value = operand;
			}
			else if (type.is_integral() || type.is_null_pointer() || type.is_pointer() || type.is_wchar()){
				operand = std::make_shared<easm::immediate_instruction_operand<__int64>>(
					std::visit(get_operand_value<__int64>(), info.value));
			}
			else if (type.is_char()){
				std::string value(1, std::get<char>(info.value));
				operand = std::make_shared<easm::string_instruction_operand>(std::move(value));
			}
			else if (type.is_float()){
				operand = std::make_shared<easm::immediate_instruction_operand<long double>>(
					std::visit(get_operand_value<long double>(), info.value));
			}
			else//Error
				throw common::error::lang_bad_operand;

			return (convert_immediate ? immediate_to_register(operand, info) : operand);
		}

		static easm::instruction_operand_object::ptr_type immediate_to_register(easm::instruction_operand_object::ptr_type value, operand_info &info){
			if (dynamic_cast<easm::immediate_instruction_operand<__int64> *>(value.get()) != nullptr)
				return immediate_to_register(value, thread_info::reg_stack.pop_integral(info.type->size()), info);

			if (dynamic_cast<easm::immediate_instruction_operand<long double> *>(value.get()) != nullptr)
				return immediate_to_register(value, thread_info::reg_stack.pop_float(info.type->size()), info);

			return value;
		}

		static easm::instruction_operand_object::ptr_type immediate_to_register(easm::instruction_operand_object::ptr_type from,
			register_stack::item_ptr_type to, operand_info &info){
			std::vector<easm::instruction_operand_object::ptr_type> operands({
				std::make_shared<easm::register_instruction_operand>(to->value()),
				from
			});

			info.value = to;
			thread_info::add(easm::section_id::text, std::make_shared<easm::mov_instruction>(std::move(operands)));

			return operands[0];
		}
	};
}

#endif /* !ELANG_LANG_ASM_OPERAND_H */
