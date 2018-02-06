#pragma once

#ifndef ELANG_LANG_THREAD_INFO_H
#define ELANG_LANG_THREAD_INFO_H

#include "../asm/asm_instruction_table.h"

#include "lang_register_stack.h"
#include "lang_label_store.h"

namespace elang::lang{
	struct thread_info{
		template <typename value_type>
		static void add(easm::section_id section, const value_type &value){
			if (decltype_count == 0u){
				ins_table.add(section);
				ins_table.add(value);
			}
		}

		static thread_local register_stack reg_stack;
		static thread_local label_store lbl_store;
		static thread_local easm::instruction_table ins_table;
		static thread_local unsigned int decltype_count;
	};
}

#endif /* !ELANG_LANG_THREAD_INFO_H */
