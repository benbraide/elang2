#pragma once

#ifndef ELANG_ASM_MEMORY_INSTRUCTION_OPERAND_H
#define ELANG_ASM_MEMORY_INSTRUCTION_OPERAND_H

#include "asm_instruction_operand_object.h"

namespace elang::easm{
	class memory_instruction_operand : public instruction_operand_object{
	public:
		explicit memory_instruction_operand(ptr_type value, std::size_t size = 0u)
			: value_(value), size_(size){}

		virtual ~memory_instruction_operand() = default;

		virtual void encode(std::size_t target_size, common::output_writer &writer, std::size_t &size) override{
			byte_code::operand_info::format format;
			format.type = byte_code::operand_info::type::memory;
			format.value = static_cast<unsigned char>(0);

			writer.write(format);
			value_->encode(sizeof(std::size_t), writer, size);

			size += sizeof(byte_code::operand_info::format);
		}

		virtual std::size_t encoded_size(std::size_t target_size) const override{
			return (value_->encoded_size(target_size) + sizeof(byte_code::operand_info::format));
		}

		virtual std::size_t size() const override{
			return size_;
		}

		virtual bool is_memory() const override{
			return true;
		}

	protected:
		ptr_type value_;
		std::size_t size_;
	};
}

#endif /* !ELANG_ASM_MEMORY_INSTRUCTION_OPERAND_H */