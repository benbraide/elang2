#pragma once

#ifndef ELANG_BYTE_CODE_CNVT_INSTRUCTION_H
#define ELANG_BYTE_CODE_CNVT_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	struct cnvt_instruction{
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

			operand_info::destination_type src;
			operand_info::extract_destination(sizeof(target_type), mem_tbl, reg_tbl, src);

			if (!std::holds_alternative<memory::memory_register *>(src))
				throw common::error::byte_code_bad_source_operand_type;

			auto reg = std::get<memory::memory_register *>(src);
			if (!reg->is_floating_point()){
				switch (reg->size()){
				case 1u://Byte
					operand_info::destination_query::write(dest, static_cast<target_type>(reg->read<__int8>()));
					break;
				case 2u://Word
					operand_info::destination_query::write(dest, static_cast<target_type>(reg->read<__int16>()));
					break;
				case 4u://Double Word
					operand_info::destination_query::write(dest, static_cast<target_type>(reg->read<__int32>()));
					break;
				case 8u://Quad Word
					operand_info::destination_query::write(dest, static_cast<target_type>(reg->read<__int64>()));
					break;
				default:
					throw common::error::byte_code_bad_operand_size;
					break;
				}
			}
			else//Float
				operand_info::destination_query::write(dest, static_cast<target_type>(reg->read<float>()));
		}
	};

	template <>
	struct instruction<id::cnvtb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cnvt_instruction::evaluate(mem_tbl, reg_tbl, stack, 1);
		}
	};

	template <>
	struct instruction<id::cnvtw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cnvt_instruction::evaluate(mem_tbl, reg_tbl, stack, 2);
		}
	};

	template <>
	struct instruction<id::cnvtd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cnvt_instruction::evaluate(mem_tbl, reg_tbl, stack, 4);
		}
	};

	template <>
	struct instruction<id::cnvtq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cnvt_instruction::evaluate(mem_tbl, reg_tbl, stack, 8);
		}
	};

	template <>
	struct instruction<id::cnvtf>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			cnvt_instruction::evaluate(mem_tbl, reg_tbl, stack, 10);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_CNVT_INSTRUCTION_H */
