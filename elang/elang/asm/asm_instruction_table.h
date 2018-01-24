#pragma once

#ifndef ELANG_ASM_INSTRUCTION_TABLE_H
#define ELANG_ASM_INSTRUCTION_TABLE_H

#include <list>
#include <unordered_map>

#include "../memory/memory_register_table.h"

#include "asm_section_id.h"
#include "asm_equ_instruction.h"

#include "asm_position_instruction_operand.h"

namespace elang::easm{
	class instruction_table{
	public:
		typedef std::shared_ptr<instruction_object> iptr_type;

		struct instruction_info{
			std::size_t size;
			iptr_type value;
		};

		typedef std::list<instruction_info> iptr_list_type;

		struct label_info{
			unsigned __int64 offset;
			unsigned __int64 *offset_ptr;
			std::shared_ptr<equ_instruction> equ;
		};

		typedef std::unordered_map<std::string, label_info> label_map_type;

		struct section_info{
			unsigned __int64 size;
			iptr_list_type instructions;
			std::list<label_info *> labels;
		};

		typedef std::unordered_map<section_id, section_info> section_map_type;

		void add(section_id section);

		void add(const std::string &label);

		void add(const std::string &label, std::shared_ptr<equ_instruction> equ);

		void add(iptr_type instruction);

		void set_start_label(const std::string &label);

		void set_stack_size(std::size_t size);

		void encode(char *buffer, std::size_t size, std::size_t offset);

		std::size_t size() const;

		std::size_t stack_size() const;

		unsigned __int64 start_address() const;

		unsigned __int64 write_protect_start() const;

		unsigned __int64 write_protect_end() const;

		memory::memory_register *find_register(const std::string &name) const;

		unsigned __int64 *&find_label(const std::string &name);

	private:
		section_info *active_section_ = nullptr;
		section_map_type section_map_;
		label_map_type label_map_;
		memory::register_table reg_tbl_;
		std::size_t stack_size_ = 0;
		std::string start_label_;
		memory::table::range_type write_protect_{};
	};
}

#endif /* !ELANG_ASM_INSTRUCTION_TABLE_H */
