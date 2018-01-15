#include "byte_code_translator.h"

bool elang::byte_code::operand_info::is_write_protected(__int64 address){
	return (address >= translator::write_protection_start && address <= translator::write_protection_end);
}

char *elang::byte_code::operand_info::correct_ptr(char *ptr, char *base_ptr){
	if (translator::stack == nullptr || ptr < (base_ptr + translator::module_size))
		return ptr;//Pointer is within data range

	auto value = (translator::stack->ptr() + ((base_ptr + translator::module_size) - ptr));
	if (value >= (translator::stack->ptr() + translator::stack->size()))//Bad address
		throw common::error::byte_code_memory_read_access_violation;

	return value;
}
