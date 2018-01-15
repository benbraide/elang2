#pragma once

#ifndef ELANG_BYTE_CODE_LEA_INSTRUCTION_H
#define ELANG_BYTE_CODE_LEA_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct lea_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return load<__int8>(mem_tbl, reg_tbl);
			case 2u://Word
				return load<__int16>(mem_tbl, reg_tbl);
			case 4u://Double Word
				return load<__int32>(mem_tbl, reg_tbl);
			case 8u://Quad Word
				return load<__int64>(mem_tbl, reg_tbl);
			case 10u://Float
				return load<long double>(mem_tbl, reg_tbl);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void load(memory::table &mem_tbl, memory::register_table &reg_tbl){
			operand_info::destination_type dest;
			operand_info::extract_destination(mem_tbl, reg_tbl, dest);

			if (mem_tbl.read_bytes<operand_info::format>(reg_tbl.instruction_pointer()->read<unsigned __int64>())->type == operand_info::type::memory){
				operand_info::destination_type src;
				operand_info::extract_destination(mem_tbl, reg_tbl, src);
				operand_info::destination_query::write(dest, static_cast<target_type>(std::get<operand_info::memory_destination>(src).address));
			}
			else//Use value
				operand_info::destination_query::write(dest, operand_info::extract_source<target_type>(mem_tbl, reg_tbl));
		}
	};

	template <>
	struct instruction<id::leab>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			lea_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::leaw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			lea_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::lead>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			lea_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::leaq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			lea_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_LEA_INSTRUCTION_H */
