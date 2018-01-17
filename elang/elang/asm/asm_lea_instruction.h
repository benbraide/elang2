#pragma once

#ifndef ELANG_ASM_LEA_INSTRUCTION_H
#define ELANG_ASM_LEA_INSTRUCTION_H

#include "asm_instruction_object.h"

namespace elang::easm{
	class lea_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit lea_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~lea_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 2u)
				throw common::error::asm_bad_operand_count;

			if (!operands_[0]->is_register())
				throw common::error::asm_bad_operand_type;

			if (operands_[0]->size() == 10u)
				throw common::error::asm_bad_operand_size;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(byte_code::id::leab);
		}

		virtual std::size_t target_size_() const override{
			return operands_[0]->size();
		}
	};
}

#endif /* !ELANG_ASM_LEA_INSTRUCTION_H */
