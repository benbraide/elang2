#pragma once

#ifndef ELANG_BYTE_CODE_STACK_INSTRUCTION_H
#define ELANG_BYTE_CODE_STACK_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct push_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			stack.push(size, operand_info::extract_destination(ptr, base_ptr, reg_tbl));
			reg_tbl.instruction_pointer()->write(*reinterpret_cast<__int64 *>(ptr));
		}
	};

	struct pop_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			stack.pop(size, operand_info::extract_destination(ptr, base_ptr, reg_tbl));
			reg_tbl.instruction_pointer()->write(*reinterpret_cast<__int64 *>(ptr));
		}
	};

	template <>
	struct instruction<id::pushb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::pushw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::pushd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::pushq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::pushf>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::popb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::popw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::popd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::popq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::popf>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_STACK_INSTRUCTION_H */
