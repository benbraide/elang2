#pragma once

#ifndef ELANG_ASM_UNINITIALIZED_INSTRUCTION_OPERAND_H
#define ELANG_ASM_UNINITIALIZED_INSTRUCTION_OPERAND_H

#include "asm_instruction_operand_object.h"

namespace elang::easm{
	class uninitialized_instruction_operand : public instruction_operand_object{
	public:
		virtual ~uninitialized_instruction_operand() = default;

		virtual void read_constant(char *buffer, std::size_t size) override{}

		virtual std::size_t encoded_size(std::size_t target_size) const override{
			return ((target_size == 10u) ? 8u : target_size);
		}
	};
}

#endif /* !ELANG_ASM_UNINITIALIZED_INSTRUCTION_OPERAND_H */
