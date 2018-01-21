#pragma once

#ifndef ELANG_BYTE_CODE_SET_INSTRUCTION_H
#define ELANG_BYTE_CODE_SET_INSTRUCTION_H

#include "byte_code_comparison.h"
#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct set_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return set<__int8>(mem_tbl, reg_tbl, stack);
			case 2u://Word
				return set<__int16>(mem_tbl, reg_tbl, stack);
			case 4u://Double Word
				return set<__int32>(mem_tbl, reg_tbl, stack);
			case 8u://Quad Word
				return set<__int64>(mem_tbl, reg_tbl, stack);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void set(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			auto iptr = reg_tbl.instruction_pointer()->read<unsigned __int64>();
			reg_tbl.instruction_pointer()->write(iptr + sizeof(comparison_info));//Update
			auto info = mem_tbl.read_bytes<comparison_info>(iptr);

			operand_info::destination_type dest;
			operand_info::extract_destination(sizeof(target_type), mem_tbl, reg_tbl, dest);

			auto flag = '\0';
			if (reg_tbl.has_flag(memory::register_flag::less))
				flag = static_cast<char>(-1);
			else if (!reg_tbl.has_flag(memory::register_flag::zero))
				flag = static_cast<char>(1);

			auto value = (comparison::compare(*info, flag) ? static_cast<target_type>(1) : static_cast<target_type>(0));
			operand_info::destination_query::write(dest, value);
		}
	};

	template <>
	struct instruction<id::setb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			set_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::setw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			set_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::setd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			set_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::setq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			set_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_SET_INSTRUCTION_H */
