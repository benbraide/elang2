#include "memory_register_table.h"

elang::memory::register_table::register_table(){
	add_("rax", "r0", "eax", "ax", "al", "ah");
	add_("rbx", "r1", "ebx", "bx", "bl", "bh");
	add_("rcx", "r2", "ecx", "cx", "cl", "ch");
	add_("rdx", "r3", "edx", "dx", "dl", "dh");

	add_("rsp", "r4", "esp", "sp", "", "");
	add_("rbp", "r5", "ebp", "bp", "", "");
	add_("rsi", "r6", "esi", "si", "", "");
	add_("rdi", "r7", "edi", "di", "", "");
	add_("rip", "r8", "eip", "ip", "", "");

	add_qword_("r", 9, 15);
	add_float_("xmm", 0, 7);
}

elang::memory::memory_register *elang::memory::register_table::find(const std::string &key) const{
	auto entry = map_.find(key);
	return ((entry == map_.end()) ? nullptr : entry->second.get());
}

void elang::memory::register_table::add_(const std::string &name, const std::string &alias, const std::string &_32,
	const std::string &_16, const std::string &low, const std::string &high){
	auto value = std::make_shared<typed_memory_register<__int64>>(name);

	auto _32_value	= std::make_shared<memory_register>(value.get(), _32, sizeof(__int32), value->data());
	auto _16_value	= std::make_shared<memory_register>(_32_value.get(), _16, sizeof(__int16), _32_value->data());
	auto _8_value	= std::make_shared<memory_register>(_16_value.get(), (low.empty() ? (alias + "b") : low), sizeof(__int8), _16_value->data());

	if (!low.empty())
		map_[low] = _8_value;

	if (!high.empty())
		map_[high] = std::make_shared<memory_register>(_16_value.get(), high, sizeof(__int8), (_16_value->data() + sizeof(__int8)));

	map_[_16]	= _16_value;
	map_[_32]	= _32_value;
	map_[name]	= value;

	if (!alias.empty()){//Add aliases
		map_[(alias + "b")]	= _8_value;
		map_[(alias + "w")]	= _16_value;
		map_[(alias + "d")]	= _32_value;
		map_[alias]			= value;
	}
}

void elang::memory::register_table::add_qword_(const std::string &prefix, int from, int to){
	std::string name;

	register_ptr_type value;
	for (; from <= to; ++from){//Add entries
		name = (prefix + std::to_string(from));
		value = std::make_shared<typed_memory_register<__int64>>(name);

		map_[(name + "d")]	= std::make_shared<memory_register>(value.get(), (name + "d"), sizeof(__int32), value->data());
		map_[(name + "w")]	= std::make_shared<memory_register>(map_[(name + "d")].get(), (name + "w"), sizeof(__int16), map_[(name + "d")]->data());
		map_[(name + "b")]	= std::make_shared<memory_register>(map_[(name + "w")].get(), (name + "b"), sizeof(__int8), map_[(name + "w")]->data());
		map_[name]			= value;
	}
}

void elang::memory::register_table::add_float_(const std::string &prefix, int from, int to){
	std::string name;
	for (; from <= to; ++from){
		name = (prefix + std::to_string(from));
		map_[name] = std::make_shared<typed_memory_register<__int64>>(name);
	}
}
