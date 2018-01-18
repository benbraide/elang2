#pragma once

#ifndef ELANG_ASM_INSTRUCTION_TABLE_H
#define ELANG_ASM_INSTRUCTION_TABLE_H

#include <list>
#include <unordered_map>

#include "../memory/memory_register_table.h"

#include "asm_section_id.h"
#include "asm_instruction_object.h"

namespace elang::easm{
	class instruction_table{
	public:
		typedef std::shared_ptr<instruction_object> iptr_type;
		typedef std::list<iptr_type> iptr_list_type;

		struct label_info{
			unsigned __int64 offset;
			unsigned __int64 *offset_ptr;
		};

		typedef std::unordered_map<std::string, label_info> label_map_type;

		struct section_info{
			unsigned __int64 offset;
			iptr_list_type instructions;
			std::list<label_info *> labels;
		};

		typedef std::unordered_map<section_id, section_info> section_map_type;

		void add(section_id section);

		void add(const std::string &label);

		void add(iptr_type instruction);

		void encode(char *buffer, std::size_t size, std::size_t offset);

		std::size_t size() const;

		memory::memory_register *find_register(const std::string &name) const;

		unsigned __int64 *&find_label(const std::string &name);

	private:
		section_info *active_section_ = nullptr;
		section_map_type section_map_;
		label_map_type label_map_;
		memory::register_table reg_tbl_;
	};
}

#endif /* !ELANG_ASM_INSTRUCTION_TABLE_H */
