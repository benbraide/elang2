#pragma once

#ifndef ELANG_BYTE_CODE_CMP_INSTRUCTION_H
#define ELANG_BYTE_CODE_CMP_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct cmp_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return compare<__int8>(ptr, base_ptr, reg_tbl);
			case 2u://Word
				return compare<__int16>(ptr, base_ptr, reg_tbl);
			case 4u://Double Word
				return compare<__int32>(ptr, base_ptr, reg_tbl);
			case 8u://Quad Word
				return compare<__int64>(ptr, base_ptr, reg_tbl);
			case 10u://Float
				return compare<long double>(ptr, base_ptr, reg_tbl);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void compare(char *&ptr, char *base_ptr, memory::register_table &reg_tbl){
			auto left = operand_info::extract_source<target_type>(ptr, base_ptr, reg_tbl);
			auto right = operand_info::extract_source<target_type>(ptr, base_ptr, reg_tbl);

			auto diff = static_cast<target_type>(left - right);
			if (diff != static_cast<target_type>(0)){
				reg_tbl.clear_flag(memory::register_flag::zero);
				if (diff < static_cast<target_type>(0))
					reg_tbl.set_flag(memory::register_flag::less);
			}
			else{//Equal
				reg_tbl.clear_flag(memory::register_flag::less);
				reg_tbl.set_flag(memory::register_flag::zero);
			}

			reg_tbl.instruction_pointer()->write(*reinterpret_cast<__int64 *>(ptr));
		}
	};

	template <>
	struct instruction<id::cmpb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::cmpw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::cmpd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::cmpq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::cmpf>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 10);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_CMP_INSTRUCTION_H */
