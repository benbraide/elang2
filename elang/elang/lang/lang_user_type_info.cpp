#include "lang_user_type_info.h"

elang::lang::user_type_info::user_type_info(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: type_info(attribute_type::nil), symbol_table(name, parent, attributes){}

elang::lang::type_info::ptr_type elang::lang::user_type_info::clone(attribute_type attributes) const{
	if (attributes == attribute_type::nil)//Unmodified
		return const_cast<user_type_info *>(this)->reflect();
	return std::make_shared<modified_user_type_info>(const_cast<user_type_info *>(this)->reflect(), attributes);
}

std::size_t elang::lang::user_type_info::size() const{
	return (is_ref() ? sizeof(void *) : size_);
}

std::string elang::lang::user_type_info::mangle_name() const{
	if (parent_ == nullptr)
		return ("T" + std::to_string(name_.size()) + name_);
	return (parent_->mangle() + "T" + std::to_string(name_.size()) + name_);
}

int elang::lang::user_type_info::score(const type_info &type) const{
	auto base_type = &type;
	if (base_type == nullptr){//Type is not a user type
		auto modified_user_type = dynamic_cast<const modified_user_type_info *>(&type);
		if (modified_user_type == nullptr)
			return (type.has_conversion_to(*this) ? (ELANG_TYPE_INFO_MAX_SCORE - 2) : ELANG_TYPE_INFO_MIN_SCORE);

		base_type = modified_user_type->value().get();
	}

	if (base_type != this)//Mismatch
		return (type.has_conversion_to(*this) ? (ELANG_TYPE_INFO_MAX_SCORE - 2) : ELANG_TYPE_INFO_MIN_SCORE);

	if (type.is_vref())//Types must match
		return ((is_vref() && (is_const() || !type.is_const())) ? ELANG_TYPE_INFO_MAX_SCORE : ELANG_TYPE_INFO_MIN_SCORE);

	if (is_ref()){//Type must be a ref
		if (!is_const() && (!type.is_ref() || (type.is_ref() && type.is_const())))
			return ELANG_TYPE_INFO_MIN_SCORE;//'const ref' to 'ref' OR 'rval' to 'ref'

		return (type.is_ref() ? ELANG_TYPE_INFO_MAX_SCORE : (ELANG_TYPE_INFO_MAX_SCORE - 1));
	}

	return ELANG_TYPE_INFO_MAX_SCORE;
}

bool elang::lang::user_type_info::has_conversion_to(const type_info &type) const{
	return false;
}

bool elang::lang::user_type_info::is_user() const{
	return true;
}

void elang::lang::user_type_info::add(const variable_entry_info &variable){
	symbol_table::add(variable);
	size_ += variable.type->size();
}

unsigned __int64 elang::lang::user_type_info::compute_offset(const variable_entry_info &var) const{
	auto offset = ((parent_ == nullptr) ? 0ui64 : parent_->compute_offset(*this));
	for (auto variable : order_list_){
		if (variable != &var)
			offset += variable->type->size();
		else//Matched
			return offset;
	}

	return 0u;//No match
}

elang::lang::modified_user_type_info::modified_user_type_info(ptr_type value, attribute_type attributes)
	: type_info(attributes), value_(value){}

elang::lang::type_info::ptr_type elang::lang::modified_user_type_info::clone(attribute_type attributes) const{
	if (attributes == attribute_type::nil)
		return value_;//Unmodified
	return std::make_shared<modified_user_type_info>(value_, attributes);
}

std::size_t elang::lang::modified_user_type_info::size() const{
	return value_->size();
}

std::string elang::lang::modified_user_type_info::mangle() const{
	return value_->mangle();
}

std::string elang::lang::modified_user_type_info::mangle_name() const{
	return value_->mangle_name();
}

std::string elang::lang::modified_user_type_info::mangle_attributes() const{
	return value_->mangle_attributes();
}

int elang::lang::modified_user_type_info::score(const type_info &type) const{
	return value_->score(type);
}

bool elang::lang::modified_user_type_info::has_conversion_to(const type_info &type) const{
	return value_->has_conversion_to(type);
}

bool elang::lang::modified_user_type_info::is_auto() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_primitive() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_void() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_bool() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_numeric() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_integral() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_unsigned_integral() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_float() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_function() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_array() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_pointer() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_null_pointer() const{
	return false;
}

bool elang::lang::modified_user_type_info::is_user() const{
	return true;
}

elang::lang::type_info::ptr_type elang::lang::modified_user_type_info::value() const{
	return value_;
}

elang::lang::union_user_type_info::union_user_type_info(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: user_type_info(name, parent, attributes){}

elang::lang::type_info::ptr_type elang::lang::union_user_type_info::clone(attribute_type attributes) const{
	if (attributes == attribute_type::nil)//Unmodified
		return const_cast<union_user_type_info *>(this)->reflect();
	return std::make_shared<modified_user_type_info>(const_cast<union_user_type_info *>(this)->reflect(), attributes);
}

void elang::lang::union_user_type_info::add(const variable_entry_info &variable){
	symbol_table::add(variable);
	if (size_ < variable.type->size())//Max
		size_ = variable.type->size();
}

unsigned __int64 elang::lang::union_user_type_info::compute_offset(const variable_entry_info &var) const{
	return 0;
}

elang::lang::enum_user_type_info::enum_user_type_info(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: user_type_info(name, parent, attributes){
	size_ = sizeof(__int32);
}

elang::lang::type_info::ptr_type elang::lang::enum_user_type_info::clone(attribute_type attributes) const{
	if (attributes == attribute_type::nil)//Unmodified
		return const_cast<enum_user_type_info *>(this)->reflect();
	return std::make_shared<modified_user_type_info>(const_cast<enum_user_type_info *>(this)->reflect(), attributes);
}

void elang::lang::enum_user_type_info::add(const variable_entry_info &variable){
	symbol_table::add(variable);
	if (order_list_.size() > std::numeric_limits<__int32>::max() && size_ == sizeof(__int32))
		size_ = sizeof(__int64);
}

unsigned __int64 elang::lang::enum_user_type_info::compute_offset(const variable_entry_info &var) const{
	auto entry = std::find(order_list_.begin(), order_list_.end(), &var);
	return static_cast<unsigned __int64>(std::distance(order_list_.begin(), entry));
}

elang::lang::extended_user_type_info::extended_user_type_info(const std::string &name, symbol_table *parent, entry_attribute_type attributes)
	: user_type_info(name, parent, attributes){}

elang::lang::symbol_table *elang::lang::extended_user_type_info::find_table(const std::string &name) const{
	auto table = user_type_info::find_table(name);
	if (table != nullptr)
		return table;//Found inside this

	auto entry = type_map_.find(name);
	return ((entry == type_map_.end()) ? nullptr : entry->second);
}

unsigned __int64 elang::lang::extended_user_type_info::compute_offset(const symbol_table &table) const{
	auto type = dynamic_cast<const user_type_info *>(&table);
	if (type == nullptr)
		return 0u;//Target is not a type

	auto offset = ((parent_ == nullptr) ? 0ui64 : parent_->compute_offset(*this));
	for (auto base_type : type_order_list_){
		if (base_type != type)
			offset += base_type->size();
		else//Matched
			return offset;
	}

	return 0u;//No match
}

void elang::lang::extended_user_type_info::add_base(user_type_info &value){
	if (type_map_.find(value.name()) != type_map_.end())
		throw common::error::lang_base_type_exists;

	type_map_[value.name()] = &value;
	size_ += value.size();
}
