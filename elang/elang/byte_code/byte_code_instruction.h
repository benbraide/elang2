#pragma once

#ifndef ELANG_BYTE_CODE_INSTRUCTION_H
#define ELANG_BYTE_CODE_INSTRUCTION_H

#include "../memory/memory_stack.h"

#include "byte_code_id.h"
#include "byte_code_operand_info.h"

namespace elang::byte_code{
	template <id id>
	struct instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			throw common::error::byte_code_unknown_instruction;
		}
	};

	template <>
	struct instruction<id::nop>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){}
	};

	template <>
	struct instruction<id::enter>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			stack.push(*reg_tbl.base_pointer());
			reg_tbl.base_pointer()->write(reg_tbl.stack_pointer()->data(), 8);

			auto size = operand_info::extract_source<__int64>(ptr, base_ptr, reg_tbl);
			if (size > 0)//Allocate space on stack
				reg_tbl.stack_pointer()->write(reinterpret_cast<__int64>(stack.push(size)));

			reg_tbl.instruction_pointer()->write(reinterpret_cast<__int64>(ptr));
		}
	};

	template <>
	struct instruction<id::leave>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			reg_tbl.stack_pointer()->write(reg_tbl.base_pointer()->data(), 8);
			stack.pop(*reg_tbl.base_pointer());
			reg_tbl.instruction_pointer()->write(reinterpret_cast<__int64>(ptr));
		}
	};

	template <>
	struct instruction<id::ret>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			stack.pop(*reg_tbl.instruction_pointer());
		}
	};
}

#endif /* !ELANG_BYTE_CODE_INSTRUCTION_H */
