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
		lal,
		lar,
		not,
		inc,
		dec,
	};

	struct unary_arithmetic_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack,
			std::size_t size, operator_id id){
			switch (size){
			case 1u://Byte
				return eval<__int8>(ptr, base_ptr, reg_tbl, id);
			case 2u://Word
				return eval<__int16>(ptr, base_ptr, reg_tbl, id);
			case 4u://Double Word
				return eval<__int32>(ptr, base_ptr, reg_tbl, id);
			case 8u://Quad Word
				return eval<__int64>(ptr, base_ptr, reg_tbl, id);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void eval(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, operator_id id){
			auto destination = operand_info::extract_destination(ptr, base_ptr, reg_tbl);

			auto value = target_type();
			memcpy(&value, destination, sizeof(target_type));

			switch (id){
			case operator_id::not:
				value = ~value;
				break;
			case operator_id::dec:
				++value;
				break;
			case operator_id::inc:
				--value;
				break;
			default:
				throw common::error::byte_code_unknown_instruction;
				break;
			}

			memcpy(destination, &value, sizeof(target_type));
			reg_tbl.instruction_pointer()->write(*reinterpret_cast<__int64 *>(ptr));
		}
	};

	struct binary_arithmetic_instruction{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack,
			std::size_t size, operator_id id){
			switch (size){
			case 1u://Byte
				return eval<__int8>(ptr, base_ptr, reg_tbl, id);
			case 2u://Word
				return eval<__int16>(ptr, base_ptr, reg_tbl, id);
			case 4u://Double Word
				return eval<__int32>(ptr, base_ptr, reg_tbl, id);
			case 8u://Quad Word
				return eval<__int64>(ptr, base_ptr, reg_tbl, id);
			case 10u://Float
				return evalf<long double>(ptr, base_ptr, reg_tbl, id);
			default:
				break;
			}

			throw common::error::byte_code_bad_operand_size;
		}

		template <typename target_type>
		static void eval(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, operator_id id){
			auto destination = operand_info::extract_destination(ptr, base_ptr, reg_tbl);
			auto right = operand_info::extract_source<target_type>(ptr, base_ptr, reg_tbl);

			auto left = target_type();
			memcpy(&left, destination, sizeof(target_type));

			if (evalf(ptr, reg_tbl, id, destination, left, right))
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
			case operator_id::lal:
				left <<= right;
				break;
			case operator_id::lar:
				left >>= right;
				break;
			default:
				throw common::error::byte_code_unknown_instruction;
				break;
			}

			memcpy(destination, &left, sizeof(target_type));
			reg_tbl.instruction_pointer()->write(*reinterpret_cast<__int64 *>(ptr));
		}

		template <typename target_type>
		static void evalf(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, operator_id id){
			auto destination = operand_info::extract_destination(ptr, base_ptr, reg_tbl);

			auto left = target_type();
			memcpy(&left, destination, sizeof(target_type));

			if (!evalf<target_type>(ptr, reg_tbl, id, destination, left, operand_info::extract_source<target_type>(ptr, base_ptr, reg_tbl)))
				throw common::error::byte_code_unknown_instruction;
		}

		template <typename target_type>
		static bool evalf(char *&ptr, memory::register_table &reg_tbl, operator_id id, char *destination,
			target_type left, target_type right){
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

			memcpy(destination, &left, sizeof(target_type));
			reg_tbl.instruction_pointer()->write(*reinterpret_cast<__int64 *>(ptr));

			return true;
		}
	};

	template <>
	struct instruction<id::notb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::not);
		}
	};

	template <>
	struct instruction<id::notw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::not);
		}
	};

	template <>
	struct instruction<id::notd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::not);
		}
	};

	template <>
	struct instruction<id::notq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::not);
		}
	};

	template <>
	struct instruction<id::decb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::dec);
		}
	};

	template <>
	struct instruction<id::decw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::dec);
		}
	};

	template <>
	struct instruction<id::decd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::dec);
		}
	};

	template <>
	struct instruction<id::decq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::dec);
		}
	};

	template <>
	struct instruction<id::incb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::inc);
		}
	};

	template <>
	struct instruction<id::incw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::inc);
		}
	};

	template <>
	struct instruction<id::incd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::inc);
		}
	};

	template <>
	struct instruction<id::incq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			unary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::inc);
		}
	};

	template <>
	struct instruction<id::addb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::add);
		}
	};

	template <>
	struct instruction<id::addw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::add);
		}
	};

	template <>
	struct instruction<id::addd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::add);
		}
	};

	template <>
	struct instruction<id::addq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::add);
		}
	};

	template <>
	struct instruction<id::addf>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 10, operator_id::add);
		}
	};

	template <>
	struct instruction<id::subb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::subw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::subd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::subq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::subf>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 10, operator_id::sub);
		}
	};

	template <>
	struct instruction<id::multb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::multw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::multd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::multq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::multf>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 10, operator_id::mult);
		}
	};

	template <>
	struct instruction<id::divb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::div);
		}
	};

	template <>
	struct instruction<id::divw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::div);
		}
	};

	template <>
	struct instruction<id::divd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::div);
		}
	};

	template <>
	struct instruction<id::divq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::div);
		}
	};

	template <>
	struct instruction<id::divf>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 10, operator_id::div);
		}
	};

	template <>
	struct instruction<id::modb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::mod);
		}
	};

	template <>
	struct instruction<id::modw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::mod);
		}
	};

	template <>
	struct instruction<id::modd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::mod);
		}
	};

	template <>
	struct instruction<id::modq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::mod);
		}
	};

	template <>
	struct instruction<id::andb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::and_);
		}
	};

	template <>
	struct instruction<id::andw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::and_);
		}
	};

	template <>
	struct instruction<id::andd>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::and_);
		}
	};

	template <>
	struct instruction<id::andq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::and_);
		}
	};

	template <>
	struct instruction<id::xorb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::xor_);
		}
	};

	template <>
	struct instruction<id::xorw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::xor_);
		}
	};

	template <>
	struct instruction<id::xord>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::xor_);
		}
	};

	template <>
	struct instruction<id::xorq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::xor_);
		}
	};

	template <>
	struct instruction<id::orb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::or_);
		}
	};

	template <>
	struct instruction<id::orw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::or_);
		}
	};

	template <>
	struct instruction<id::ord>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::or_);
		}
	};

	template <>
	struct instruction<id::orq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::or_);
		}
	};

	template <>
	struct instruction<id::lalb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::lal);
		}
	};

	template <>
	struct instruction<id::lalw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::lal);
		}
	};

	template <>
	struct instruction<id::lald>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::lal);
		}
	};

	template <>
	struct instruction<id::lalq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::lal);
		}
	};

	template <>
	struct instruction<id::larb>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 1, operator_id::lar);
		}
	};

	template <>
	struct instruction<id::larw>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 2, operator_id::lar);
		}
	};

	template <>
	struct instruction<id::lard>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 4, operator_id::lar);
		}
	};

	template <>
	struct instruction<id::larq>{
		static void evaluate(char *&ptr, char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
			binary_arithmetic_instruction::evaluate(ptr, base_ptr, reg_tbl, stack, 8, operator_id::lar);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_ARITHMETIC_INSTRUCTION_H */
