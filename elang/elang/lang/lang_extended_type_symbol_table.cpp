#include "lang_extended_type_symbol_table.h"

elang::lang::extended_type_symbol_table::extended_type_symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: type_symbol_table(name, parent, attributes){}

elang::lang::symbol_table *elang::lang::extended_type_symbol_table::find_table(const std::string &name) const{
	auto table = type_symbol_table::find_table(name);
	if (table != nullptr)
		return table;//Found inside this

	for (auto &info : type_map_){
		if (info.first == name)
			return info.second;

		if ((table = info.second->find_table(name)) != nullptr)
			return table;//Found in base
	}

	return nullptr;
}

elang::lang::symbol_table::non_table_type *elang::lang::extended_type_symbol_table::find_non_table(const std::string &name) const{
	auto non_table = type_symbol_table::find_non_table(name);
	if (non_table != nullptr)
		return non_table;//Found inside this

	for (auto &info : type_map_){
		if ((non_table = info.second->find_non_table(name)) != nullptr)
			return non_table;//Found in base
	}

	return nullptr;
}

unsigned __int64 elang::lang::extended_type_symbol_table::compute_relative_offset(const symbol_table &table) const{
	auto type = dynamic_cast<const type_symbol_table *>(&table);
	if (type == nullptr)//Target is not a user type
		return static_cast<unsigned __int64>(-1);

	auto offset = 0ui64;
	for (auto base_type : type_order_list_){
		if (base_type != type)
			offset += base_type->size();
		else//Matched
			return offset;
	}

	return static_cast<unsigned __int64>(-1);//No match
}

unsigned __int64 elang::lang::extended_type_symbol_table::compute_relative_offset(const variable_entry_info &var) const{
	auto offset = type_symbol_table::compute_relative_offset(var);
	if (offset != static_cast<unsigned __int64>(-1))
		return offset;

	auto base_offset = 0ui64;
	for (auto base_type : type_order_list_){
		if ((offset = base_type->compute_relative_offset(var)) == static_cast<unsigned __int64>(-1))
			base_offset += base_type->size();
		else//Matched
			return (base_offset + offset);
	}

	return static_cast<unsigned __int64>(-1);//No match
}

unsigned __int64 elang::lang::extended_type_symbol_table::align_address(unsigned __int64 value, const type_symbol_table &table) const{
	if (&table == this)
		return value;//Not a user type OR same type

	auto offset = compute_relative_offset(table);
	if (offset != static_cast<unsigned __int64>(-1))
		return (value + offset);//Type is a base of this

	if ((offset = table.compute_relative_offset(*this)) != static_cast<unsigned __int64>(-1))
		return (value + offset);//This is a base of type

	return value;//Type is not related to this
}

bool elang::lang::extended_type_symbol_table::is_base(const type_symbol_table &value) const{
	for (auto base_type : type_order_list_){
		if (base_type == &value || base_type->is_base(value))
			return true;
	}

	return false;
}

void elang::lang::extended_type_symbol_table::add_base(type_symbol_table &value){
	if (type_map_.find(value.name()) != type_map_.end())
		throw common::error::lang_base_type_exists;

	type_map_[value.name()] = &value;
	size_ += value.size();
}
