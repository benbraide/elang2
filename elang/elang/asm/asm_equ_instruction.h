#pragma once

#ifndef ELANG_ASM_EQU_INSTRUCTION_H
#define ELANG_ASM_EQU_INSTRUCTION_H

#include "asm_instruction_object.h"

namespace elang::easm{
	class equ_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit equ_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~equ_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 1u)
				throw common::error::asm_bad_operand_count;
		}

		virtual void update_position(unsigned __int64 value) override{
			operands_[0]->update_position(value);
			operands_[0]->read_constant(reinterpret_cast<char *>(&value_), sizeof(unsigned __int64));
		}

		virtual unsigned __int64 &value(){
			return value_;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(byte_code::id::equ);
		}

		virtual std::size_t target_size_() const override{
			return 0;
		}

		unsigned __int64 value_;
	};
}

#endif /* !ELANG_ASM_EQU_INSTRUCTION_H */
