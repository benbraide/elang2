#pragma once

#ifndef ELANG_ASM_SET_INSTRUCTION_H
#define ELANG_ASM_SET_INSTRUCTION_H

#include "../byte_code/byte_code_comparison.h"

#include "asm_instruction_object.h"

namespace elang::easm{
	template <unsigned char inverse, byte_code::comparison_id id>
	class set_instruction : public instruction_object{
	public:
		template <typename... args_types>
		explicit set_instruction(args_types &&... args)
			: instruction_object(std::forward<args_types>(args)...){}

		virtual ~set_instruction() = default;

		virtual void validate() const override{
			if (operands_.size() != 1u)
				throw common::error::asm_bad_operand_count;

			if (!operands_[0]->is_register())
				throw common::error::asm_ambiguous_instruction;

			if (operands_[0]->size() == 10u)
				throw common::error::asm_bad_operand_size;
		}

	protected:
		virtual void encode_additional_(common::output_writer &writer, std::size_t &size) override{
			byte_code::comparison_info info;
			info.inverse = inverse;
			info.id = id;

			writer.write(info);
			size += sizeof(byte_code::comparison_info);
		}

		virtual byte_code::id get_opcode_() const override{
			return offset_opcode_(byte_code::id::setb);
		}

		virtual std::size_t target_size_() const override{
			return operands_[0]->size();
		}
	};

	using setz_instruction		= set_instruction<'\00', byte_code::comparison_id::equal>;
	using setnz_instruction		= set_instruction<'\01', byte_code::comparison_id::equal>;

	using sete_instruction		= set_instruction<'\00', byte_code::comparison_id::equal>;
	using setne_instruction		= set_instruction<'\01', byte_code::comparison_id::equal>;

	using setl_instruction		= set_instruction<'\00', byte_code::comparison_id::less>;
	using setnl_instruction		= set_instruction<'\01', byte_code::comparison_id::less>;

	using setle_instruction		= set_instruction<'\00', byte_code::comparison_id::less_or_equal>;
	using setnle_instruction	= set_instruction<'\01', byte_code::comparison_id::less_or_equal>;

	using setg_instruction		= set_instruction<'\00', byte_code::comparison_id::greater>;
	using setng_instruction		= set_instruction<'\01', byte_code::comparison_id::greater>;

	using setge_instruction		= set_instruction<'\00', byte_code::comparison_id::greater_or_equal>;
	using setnge_instruction	= set_instruction<'\01', byte_code::comparison_id::greater_or_equal>;
}

#endif /* !ELANG_ASM_SET_INSTRUCTION_H */
