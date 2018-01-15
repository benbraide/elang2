#pragma once

#ifndef ELANG_BYTE_CODE_CALL_INSTRUCTION_H
#define ELANG_BYTE_CODE_CALL_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct call_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return call<__int8>(mem_tbl, reg_tbl, stack);
			case 2u://Word
				return call<__int16>(mem_tbl, reg_tbl, stack);
			case 4u://Double Word
				return call<__int32>(mem_tbl, reg_tbl, stack);
			case 8u://Quad Word
				return call<__int64>(mem_tbl, reg_tbl, stack);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void call(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			auto value = operand_info::extract_source<target_type>(mem_tbl, reg_tbl);
			stack.push_value(reg_tbl.instruction_pointer()->read<unsigned __int64>());//Push return address onto stack
			reg_tbl.instruction_pointer()->write(value);//Jump to address
		}
	};

	template <>
	struct instruction<id::callb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			call_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::callw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			call_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::calld>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			call_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::callq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			call_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_CALL_INSTRUCTION_H */
