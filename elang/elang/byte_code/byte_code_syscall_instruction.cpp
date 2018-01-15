#include "byte_code_translator.h"

void elang::byte_code::syscall_handler::handle(char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
	switch (static_cast<syscall_id>(reg_tbl.find("eax")->read<unsigned int>())){
	case syscall_id::exit:
		return exit();
	case syscall_id::read:
		return read(reg_tbl);
	case syscall_id::write:
		return write(reg_tbl);
	default:
		break;
	}

	throw common::error::byte_code_syscall_unknown;
}

void elang::byte_code::syscall_handler::exit(){
	translator::running_main = false;
}

void elang::byte_code::syscall_handler::read(memory::register_table &reg_tbl){

}

void elang::byte_code::syscall_handler::write(memory::register_table &reg_tbl){

}
