#pragma once

#ifndef ELANG_ASM_IMMEDIATE_INSTRUCTION_OPERAND_H
#define ELANG_ASM_IMMEDIATE_INSTRUCTION_OPERAND_H

#include "asm_instruction_operand_object.h"

namespace elang::easm{
	template <typename value_type>
	class immediate_instruction_operand : public instruction_operand_object{
	public:
		typedef value_type value_type;

		explicit immediate_instruction_operand(value_type value)
			: value_(value){}

		virtual ~immediate_instruction_operand() = default;

		virtual void encode(std::size_t target_size, common::binary_output_writer &writer, memory::register_table &reg_tbl) override{
			byte_code::operand_info::format format;
			format.type = byte_code::operand_info::type::immediate;
			format.value = static_cast<unsigned char>(target_size);

			writer.write(format);
			read_constant(writer.allocate((target_size == 10u) ? 8u : target_size), target_size);
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
			case 10u://Float
				return read_constant_<long double>(buffer);
			default:
				break;
			}

			throw common::error::asm_bad_operand_size;
		}

		virtual std::size_t encoded_size(std::size_t target_size) const override{
			return (((target_size == 10u) ? 8u : target_size) + sizeof(byte_code::operand_info::format));
		}

		virtual bool is_immediate() const override{
			return true;
		}

	protected:
		template <typename target_type>
		void read_constant_(char *buffer){
			auto target = static_cast<target_type>(value_);
			memcpy(buffer, &target, sizeof(target_type));
		}

		value_type value_;
	};
}

#endif /* !ELANG_ASM_IMMEDIATE_INSTRUCTION_OPERAND_H */
