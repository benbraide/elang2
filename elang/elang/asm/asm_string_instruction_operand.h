#pragma once

#ifndef ELANG_ASM_STRING_INSTRUCTION_OPERAND_H
#define ELANG_ASM_STRING_INSTRUCTION_OPERAND_H

#include "asm_instruction_operand_object.h"

namespace elang::easm{
	class string_instruction_operand : public instruction_operand_object{
	public:
		explicit string_instruction_operand(std::string &&value)
			: value_(std::move(value)){}

		virtual ~string_instruction_operand() = default;

		virtual void encode(std::size_t target_size, common::binary_output_writer &writer, std::size_t &size) override{
			if (value_.size() != 1u)
				throw common::error::asm_bad_operand_type;

			if (target_size != 1u)
				throw common::error::asm_bad_operand_size;

			byte_code::operand_info::format format;
			format.type = byte_code::operand_info::type::immediate;
			format.value = static_cast<unsigned char>(target_size);

			writer.write(format);
			writer.write(value_.data(), target_size);
			size += (target_size + sizeof(byte_code::operand_info::format));
		}

		virtual void read_constant(char *buffer, std::size_t size, std::size_t &offset) override{
			switch (size){
			case 1u://Byte
				return read_constant_<__int8>(buffer, offset);
			case 2u://Word
				return read_constant_<__int16>(buffer, offset);
			case 4u://Double Word
				return read_constant_<__int32>(buffer, offset);
			case 8u://Quad Word
				return read_constant_<__int64>(buffer, offset);
			default:
				break;
			}

			throw common::error::asm_bad_operand_size;
		}

		virtual std::size_t encoded_size(std::size_t target_size) const override{
			return ((target_size == 0u) ? 0u : ((value_.size() * target_size) + (value_.size() % target_size)));
		}

	protected:
		template <typename target_type>
		void read_constant_(char *buffer, std::size_t &offset){
			memcpy(buffer, value_.data(), value_.size());
			offset += encoded_size(sizeof(target_type));
		}

		std::string value_;
	};
}

#endif /* !ELANG_ASM_STRING_INSTRUCTION_OPERAND_H */
