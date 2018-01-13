#pragma once

#ifndef ELANG_BYTE_CODE_SET_INSTRUCTION_H
#define ELANG_BYTE_CODE_SET_INSTRUCTION_H

#include "byte_code_comparison.h"
#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct set_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return set<__int8>(ptr, base_ptr, reg_tbl, stack);
			case 2u://Word
				return set<__int16>(ptr, base_ptr, reg_tbl, stack);
			case 4u://Double Word
				return set<__int32>(ptr, base_ptr, reg_tbl, stack);
			case 8u://Quad Word
				return set<__int64>(ptr, base_ptr, reg_tbl, stack);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void set(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			auto destination = operand_info::extract_destination(ptr, base_ptr, reg_tbl);
			auto flag = '\0';
			if (reg_tbl.has_flag(memory::register_flag::less))
				flag = static_cast<char>(-1);
			else if (!reg_tbl.has_flag(memory::register_flag::zero))
				flag = static_cast<char>(1);

			auto value = ((comparison::compare(*reinterpret_cast<comparison_info *>(ptr++), flag)) ?
				static_cast<target_type>(1) : static_cast<target_type>(0));

			memcpy(destination, &value, sizeof(target_type));
			reg_tbl.instruction_pointer()->write(*reinterpret_cast<__int64 *>(ptr));
		}
	};

	template <>
	struct instruction<id::setb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			set_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::setw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			set_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::setd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			set_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::setq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			set_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_SET_INSTRUCTION_H */
