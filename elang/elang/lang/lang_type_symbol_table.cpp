#include "lang_type_symbol_table.h"

elang::lang::type_symbol_table::type_symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: symbol_table(name, parent, attributes), size_(0){}

std::string elang::lang::type_symbol_table::mangle() const{
	if (parent_ == nullptr)
		return ("T" + std::to_string(name_.size()) + name_);
	return (parent_->mangle() + "T" + std::to_string(name_.size()) + name_);
}

void elang::lang::type_symbol_table::add(const variable_entry_info &variable){
	symbol_table::add(variable);
	size_ += variable.type->size();
}

unsigned __int64 elang::lang::type_symbol_table::compute_relative_offset(const variable_entry_info &var) const{
	auto offset = 0ui64;
	for (auto variable : order_list_){
		if (variable != &var)
			offset += variable->type->size();
		else//Matched
			return offset;
	}

	return static_cast<unsigned __int64>(-1);//No match
}

std::size_t elang::lang::type_symbol_table::size() const{
	return size_;
}

bool elang::lang::type_symbol_table::is_base(const type_symbol_table &value) const{
	return false;
}

unsigned __int64 elang::lang::type_symbol_table::align_address(unsigned __int64 value, const type_symbol_table &table) const{
	return value;
}

elang::lang::union_type_symbol_table::union_type_symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: type_symbol_table(name, parent, attributes){}

void elang::lang::union_type_symbol_table::add(const variable_entry_info &variable){
	type_symbol_table::add(variable);
	if (size_ < variable.type->size())//Max
		size_ = variable.type->size();
}

unsigned __int64 elang::lang::union_type_symbol_table::compute_offset(const variable_entry_info &var) const{
	return 0;
}

elang::lang::enum_type_symbol_table::enum_type_symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: type_symbol_table(name, parent, attributes){
	size_ = sizeof(__int32);
}

void elang::lang::enum_type_symbol_table::add(const variable_entry_info &variable){
	type_symbol_table::add(variable);
	if (order_list_.size() > std::numeric_limits<__int32>::max() && size_ == sizeof(__int32))
		size_ = sizeof(__int64);
}

unsigned __int64 elang::lang::enum_type_symbol_table::compute_offset(const variable_entry_info &var) const{
	auto offset = 0ui64;
	for (auto entry : order_list_){
		if (entry != &var)
			++offset;
		else//Match
			return offset;
	}

	return static_cast<unsigned __int64>(-1);
}
