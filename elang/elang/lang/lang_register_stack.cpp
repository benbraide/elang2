#include "lang_register_stack.h"

elang::lang::register_stack::register_stack(){
	integral_info_.list.reserve(11);
	integral_info_.used.reserve(11);

	float_info_.list.reserve(8);
	float_info_.used.reserve(8);

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

elang::lang::register_stack::item_ptr_type elang::lang::register_stack::pop_integral(std::size_t size){
	return pop_(size, integral_info_);
}

elang::lang::register_stack::item_ptr_type elang::lang::register_stack::pop_float(std::size_t size){
	return pop_(size, float_info_);
}

void elang::lang::register_stack::push(memory::memory_register &reg){
	if (reg.is_floating_point())
		push_(reg, float_info_);
	else//Integral
		push_(reg, integral_info_);
}

elang::lang::register_stack::item_ptr_type elang::lang::register_stack::pop_(std::size_t size, info &info){
	if (integral_info_.list.empty())
		throw common::error::lang_out_of_registers;

	auto value = *integral_info_.list.begin();
	if (size != 0u)
		value = value->match(size);

	integral_info_.used.push_back(*integral_info_.list.begin());
	integral_info_.list.erase(integral_info_.list.begin());

	return std::make_shared<item>(*this, *value);
}

void elang::lang::register_stack::push_(memory::memory_register &reg, info &info){
	auto entry = std::find(info.used.begin(), info.used.end(), reg.match(sizeof(unsigned __int64)));
	if (entry != info.used.end()){
		info.list.push_back(*entry);
		info.used.erase(entry);
	}
}
