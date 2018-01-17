#pragma once

#ifndef ELANG_ASM_STACK_INSTRUCTION_H
#define ELANG_ASM_STACK_INSTRUCTION_H

#include "asm_instruction_object.h"

namespace elang::easm{
	template <byte_code::id id>
	class stack_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit stack_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~stack_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 1u)
				throw common::error::asm_bad_operand_count;

			if (!operands_[0]->is_register() && !operands_[0]->is_memory())
				throw common::error::asm_bad_operand_type;

			if (operands_[0]->size() == 0u)
				throw common::error::asm_ambiguous_instruction;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(id);
		}

		virtual std::size_t target_size_() const override{
			return operands_[0]->size();
		}
	};

	using push_instruction		= stack_instruction<byte_code::id::pushb>;
	using pop_instruction		= stack_instruction<byte_code::id::popb>;
}

#endif /* !ELANG_ASM_STACK_INSTRUCTION_H */
