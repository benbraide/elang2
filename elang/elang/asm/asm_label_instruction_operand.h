#pragma once

#ifndef ELANG_ASM_LABEL_INSTRUCTION_OPERAND_H
#define ELANG_ASM_LABEL_INSTRUCTION_OPERAND_H

#include "asm_instruction_operand_object.h"

namespace elang::easm{
	class label_instruction_operand : public instruction_operand_object{
	public:
		explicit label_instruction_operand(unsigned __int64 *&value)
			: value_(&value){}

		virtual ~label_instruction_operand() = default;

		virtual void encode(std::size_t target_size, common::binary_output_writer &writer, memory::register_table &reg_tbl) override{
			if (target_size > 8u)
				throw common::error::asm_bad_operand_size;

			if (*value_ == nullptr)
				throw common::error::asm_label_not_found;

			byte_code::operand_info::format format;
			format.type = byte_code::operand_info::type::immediate;
			format.value = static_cast<unsigned char>(target_size);

			writer.write(format);
			switch (target_size){
			case 1u://Byte
				writer.write(static_cast<__int8>(**value_));
				break;
			case 2u://Word
				writer.write(static_cast<__int16>(**value_));
				break;
			case 4u://Double Word
				writer.write(static_cast<__int32>(**value_));
				break;
			case 8u://Quad Word
				writer.write(static_cast<__int64>(**value_));
				break;
			default:
				throw common::error::asm_bad_operand_size;
				break;
			}
		}

		virtual void read_constant(char *buffer, std::size_t size) override{
			switch (size){
			case 1u://Byte
				return read_constant_<__int8>(buffer);
			case 2u://Word
				return read_constant_<__int16>(buffer);
			case 4u://Double Word
				return read_constant_<__int32>(buffer);
			case 8u://Quad Word
				return read_constant_<__int64>(buffer);
			default:
				break;
			}

			throw common::error::asm_bad_operand_size;
		}

		virtual std::size_t encoded_size(std::size_t target_size) const override{
			return (((target_size == 10u) ? 8u : target_size) + sizeof(byte_code::operand_info::format));
		}

	protected:
		template <typename target_type>
		void read_constant_(char *buffer){
			if (*value_ == nullptr)
				throw common::error::asm_label_not_found;

			auto value = static_cast<target_type>(**value_);
			memcpy(buffer, &value, sizeof(target_type));
		}

		unsigned __int64 **value_;
	};
}

#endif /* !ELANG_ASM_LABEL_INSTRUCTION_OPERAND_H */
