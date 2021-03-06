#pragma once

#ifndef ELANG_BYTE_CODE_TEST_INSTRUCTION_H
#define ELANG_BYTE_CODE_TEST_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct test_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack, std::size_t size){
			switch (size){
			case 1u://Byte
				return test<unsigned __int8>(mem_tbl, reg_tbl);
			case 2u://Word
				return test<unsigned __int16>(mem_tbl, reg_tbl);
			case 4u://Double Word
				return test<unsigned __int32>(mem_tbl, reg_tbl);
			case 8u://Quad Word
				return test<unsigned __int64>(mem_tbl, reg_tbl);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void test(memory::table &mem_tbl, memory::register_table &reg_tbl){
			auto left = operand_info::extract_source<target_type>(mem_tbl, reg_tbl);
			auto right = operand_info::extract_source<target_type>(mem_tbl, reg_tbl);

			reg_tbl.clear_flag(memory::register_flag::less);
			if ((left & right) == static_cast<target_type>(0))
				reg_tbl.set_flag(memory::register_flag::zero);
			else//Not zero
				reg_tbl.clear_flag(memory::register_flag::zero);
		}
	};

	template <>
	struct instruction<id::testb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			test_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::testw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			test_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::testd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			test_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::testq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			test_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_TEST_INSTRUCTION_H */
