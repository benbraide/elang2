#pragma once

#ifndef ELANG_ASM_REGISTER_INSTRUCTION_OPERAND_H
#define ELANG_ASM_REGISTER_INSTRUCTION_OPERAND_H

#include "asm_instruction_operand_object.h"

namespace elang::easm{
	class register_instruction_operand : public instruction_operand_object{
	public:
		explicit register_instruction_operand(memory::memory_register &value)
			: value_(&value){}

		virtual ~register_instruction_operand() = default;

		virtual void encode(std::size_t target_size, common::binary_output_writer &writer, std::size_t &size, memory::register_table &reg_tbl) override{
			if (target_size == 0u)//Any size
				target_size = value_->size();
			else if ((value_->is_floating_point() && target_size != 10u) || target_size != value_->size())
				throw common::error::asm_bad_operand_size;//Size mismatch

			byte_code::operand_info::format format;
			format.type = byte_code::operand_info::type::register_;
			format.value = static_cast<unsigned char>(reg_tbl.index(*value_));

			writer.write(format);
			size += sizeof(byte_code::operand_info::format);
		}

		virtual std::size_t encoded_size(std::size_t target_size) const override{
			return sizeof(byte_code::operand_info::format);
		}

		virtual std::size_t size() const override{
			return (value_->is_floating_point() ? 10u : value_->size());
		}

		virtual bool is_register() const override{
			return true;
		}

	protected:
		memory::memory_register *value_;
	};
}

#endif /* !ELANG_ASM_REGISTER_INSTRUCTION_OPERAND_H */
