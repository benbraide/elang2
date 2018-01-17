#pragma once

#ifndef ELANG_ASM_INSTRUCTION_OPERAND_OBJECT_H
#define ELANG_ASM_INSTRUCTION_OPERAND_OBJECT_H

#include "../common/output_writer.h"

#include "../byte_code/byte_code_operand_info.h"

namespace elang::easm{
	class instruction_operand_object{
	public:
		typedef std::shared_ptr<instruction_operand_object> ptr_type;

		virtual ~instruction_operand_object() = default;

		virtual void encode(std::size_t target_size, common::output_writer &writer, std::size_t &size){
			throw common::error::asm_bad_operand_type;
		}

		virtual void encode(std::size_t target_size, common::output_writer &writer, std::size_t &size, memory::register_table &reg_tbl){
			encode(target_size, writer, size);
		}

		virtual void read_constant(char *buffer, std::size_t size, std::size_t &offset){
			throw common::error::asm_bad_contant_value;
		}

		template <typename target_type>
		target_type read_constant(std::size_t &offset){
			auto value = target_type();
			read_constant((char *)(&value), sizeof(target_type), offset);
			return value;
		}

		virtual std::size_t encoded_size(std::size_t target_size) const = 0;

		virtual std::size_t size() const{
			return 0;
		}

		virtual bool is_register() const{
			return false;
		}

		virtual bool is_memory() const{
			return false;
		}

		virtual bool is_immediate() const{
			return false;
		}
	};
}

#endif /* !ELANG_ASM_INSTRUCTION_OPERAND_OBJECT_H */
