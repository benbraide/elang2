#pragma once

#ifndef ELANG_ASM_JMP_INSTRUCTION_H
#define ELANG_ASM_JMP_INSTRUCTION_H

#include "../byte_code/byte_code_comparison.h"

#include "asm_instruction_object.h"

namespace elang::easm{
	class jmp_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit jmp_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~jmp_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 1u)
				throw common::error::asm_bad_operand_count;

			if (operands_[0]->is_memory() && operands_[0]->size() == 0u)
				throw common::error::asm_ambiguous_instruction;

			if (operands_[0]->size() == 10u)
				throw common::error::asm_bad_operand_size;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(byte_code::id::jmpb);
		}

		virtual std::size_t target_size_() const override{
			return ((operands_[0]->size() == 0u) ? 8u : operands_[0]->size());
		}
	};

	template <unsigned char inverse, byte_code::comparison_id id>
	class cjmp_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit cjmp_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~cjmp_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 1u)
				throw common::error::asm_bad_operand_count;

			if (operands_[0]->is_memory() && operands_[0]->size() == 0u)
				throw common::error::asm_ambiguous_instruction;

			if (operands_[0]->size() == 10u)
				throw common::error::asm_bad_operand_size;
		}

	protected:
		virtual void encode_additional_(common::binary_output_writer &writer, std::size_t &size) override{
			byte_code::comparison_info info;
			info.inverse = inverse;
			info.id = id;

			writer.write(info);
			size += sizeof(byte_code::comparison_info);
		}

		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(byte_code::id::cjmpb);
		}

		virtual std::size_t target_size_() const override{
			return ((operands_[0]->size() == 0u) ? 8u : operands_[0]->size());
		}
	};

	using jz_instruction	= cjmp_instruction<'\00', byte_code::comparison_id::equal>;
	using jnz_instruction	= cjmp_instruction<'\01', byte_code::comparison_id::equal>;

	using je_instruction	= cjmp_instruction<'\00', byte_code::comparison_id::equal>;
	using jne_instruction	= cjmp_instruction<'\01', byte_code::comparison_id::equal>;

	using jl_instruction	= cjmp_instruction<'\00', byte_code::comparison_id::less>;
	using jnl_instruction	= cjmp_instruction<'\01', byte_code::comparison_id::less>;

	using jle_instruction	= cjmp_instruction<'\00', byte_code::comparison_id::less_or_equal>;
	using jnle_instruction	= cjmp_instruction<'\01', byte_code::comparison_id::less_or_equal>;

	using jg_instruction	= cjmp_instruction<'\00', byte_code::comparison_id::greater>;
	using jng_instruction	= cjmp_instruction<'\01', byte_code::comparison_id::greater>;

	using jge_instruction	= cjmp_instruction<'\00', byte_code::comparison_id::greater_or_equal>;
	using jnge_instruction	= cjmp_instruction<'\01', byte_code::comparison_id::greater_or_equal>;
}

#endif /* !ELANG_ASM_JMP_INSTRUCTION_H */
