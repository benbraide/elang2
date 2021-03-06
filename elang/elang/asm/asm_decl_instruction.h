#pragma once

#ifndef ELANG_ASM_DECL_INSTRUCTION_H
#define ELANG_ASM_DECL_INSTRUCTION_H

#include "asm_instruction_object.h"

namespace elang::easm{
	class dz_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit dz_instruction(unsigned __int64 count, args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...), count_(count){}

		virtual ~dz_instruction() = default;

		virtual void encode(common::binary_output_writer &writer, memory::register_table &reg_tbl) override{
			memset(writer.allocate(count_), 0, count_);
		}

		virtual std::size_t encoded_size() const override{
			return count_;
		}

		virtual void validate() const override{
			if (!operands_.empty())
				throw common::error::asm_bad_operand_count;
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(byte_code::id::dz);
		}

		virtual std::size_t target_size_() const override{
			return 0;
		}

		unsigned __int64 count_;
	};

	template <std::size_t decl_size>
	class decl_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit decl_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~decl_instruction() = default;

		virtual void encode(common::binary_output_writer &writer, memory::register_table &reg_tbl) override{
			std::size_t actual_size = ((decl_size == 10u) ? 8u : decl_size);
			for (auto operand : operands_){//Copy bytes
				if (operand->is_string())
					operand->read_constant(writer.allocate(instruction_operand_object::count_string(operand->size(), actual_size)), decl_size);
				else//Constant
					operand->read_constant(writer.allocate(actual_size), decl_size);
			}
		}

		virtual std::size_t encoded_size() const override{
			std::size_t size = 0, actual_size = ((decl_size == 10u) ? 8u : decl_size);
			for (auto operand : operands_){//Accumulate
				if (operand->is_string())
					size += instruction_operand_object::count_string(operand->size(), actual_size);
				else//Constant
					size += actual_size;
			}

			return size;
		}

		virtual void validate() const override{
			if (operands_.empty())
				throw common::error::asm_bad_operand_count;
		}

		virtual void update_position(unsigned __int64 value) override{
			std::size_t actual_size = ((decl_size == 10u) ? 8u : decl_size);
			for (auto operand : operands_){
				operand->update_position(value);
				if (operand->is_string())
					value += instruction_operand_object::count_string(operand->size(), actual_size);
				else//Constant
					value += actual_size;
			}
		}

	protected:
		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(byte_code::id::db);
		}

		virtual std::size_t target_size_() const override{
			return decl_size;
		}
	};

	using db_instruction	= decl_instruction<1u>;
	using dw_instruction	= decl_instruction<2u>;
	using dd_instruction	= decl_instruction<4u>;
	using dq_instruction	= decl_instruction<8u>;
	using df_instruction	= decl_instruction<10u>;
}

#endif /* !ELANG_ASM_DECL_INSTRUCTION_H */
