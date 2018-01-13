#pragma once

#ifndef ELANG_BYTE_CODE_MOV_INSTRUCTION_H
#define ELANG_BYTE_CODE_MOV_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct mov_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return copy<__int8>(ptr, base_ptr, reg_tbl);
			case 2u://Word
				return copy<__int16>(ptr, base_ptr, reg_tbl);
			case 4u://Double Word
				return copy<__int32>(ptr, base_ptr, reg_tbl);
			case 8u://Quad Word
				return copy<__int64>(ptr, base_ptr, reg_tbl);
			case 10u://Float
				return copy<long double>(ptr, base_ptr, reg_tbl);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void copy(char *&ptr, char *base_ptr, memory::register_table &reg_tbl){
			auto destination = operand_info::extract_destination(ptr, base_ptr, reg_tbl);
			auto value = operand_info::extract_source<target_type>(ptr, base_ptr, reg_tbl);
			memcpy(destination, &value, sizeof(target_type));
			reg_tbl.instruction_pointer()->write(*reinterpret_cast<__int64 *>(ptr));
		}
	};

	template <>
	struct instruction<id::movb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::movw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::movd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::movq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::movf>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			mov_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 10);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_MOV_INSTRUCTION_H */
