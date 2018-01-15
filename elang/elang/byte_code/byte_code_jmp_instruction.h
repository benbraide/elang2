#pragma once

#ifndef ELANG_BYTE_CODE_JMP_INSTRUCTION_H
#define ELANG_BYTE_CODE_JMP_INSTRUCTION_H

#include "byte_code_comparison.h"
#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct jmp_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack,
			std::size_t size, comparison_info *info = nullptr){
			switch (size){
			case 1u://Byte
				return jmp<__int8>(mem_tbl, reg_tbl, stack, info);
			case 2u://Word
				return jmp<__int16>(mem_tbl, reg_tbl, stack, info);
			case 4u://Double Word
				return jmp<__int32>(mem_tbl, reg_tbl, stack, info);
			case 8u://Quad Word
				return jmp<__int64>(mem_tbl, reg_tbl, stack, info);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void jmp(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, comparison_info *info){
			auto value = operand_info::extract_source<target_type>(mem_tbl, reg_tbl);
			if (info != nullptr){//Test condition
				auto flag = '\0';
				if (reg_tbl.has_flag(memory::register_flag::less))
					flag = static_cast<char>(-1);
				else if (!reg_tbl.has_flag(memory::register_flag::zero))
					flag = static_cast<char>(1);

				if (comparison::compare(*info, flag))
					reg_tbl.instruction_pointer()->write(value);
			}
			else//Unconditional jump
				reg_tbl.instruction_pointer()->write(value);
		}
	};

	struct cjmp_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			auto iptr = reg_tbl.instruction_pointer()->read<unsigned __int64>();
			reg_tbl.instruction_pointer()->write(iptr + 1);//Update
			jmp_instruction::evaluate(mem_tbl, reg_tbl, stack, size, mem_tbl.read_bytes<comparison_info>(iptr));
		}
	};

	template <>
	struct instruction<id::jmpb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			jmp_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::jmpw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			jmp_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::jmpd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			jmp_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::jmpq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			jmp_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::cjmpb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cjmp_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::cjmpw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cjmp_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::cjmpd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cjmp_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::cjmpq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cjmp_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_JMP_INSTRUCTION_H */
