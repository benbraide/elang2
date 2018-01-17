#pragma once

#ifndef ELANG_ASM_ARITHMETIC_INSTRUCTION_H
#define ELANG_ASM_ARITHMETIC_INSTRUCTION_H

#include "asm_instruction_object.h"

namespace elang::easm{
	template <byte_code::id id>
	class unary_arithmetic_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit unary_arithmetic_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~unary_arithmetic_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 1u)
				throw common::error::asm_bad_operand_count;

			if (!operands_[0]->is_register() && !operands_[0]->is_memory())
				throw common::error::asm_bad_operand_type;

			if (operands_[0]->size() == 10u)
				throw common::error::asm_bad_operand_size;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(id);
		}

		virtual std::size_t target_size_() const override{
			return operands_[0]->size();
		}
	};

	template <byte_code::id id, bool is_integral>
	class binary_arithmetic_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit binary_arithmetic_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~binary_arithmetic_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 2u)
				throw common::error::asm_bad_operand_count;

			if (!operands_[0]->is_register() && !operands_[0]->is_memory())
				throw common::error::asm_bad_operand_type;

			auto dest_size = operands_[0]->size(), src_size = operands_[1]->size();
			if (dest_size == 0u || src_size == 0u){
				if (dest_size == src_size)
					throw common::error::asm_ambiguous_instruction;
			}
			else if (dest_size != src_size)
				throw common::error::asm_ambiguous_instruction;

			if (is_integral && target_size_() == 10u)
				throw common::error::asm_bad_operand_size;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(id);
		}

		virtual std::size_t target_size_() const override{
			return ((operands_[0]->size() == 0u) ? operands_[1]->size() : operands_[0]->size());
		}
	};

	using not_instruction		= unary_arithmetic_instruction<byte_code::id::notb>;
	using inc_instruction		= unary_arithmetic_instruction<byte_code::id::incb>;
	using dec_instruction		= unary_arithmetic_instruction<byte_code::id::decb>;

	using add_instruction		= binary_arithmetic_instruction<byte_code::id::addb, false>;
	using sub_instruction		= binary_arithmetic_instruction<byte_code::id::subb, false>;

	using mult_instruction		= binary_arithmetic_instruction<byte_code::id::multb, false>;
	using div_instruction		= binary_arithmetic_instruction<byte_code::id::divb, false>;
	using mod_instruction		= binary_arithmetic_instruction<byte_code::id::modb, true>;

	using and_instruction		= binary_arithmetic_instruction<byte_code::id::andb, true>;
	using xor_instruction		= binary_arithmetic_instruction<byte_code::id::xorb, true>;
	using or_instruction		= binary_arithmetic_instruction<byte_code::id::orb, true>;

	using sal_instruction		= binary_arithmetic_instruction<byte_code::id::salb, true>;
	using sar_instruction		= binary_arithmetic_instruction<byte_code::id::sarb, true>;
}

#endif /* !ELANG_ASM_ARITHMETIC_INSTRUCTION_H */
