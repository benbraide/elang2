#include <iostream>

#include "byte_code/byte_code_translator.h"
#include "asm/asm_instruction_translator.h"
#include "common/string_output_writer.h"

int main(){
	std::wstring log;
	elang::common::string_output_writer writer(log);
	elang::byte_code::debug dbg(writer);

	std::string source(".global main\nsection .text\nmain:\nmov rax, 9\nadd [rsp + 45], al\nmov ax, 0\nsyscall");

	elang::easm::instruction_translator::encode_file("basic", "basic");
	elang::byte_code::translator::translate_file("basic");

	return 0;
}