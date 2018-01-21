#pragma once

#ifndef ELANG_BYTE_CODE_MOV_INSTRUCTION_H
#define ELANG_BYTE_CODE_MOV_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct mov_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return copy<__int8>(mem_tbl, reg_tbl);
			case 2u://Word
				return copy<__int16>(mem_tbl, reg_tbl);
			case 4u://Double Word
				return copy<__int32>(mem_tbl, reg_tbl);
			case 8u://Quad Word
				return copy<__int64>(mem_tbl, reg_tbl);
			case 10u://Float
				return copy<long double>(mem_tbl, reg_tbl);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void copy(memory::table &mem_tbl, memory::register_table &reg_tbl){
			operand_info::destination_type dest;
			operand_info::extract_destination(sizeof(target_type), mem_tbl, reg_tbl, dest);
			operand_info::destination_query::write(dest, operand_info::extract_source<target_type>(mem_tbl, reg_tbl));
		}
	};

	template <>
	struct instruction<id::movb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::movw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::movd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::movq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::movf>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(mem_tbl, reg_tbl, stack, 10);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_MOV_INSTRUCTION_H */
