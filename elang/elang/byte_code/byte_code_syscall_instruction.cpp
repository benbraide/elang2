#include "byte_code_translator.h"

void elang::byte_code::syscall_handler::handle(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
	switch (static_cast<syscall_id>(reg_tbl.find("eax")->read<unsigned int>())){
	case syscall_id::exit:
		return exit();
	case syscall_id::read:
		return read(mem_tbl, reg_tbl);
	case syscall_id::write:
		return write(reg_tbl);
	case syscall_id::helper:
		return handle_helper(mem_tbl, reg_tbl, stack);
	default:
		break;
	}

	throw common::error::byte_code_syscall_unknown;
}

void elang::byte_code::syscall_handler::handle_helper(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
	switch (static_cast<syscall_helper_id>(reg_tbl.find("ebx")->read<unsigned int>())){
	case syscall_helper_id::read_char:
		break;
	default:
		break;
	}
}

void elang::byte_code::syscall_handler::exit(){
	translator::running_main = false;
}

void elang::byte_code::syscall_handler::read(memory::table &mem_tbl, memory::register_table &reg_tbl){
	if (reader != nullptr)
		mem_tbl.read_from_input(reg_tbl.find("rbx")->read<unsigned __int64>(), *reader, reg_tbl.find("rcx")->read<std::size_t>());
	else//Error
		throw common::error::no_input_reader;
}

void elang::byte_code::syscall_handler::write(memory::register_table &reg_tbl){
	if (writer != nullptr)
		writer->write(reg_tbl.find("bl")->read<char>());
	else//Error
		throw common::error::no_output_writer;
}

elang::common::stream_input_reader<decltype(std::cin)> elang::byte_code::syscall_handler::std_in(std::cin);

elang::common::stream_output_writer<decltype(std::cout)> elang::byte_code::syscall_handler::std_out(std::cout);

elang::common::stream_output_writer<decltype(std::cerr)> elang::byte_code::syscall_handler::std_err(std::cerr);

elang::common::stream_output_writer<decltype(std::clog)> elang::byte_code::syscall_handler::std_log(std::clog);

elang::common::input_reader *elang::byte_code::syscall_handler::reader = &std_in;

elang::common::output_writer *elang::byte_code::syscall_handler::writer = &std_out;
