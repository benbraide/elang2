#pragma once

#ifndef ELANG_BYTE_CODE_LEA_INSTRUCTION_H
#define ELANG_BYTE_CODE_LEA_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct lea_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return load<__int8>(ptr, base_ptr, reg_tbl);
			case 2u://Word
				return load<__int16>(ptr, base_ptr, reg_tbl);
			case 4u://Double Word
				return load<__int32>(ptr, base_ptr, reg_tbl);
			case 8u://Quad Word
				return load<__int64>(ptr, base_ptr, reg_tbl);
			case 10u://Float
				return load<long double>(ptr, base_ptr, reg_tbl);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void load(char *&ptr, char *base_ptr, memory::register_table &reg_tbl){
			auto destination = operand_info::extract_destination(ptr, base_ptr, reg_tbl);

			target_type value;
			if (reinterpret_cast<operand_info::format *>(ptr)->type == operand_info::type::memory)
				value = static_cast<target_type>(operand_info::extract_destination(ptr, base_ptr, reg_tbl) - base_ptr);
			else//Use value
				value = operand_info::extract_source<target_type>(ptr, base_ptr, reg_tbl);

			memcpy(destination, &value, sizeof(target_type));
			reg_tbl.instruction_pointer()->write(reinterpret_cast<__int64>(ptr));
		}
	};

	template <>
	struct instruction<id::leab>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			lea_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::leaw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			lea_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::lead>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			lea_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::leaq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			lea_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_LEA_INSTRUCTION_H */
