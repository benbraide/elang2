#pragma once

#ifndef ELANG_BYTE_CODE_STACK_INSTRUCTION_H
#define ELANG_BYTE_CODE_STACK_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct push_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			if (static_cast<std::size_t>(stack.size() - (stack.ptr() - stack.base())) < size)
				throw common::error::stack_overflow;

			operand_info::destination_type dest;
			operand_info::extract_destination(size, mem_tbl, reg_tbl, dest);

			auto sptr = stack.ptr();
			stack.push(size);//Verify push

			operand_info::destination_query::read(dest, sptr, size);
		}
	};

	struct pop_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			if (static_cast<std::size_t>(stack.ptr() - stack.base()) < size)
				throw common::error::stack_underflow;

			operand_info::destination_type dest;
			operand_info::extract_destination(size, mem_tbl, reg_tbl, dest);

			auto sptr = stack.ptr();
			stack.pop(size);//Verify pop

			operand_info::destination_query::write(dest, sptr, size);
		}
	};

	template <>
	struct instruction<id::pushb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::pushw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::pushd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::pushq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::pushf>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			push_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::popb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::popw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::popd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::popq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::popf>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			pop_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_STACK_INSTRUCTION_H */
