#pragma once

#ifndef ELANG_ASM_CNVT_INSTRUCTION_H
#define ELANG_ASM_CNVT_INSTRUCTION_H

#include "asm_instruction_object.h"

namespace elang::easm{
	class cnvt_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit cnvt_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~cnvt_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 2u)
				throw common::error::asm_bad_operand_count;

			if (!operands_[0]->is_register() || !operands_[1]->is_register())
				throw common::error::asm_bad_operand_type;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			switch (operands_[0]->size()){
			case 1u://Byte
				return byte_code::id::cnvtb;
			case 2u://Word
				return byte_code::id::cnvtw;
			case 4u://Double Word
				return byte_code::id::cnvtd;
			case 8u://Word
				return byte_code::id::cnvtq;
			case 10u://Word
				return byte_code::id::cnvtf;
			default:
				break;
			}

			throw common::error::asm_bad_operand_size;
		}

		virtual std::size_t target_size_() const override{
			return 0u;
		}
	};
}

#endif /* !ELANG_ASM_CNVT_INSTRUCTION_H */
