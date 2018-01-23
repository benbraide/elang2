#pragma once

#ifndef ELANG_BYTE_CODE_SYSCALL_INSTRUCTION_H
#define ELANG_BYTE_CODE_SYSCALL_INSTRUCTION_H

#include <iostream>

#include "../common/stream_input_reader.h"
#include "../common/stream_output_writer.h"

#include "byte_code_instruction.h"

namespace elang::byte_code{
	enum class syscall_id : unsigned int{
		exit,
		read,
		write,
		helper,
	};

	enum class syscall_helper_id : unsigned int{
		read_char,
		read_int,
		read_float,
		write_string,
		write_int,
		write_float,
		to_string,
		to_string_len,
	};

	class syscall_handler{
	public:
		static void handle(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack);

		static void handle_helper(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack);

		static void exit();

		static void read(memory::table &mem_tbl, memory::register_table &reg_tbl);

		static void write(memory::register_table &reg_tbl);

		static common::stream_input_reader<decltype(std::cin)> std_in;

		static common::stream_output_writer<decltype(std::cout)> std_out;
		static common::stream_output_writer<decltype(std::cerr)> std_err;
		static common::stream_output_writer<decltype(std::clog)> std_log;

		static common::input_reader *reader;
		static common::output_writer *writer;
	};

	template <>
	struct instruction<id::syscall>{
		static void evaluate(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
			syscall_handler::handle(mem_tbl, reg_tbl, stack);
		}
	};
}

#endif /* !ELANG_BYTE_CODE_SYSCALL_INSTRUCTION_H */
