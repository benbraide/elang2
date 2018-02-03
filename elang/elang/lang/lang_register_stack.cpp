#include "lang_register_stack.h"

elang::lang::register_stack::register_stack(){
	integral_info_.index = float_info_.index = 0;
	integral_info_.list.reserve(11);
	float_info_.list.reserve(8);

	integral_info_.list.push_back(table_.find("rax"));
	integral_info_.list.push_back(table_.find("rbx"));
	integral_info_.list.push_back(table_.find("rcx"));
	integral_info_.list.push_back(table_.find("rdx"));

	integral_info_.list.push_back(table_.find("r9"));
	integral_info_.list.push_back(table_.find("r10"));
	integral_info_.list.push_back(table_.find("r11"));
	integral_info_.list.push_back(table_.find("r12"));
	integral_info_.list.push_back(table_.find("r13"));
	integral_info_.list.push_back(table_.find("r14"));
	integral_info_.list.push_back(table_.find("r15"));

	float_info_.list.push_back(table_.find("xmm0"));
	float_info_.list.push_back(table_.find("xmm1"));
	float_info_.list.push_back(table_.find("xmm2"));
	float_info_.list.push_back(table_.find("xmm3"));
	float_info_.list.push_back(table_.find("xmm4"));
	float_info_.list.push_back(table_.find("xmm5"));
	float_info_.list.push_back(table_.find("xmm6"));
	float_info_.list.push_back(table_.find("xmm7"));
}

elang::memory::memory_register *elang::lang::register_stack::pop_integral(std::size_t size){
	if (integral_info_.index < integral_info_.list.size()){
		++integral_info_.index;
		return (*std::next(integral_info_.list.begin(), (integral_info_.index - 1u)))->match(size);
	}
	
	throw common::error::lang_out_of_registers;
}

elang::memory::memory_register *elang::lang::register_stack::pop_float(std::size_t size){
	if (float_info_.index < float_info_.list.size()){
		++float_info_.index;
		return (*std::next(float_info_.list.begin(), (float_info_.index - 1u)))->match(size);
	}

	throw common::error::lang_out_of_registers;
}

void elang::lang::register_stack::push_integral(){
	if (integral_info_.index > 0u)
		--integral_info_.index;
}

void elang::lang::register_stack::push_float(){
	if (float_info_.index > 0u)
		--float_info_.index;
}
