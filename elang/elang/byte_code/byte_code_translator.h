#pragma once

#ifndef ELANG_BYTE_CODE_TRANSLATOR_H
#define ELANG_BYTE_CODE_TRANSLATOR_H

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost/iostreams/device/mapped_file.hpp>

#include "../common/file_resource.h"

#include "byte_code_arithmetic_instruction.h"
#include "byte_code_call_instruction.h"
#include "byte_code_cmp_instruction.h"
#include "byte_code_cnvt_instruction.h"
#include "byte_code_jmp_instruction.h"
#include "byte_code_lea_instruction.h"
#include "byte_code_loop_instruction.h"
#include "byte_code_mov_instruction.h"
#include "byte_code_set_instruction.h"
#include "byte_code_stack_instruction.h"
#include "byte_code_syscall_instruction.h"
#include "byte_code_test_instruction.h"

namespace elang::byte_code{
	class translator{
	public:
		static void translate_file(const std::string &file, const std::string &dir = "test/bcd");

		static void translate(const char *ptr);

		static void translate(memory::table &mem_tbl, unsigned __int64 entry);

		static bool running_main;
		static thread_local bool running_thread;

		static std::size_t module_size;
		static std::size_t stack_size;

		static unsigned __int64 write_protection_start;
		static unsigned __int64 write_protection_end;

		static thread_local memory::stack *stack;

	private:
		static id extract_id_(memory::table &mem_tbl, memory::register_table &reg_tbl);

		static void translate_(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack);

		template <id id>
		static void translate_instruction_(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			instruction<id>::evaluate(mem_tbl, reg_tbl, stack);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_TRANSLATOR_H */
