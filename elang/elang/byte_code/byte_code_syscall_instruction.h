#pragma once

#ifndef ELANG_BYTE_CODE_SYSCALL_INSTRUCTION_H
#define ELANG_BYTE_CODE_SYSCALL_INSTRUCTION_H

#include "byte_code_instruction.h"

namespace elang::byte_code{
	enum class syscall_id : unsigned int{
		exit,
		read,
		write,
	};

	class syscall_handler{
	public:
		static void handle(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack);

		static void exit();

		static void read(memory::register_table &reg_tbl);

		static void write(memory::register_table &reg_tbl);
	};

	template <>
	struct instruction<id::syscall>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			syscall_handler::handle(mem_tbl, reg_tbl, stack);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_SYSCALL_INSTRUCTION_H */
