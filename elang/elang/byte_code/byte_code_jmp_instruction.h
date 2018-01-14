#pragma once

#ifndef ELANG_BYTE_CODE_JMP_INSTRUCTION_H
#define ELANG_BYTE_CODE_JMP_INSTRUCTION_H

#include "byte_code_comparison.h"
#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct jmp_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack,
			std::size_t size, comparison_info *info = nullptr){
			switch (size){
			case 1u://Byte
				return jmp<__int8>(ptr, base_ptr, reg_tbl, stack, info);
			case 2u://Word
				return jmp<__int16>(ptr, base_ptr, reg_tbl, stack, info);
			case 4u://Double Word
				return jmp<__int32>(ptr, base_ptr, reg_tbl, stack, info);
			case 8u://Quad Word
				return jmp<__int64>(ptr, base_ptr, reg_tbl, stack, info);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void jmp(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack, comparison_info *info){
			auto value = operand_info::extract_source<target_type>(ptr, base_ptr, reg_tbl);
			if (info != nullptr){//Test condition
				auto flag = '\0';
				if (reg_tbl.has_flag(memory::register_flag::less))
					flag = static_cast<char>(-1);
				else if (!reg_tbl.has_flag(memory::register_flag::zero))
					flag = static_cast<char>(1);

				if (comparison::compare(*info, flag))
					reg_tbl.instruction_pointer()->write(value);
				else//Failed test
					reg_tbl.instruction_pointer()->write(reinterpret_cast<__int64>(ptr));
			}
			else//Unconditional jump
				reg_tbl.instruction_pointer()->write(value);
		}
	};

	struct cjmp_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			jmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, size, reinterpret_cast<comparison_info *>(ptr++));
		}
	};

	template <>
	struct instruction<id::jmpb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			jmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::jmpw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			jmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::jmpd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			jmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::jmpq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			jmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::cjmpb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cjmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::cjmpw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cjmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::cjmpd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cjmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::cjmpq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			cjmp_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_JMP_INSTRUCTION_H */
