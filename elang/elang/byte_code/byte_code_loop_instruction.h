#pragma once

#ifndef ELANG_BYTE_CODE_LOOP_INSTRUCTION_H
#define ELANG_BYTE_CODE_LOOP_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct loop_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return loop<__int8>(mem_tbl, reg_tbl);
			case 2u://Word
				return loop<__int16>(mem_tbl, reg_tbl);
			case 4u://Double Word
				return loop<__int32>(mem_tbl, reg_tbl);
			case 8u://Quad Word
				return loop<__int64>(mem_tbl, reg_tbl);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void loop(memory::table &mem_tbl, memory::register_table &reg_tbl){
			auto reg = reg_tbl.find("rcx");
			auto value = operand_info::extract_source<target_type>(mem_tbl, reg_tbl);

			if (reg->read<unsigned __int64>() != 0u){
				reg->write(reg->read<unsigned __int64>() - 1u);
				reg_tbl.instruction_pointer()->write(value);
			}
		}
	};

	template <>
	struct instruction<id::loopb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			loop_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::loopw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			loop_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::loopd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			loop_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::loopq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			loop_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_LOOP_INSTRUCTION_H */
