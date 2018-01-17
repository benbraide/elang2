#pragma once

#ifndef ELANG_ASM_CALL_INSTRUCTION_H
#define ELANG_ASM_CALL_INSTRUCTION_H

#include "asm_instruction_object.h"

namespace elang::easm{
	class call_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit call_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~call_instruction() = default;

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
			return offset_opcode_(byte_code::id::callb);
		}

		virtual std::size_t target_size_() const override{
			return ((operands_[0]->size() == 0u) ? 8u : operands_[0]->size());
		}
	};
}

#endif /* !ELANG_ASM_CALL_INSTRUCTION_H */
