#pragma once

#ifndef ELANG_ASM_MOV_INSTRUCTION_H
#define ELANG_ASM_MOV_INSTRUCTION_H

#include "asm_instruction_object.h"

namespace elang::easm{
	class mov_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit mov_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~mov_instruction() = default;

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
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(byte_code::id::movb);
		}

		virtual std::size_t target_size_() const override{
			return ((operands_[0]->size() == 0u) ? operands_[1]->size() : operands_[0]->size());
		}
	};
}

#endif /* !ELANG_ASM_MOV_INSTRUCTION_H */
