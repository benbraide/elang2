#pragma once

#ifndef ELANG_ASM_OFFSET_INSTRUCTION_OPERAND_H
#define ELANG_ASM_OFFSET_INSTRUCTION_OPERAND_H

#include "asm_instruction_operand_object.h"

namespace elang::easm{
	class offset_instruction_operand : public instruction_operand_object{
	public:
		struct item_info{
			byte_code::operand_info::offset_op_type op;
			ptr_type value;
		};

		explicit offset_instruction_operand(std::vector<item_info> &&list)
			: list_(std::move(list)){}

		virtual ~offset_instruction_operand() = default;

		virtual void encode(std::size_t target_size, common::output_writer &writer, std::size_t &size) override{
			if (list_.size() > 63u)//Can't fit size
				throw common::error::asm_too_many_expression_operands;

			byte_code::operand_info::format format;
			format.type = byte_code::operand_info::type::offset;
			format.value = static_cast<unsigned char>(list_.size());

			writer.write(format);
			for (auto &item : list_){//Encode list
				writer.write(item.op);
				item.value->encode(target_size, writer, size);
				size += sizeof(byte_code::operand_info::offset_op_type);
			}

			size += sizeof(byte_code::operand_info::format);
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
			case 10u://Float
				return read_constant_<long double>(buffer, offset);
			default:
				break;
			}

			throw common::error::asm_bad_operand_size;
		}

		virtual std::size_t encoded_size(std::size_t target_size) const override{
			std::size_t size = 0;
			for (auto item : list_)//Accumulate size
				size += (item.value->encoded_size(target_size) + sizeof(byte_code::operand_info::offset_op_type));
			return (size + sizeof(byte_code::operand_info::format));
		}

	protected:
		template <typename target_type>
		void read_constant_(char *buffer, std::size_t &offset){
			auto sum = static_cast<target_type>(0), value = static_cast<target_type>(0);
			for (auto item : list_){//Accumulate
				item.value->read_constant((char *)(&value), sizeof(target_type), offset);
				if (item.op == byte_code::operand_info::offset_op_type::sub)
					sum -= value;
				else//Add
					sum += value;
			}

			memcpy(buffer, &sum, sizeof(target_type));
			offset += sizeof(target_type);
		}

		std::vector<item_info> list_;
	};
}

#endif /* !ELANG_ASM_OFFSET_INSTRUCTION_OPERAND_H */
