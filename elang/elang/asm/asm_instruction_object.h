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

		virtual void encode(common::binary_output_writer &writer, std::size_t &size){
			encode_opcode_(writer, size);
			encode_additional_(writer, size);

			auto target_size = target_size_();
			for (auto operand : operands_)
				operand->encode(target_size, writer, size);
		}

		virtual std::size_t encoded_size() const{
			std::size_t size = 0, target_size = target_size_();
			for (auto operand : operands_)//Accumulate
				size += operand->encoded_size(target_size);

			return (size + sizeof(byte_code::id));
		}

		virtual void validate() const{}

	protected:
		virtual void encode_opcode_(common::binary_output_writer &writer, std::size_t &size){
			writer.write(get_opcode_());
			size += sizeof(byte_code::id);
		}

		virtual void encode_additional_(common::binary_output_writer &writer, std::size_t &size){}

		virtual byte_code::id offset_opcode_(byte_code::id code) const{
			switch (target_size_()){
			case 1u://Byte
				return code;
			case 2u://Word
				return static_cast<byte_code::id>(static_cast<int>(code) + 1);
			case 4u://Double Word
				return static_cast<byte_code::id>(static_cast<int>(code) + 2);
			case 8u://Word
				return static_cast<byte_code::id>(static_cast<int>(code) + 3);
			case 10u://Word
				return static_cast<byte_code::id>(static_cast<int>(code) + 4);
			default:
				break;
			}

			throw common::error::asm_bad_operand_size;
		}

		virtual byte_code::id get_opcode_() const = 0;

		virtual std::size_t target_size_() const = 0;

		operand_object_ptr_list_type operands_;
	};

	template <byte_code::id id>
	class empty_instruction_object : public instruction_object{
	public:
		template <typename... args_types>
		explicit empty_instruction_object(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~empty_instruction_object() = default;

		virtual void validate() const override{
			if (!operands_.empty())
				throw common::error::asm_bad_operand_count;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return id;
		}
	};

	class enter_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit enter_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~enter_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 1u)
				throw common::error::asm_bad_operand_count;

			if (!operands_[0]->is_immediate())
				throw common::error::asm_ambiguous_instruction;

			if (operands_[0]->size() == 10u)
				throw common::error::asm_bad_operand_size;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return byte_code::id::enter;
		}

		virtual std::size_t target_size_() const override{
			return 8u;
		}
	};

	using nop_instruction		= empty_instruction_object<byte_code::id::nop>;
	using leave_instruction		= empty_instruction_object<byte_code::id::leave>;
	using ret_instruction		= empty_instruction_object<byte_code::id::ret>;
}

#endif /* !ELANG_ASM_INSTRUCTION_OBJECT_H */
