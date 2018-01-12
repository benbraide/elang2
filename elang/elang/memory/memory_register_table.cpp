#include "memory_register_table.h"

elang::memory::register_table::register_table()
	: flags_(register_flag::nil), data_(new __int64[24]){
	auto offset = 0ui64;
	memset(data_.get(), 0, (sizeof(__int64) * 24));

	add_("rax", "r0", "eax", "ax", "al", "ah", offset);
	add_("rbx", "r1", "ebx", "bx", "bl", "bh", offset);
	add_("rcx", "r2", "ecx", "cx", "cl", "ch", offset);
	add_("rdx", "r3", "edx", "dx", "dl", "dh", offset);

	add_("rsp", "r4", "esp", "sp", "", "", offset);
	add_("rbp", "r5", "ebp", "bp", "", "", offset);
	add_("rsi", "r6", "esi", "si", "", "", offset);
	add_("rdi", "r7", "edi", "di", "", "", offset);
	add_("rip", "r8", "eip", "ip", "", "", offset);

	add_qword_("r", 9, 15, offset);
	add_float_("xmm", 0, 7, offset);
}

elang::memory::memory_register *elang::memory::register_table::find(const std::string &key) const{
	auto entry = map_.find(key);
	return ((entry == map_.end()) ? nullptr : entry->second.get());
}

elang::memory::memory_register *elang::memory::register_table::find(std::size_t offset) const{
	auto entry = offset_map_.find(offset);
	return ((entry == offset_map_.end()) ? nullptr : entry->second);
}

void elang::memory::register_table::set_flag(register_flag flag){
	ELANG_SET(flags_, flag);
}

void elang::memory::register_table::clear_flag(register_flag flag){
	ELANG_REMOVE(flags_, flag);
}

bool elang::memory::register_table::has_flag(register_flag flag) const{
	return ELANG_IS(flags_, flag);
}

std::size_t elang::memory::register_table::offset(const char *ptr) const{
	if (ptr < reinterpret_cast<char *>(data_.get()))
		return -1;//Error
	return static_cast<std::size_t>(ptr - reinterpret_cast<char *>(data_.get()));
}

void elang::memory::register_table::add_(const std::string &name, const std::string &alias, const std::string &_32,
	const std::string &_16, const std::string &low, const std::string &high, unsigned __int64 &offset){
	auto value = std::make_shared<memory_register>(nullptr, name, sizeof(__int64), (reinterpret_cast<char *>(data_.get() + offset++)));

	auto _32_value	= std::make_shared<memory_register>(value.get(), _32, sizeof(__int32), value->data());
	auto _16_value	= std::make_shared<memory_register>(_32_value.get(), _16, sizeof(__int16), _32_value->data());
	auto _8_value	= std::make_shared<memory_register>(_16_value.get(), (low.empty() ? (alias + "b") : low), sizeof(__int8), _16_value->data());

	if (!low.empty())
		map_[low] = _8_value;

	if (!high.empty()){
		map_[high] = std::make_shared<memory_register>(_16_value.get(), high, sizeof(__int8), (_16_value->data() + sizeof(__int8)));
		offset_map_[static_cast<std::size_t>(map_[high]->data() - reinterpret_cast<char *>(data_.get()))] = map_[high].get();
	}

	map_[_16]	= _16_value;
	map_[_32]	= _32_value;
	map_[name]	= value;

	offset_map_[static_cast<std::size_t>(value->data() - reinterpret_cast<char *>(data_.get()))] = value.get();
	if (!alias.empty()){//Add aliases
		map_[(alias + "b")]	= _8_value;
		map_[(alias + "w")]	= _16_value;
		map_[(alias + "d")]	= _32_value;
		map_[alias]			= value;
	}
}

void elang::memory::register_table::add_qword_(const std::string &prefix, int from, int to, unsigned __int64 &offset){
	std::string name;

	register_ptr_type value;
	for (; from <= to; ++from){//Add entries
		name = (prefix + std::to_string(from));
		value = std::make_shared<memory_register>(nullptr, name, sizeof(__int64), (reinterpret_cast<char *>(data_.get() + offset++)));

		map_[(name + "d")]	= std::make_shared<memory_register>(value.get(), (name + "d"), sizeof(__int32), value->data());
		map_[(name + "w")]	= std::make_shared<memory_register>(map_[(name + "d")].get(), (name + "w"), sizeof(__int16), map_[(name + "d")]->data());
		map_[(name + "b")]	= std::make_shared<memory_register>(map_[(name + "w")].get(), (name + "b"), sizeof(__int8), map_[(name + "w")]->data());
		map_[name]			= value;

		offset_map_[static_cast<std::size_t>(map_[name]->data() - reinterpret_cast<char *>(data_.get()))] = map_[name].get();
	}
}

void elang::memory::register_table::add_float_(const std::string &prefix, int from, int to, unsigned __int64 &offset){
	std::string name;
	for (; from <= to; ++from){
		name = (prefix + std::to_string(from));

		map_[name]			= std::make_shared<memory_register>(nullptr, name, sizeof(__int64), (reinterpret_cast<char *>(data_.get() + offset++)));
		map_[(name + "d")]	= std::make_shared<memory_register>(map_[name].get(), (name + "d"), sizeof(float), map_[name]->data());

		offset_map_[static_cast<std::size_t>(map_[name]->data() - reinterpret_cast<char *>(data_.get()))] = map_[name].get();
	}
}
