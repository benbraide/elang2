#pragma once

#ifndef ELANG_ASM_INSTRUCTION_OBJECT_H
#define ELANG_ASM_INSTRUCTION_OBJECT_H

#include <memory>
#include <vector>

#include "../byte_code/byte_code_id.h"

#include "asm_instruction_operand_object.h"

namespace elang::easm{
	class instruction_object{
	public:
		typedef std::shared_ptr<instruction_operand_object> operand_object_ptr_type;
		typedef std::vector<operand_object_ptr_type> operand_object_ptr_list_type;

		explicit instruction_object(operand_object_ptr_list_type &&operands)
			: operands_(std::move(operands)){}

		virtual ~instruction_object() = default;

		virtual void encode(common::output_writer &writer, std::size_t &size){
			validate_();
			encode_opcode_(writer, size);

			auto target_size = target_size_();
			for (auto operand : operands_)
				operand->encode(target_size, writer, size);
		}

	protected:
		virtual void encode_opcode_(common::output_writer &writer, std::size_t &size) = 0;

		virtual void validate_() const{}

		virtual std::size_t target_size_() const = 0;

		operand_object_ptr_list_type operands_;
	};
}

#endif /* !ELANG_ASM_INSTRUCTION_OBJECT_H */
