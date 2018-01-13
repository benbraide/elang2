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
	struct instruction<id::ret>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			stack.pop(*reg_tbl.instruction_pointer());
		}
	};
}

#endif /* !ELANG_BYTE_CODE_INSTRUCTION_H */
