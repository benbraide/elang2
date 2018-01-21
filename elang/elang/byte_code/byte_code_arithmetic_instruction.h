#pragma once

#ifndef ELANG_BYTE_CODE_ARITHMETIC_INSTRUCTION_H
#define ELANG_BYTE_CODE_ARITHMETIC_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	enum class operator_id{
		add,
		sub,
		mult,
		div,
		mod,
		and_,
		xor_,
		or_,
		sal,
		sar,
		not,
		inc,
		dec,
	};

	struct unary_arithmetic_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack,
			std::size_t size, operator_id id){
			switch (size){
			case 1u://Byte
				return eval<__int8>(mem_tbl, reg_tbl, id);
			case 2u://Word
				return eval<__int16>(mem_tbl, reg_tbl, id);
			case 4u://Double Word
				return eval<__int32>(mem_tbl, reg_tbl, id);
			case 8u://Quad Word
				return eval<__int64>(mem_tbl, reg_tbl, id);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void eval(memory::table &mem_tbl, memory::register_table &reg_tbl, operator_id id){
			operand_info::destination_type dest;
			operand_info::extract_destination(sizeof(target_type), mem_tbl, reg_tbl, dest);

			switch (id){
			case operator_id::not:
				operand_info::destination_query::write(dest, ~operand_info::destination_query::read<target_type>(dest));
				break;
			case operator_id::dec:
				operand_info::destination_query::write(dest,
					operand_info::destination_query::read<target_type>(dest) - static_cast<target_type>(1));
				break;
			case operator_id::inc:
				operand_info::destination_query::write(dest,
					operand_info::destination_query::read<target_type>(dest) + static_cast<target_type>(1));
				break;
			default:
				throw common::error::byte_code_unknown_instruction;
				break;
			}
		}
	};

	struct binary_arithmetic_instruction{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack,
			std::size_t size, operator_id id){
			switch (size){
			case 1u://Byte
				return eval<__int8>(mem_tbl, reg_tbl, id);
			case 2u://Word
				return eval<__int16>(mem_tbl, reg_tbl, id);
			case 4u://Double Word
				return eval<__int32>(mem_tbl, reg_tbl, id);
			case 8u://Quad Word
				return eval<__int64>(mem_tbl, reg_tbl, id);
			case 10u://Float
				return evalf<long double>(mem_tbl, reg_tbl, id);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void eval(memory::table &mem_tbl, memory::register_table &reg_tbl, operator_id id){
			operand_info::destination_type dest;
			operand_info::extract_destination(sizeof(target_type), mem_tbl, reg_tbl, dest);

			auto left = operand_info::destination_query::read<target_type>(dest);
			auto right = operand_info::extract_source<target_type>(mem_tbl, reg_tbl);

			if (evalf(id, dest, left, right))
				return;//Evaluated

			switch (id){
			case operator_id::mod:
				left %= right;
				break;
			case operator_id::and_:
				left &= right;
				break;
			case operator_id::xor_:
				left ^= right;
				break;
			case operator_id::or_:
				left |= right;
				break;
			case operator_id::sal:
				left <<= right;
				break;
			case operator_id::sar:
				left >>= right;
				break;
			default:
				throw common::error::byte_code_unknown_instruction;
				break;
			}

			operand_info::destination_query::write(dest, left);
		}

		template <typename target_type>
		static void evalf(memory::table &mem_tbl, memory::register_table &reg_tbl, operator_id id){
			operand_info::destination_type dest;
			operand_info::extract_destination(sizeof(target_type), mem_tbl, reg_tbl, dest);

			auto left = operand_info::destination_query::read<target_type>(dest);
			auto right = operand_info::extract_source<target_type>(mem_tbl, reg_tbl);

			if (!evalf<target_type>(id, dest, left, right))
				throw common::error::byte_code_unknown_instruction;
		}

		template <typename target_type>
		static bool evalf(operator_id id, operand_info::destination_type &dest, target_type left, target_type right){
			switch (id){
			case operator_id::add:
				left += right;
				break;
			case operator_id::sub:
				left -= right;
				break;
			case operator_id::mult:
				left *= right;
				break;
			case operator_id::div:
				left /= right;
				break;
			default:
				return false;
			}

			operand_info::destination_query::write(dest, left);
			return true;
		}
	};

	template <>
	struct instruction<id::notb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::not);
		}
	};

	template <>
	struct instruction<id::notw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::not);
		}
	};

	template <>
	struct instruction<id::notd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::not);
		}
	};

	template <>
	struct instruction<id::notq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::not);
		}
	};

	template <>
	struct instruction<id::decb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::dec);
		}
	};

	template <>
	struct instruction<id::decw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::dec);
		}
	};

	template <>
	struct instruction<id::decd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::dec);
		}
	};

	template <>
	struct instruction<id::decq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::dec);
		}
	};

	template <>
	struct instruction<id::incb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::inc);
		}
	};

	template <>
	struct instruction<id::incw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::inc);
		}
	};

	template <>
	struct instruction<id::incd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::inc);
		}
	};

	template <>
	struct instruction<id::incq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::inc);
		}
	};

	template <>
	struct instruction<id::addb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::add);
		}
	};

	template <>
	struct instruction<id::addw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::add);
		}
	};

	template <>
	struct instruction<id::addd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::add);
		}
	};

	template <>
	struct instruction<id::addq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::add);
		}
	};

	template <>
	struct instruction<id::addf>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 10, operator_id::add);
		}
	};

	template <>
	struct instruction<id::subb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::subw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::subd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::subq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::subf>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 10, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::multb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::multw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::multd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::multq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::multf>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 10, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::divb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::div);
		}
	};

	template <>
	struct instruction<id::divw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::div);
		}
	};

	template <>
	struct instruction<id::divd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::div);
		}
	};

	template <>
	struct instruction<id::divq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::div);
		}
	};

	template <>
	struct instruction<id::divf>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 10, operator_id::div);
		}
	};

	template <>
	struct instruction<id::modb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::mod);
		}
	};

	template <>
	struct instruction<id::modw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::mod);
		}
	};

	template <>
	struct instruction<id::modd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::mod);
		}
	};

	template <>
	struct instruction<id::modq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::mod);
		}
	};

	template <>
	struct instruction<id::andb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::and_);
		}
	};

	template <>
	struct instruction<id::andw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::and_);
		}
	};

	template <>
	struct instruction<id::andd>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::and_);
		}
	};

	template <>
	struct instruction<id::andq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::and_);
		}
	};

	template <>
	struct instruction<id::xorb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::xor_);
		}
	};

	template <>
	struct instruction<id::xorw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::xor_);
		}
	};

	template <>
	struct instruction<id::xord>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::xor_);
		}
	};

	template <>
	struct instruction<id::xorq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::xor_);
		}
	};

	template <>
	struct instruction<id::orb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::or_);
		}
	};

	template <>
	struct instruction<id::orw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::or_);
		}
	};

	template <>
	struct instruction<id::ord>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::or_);
		}
	};

	template <>
	struct instruction<id::orq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::or_);
		}
	};

	template <>
	struct instruction<id::salb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::sal);
		}
	};

	template <>
	struct instruction<id::salw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::sal);
		}
	};

	template <>
	struct instruction<id::sald>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::sal);
		}
	};

	template <>
	struct instruction<id::salq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::sal);
		}
	};

	template <>
	struct instruction<id::sarb>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 1, operator_id::sar);
		}
	};

	template <>
	struct instruction<id::sarw>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 2, operator_id::sar);
		}
	};

	template <>
	struct instruction<id::sard>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 4, operator_id::sar);
		}
	};

	template <>
	struct instruction<id::sarq>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(mem_tbl, reg_tbl, stack, 8, operator_id::sar);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_ARITHMETIC_INSTRUCTION_H */
