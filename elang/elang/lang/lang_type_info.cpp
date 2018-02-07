#include "lang_type_symbol_table.h"

elang::lang::type_info::type_info(attribute_type attributes)
	: attributes_(attributes){}

elang::lang::type_info::ptr_type elang::lang::type_info::reflect(){
	return shared_from_this();
}

elang::lang::type_info::ptr_type elang::lang::type_info::resolve_auto(const type_info &type, bool is_pointer) const{
	return type.clone(is_pointer ? (type.attributes_ | attributes_) : attributes_);
}

elang::lang::type_info::attribute_type elang::lang::type_info::attributes() const{
	return attributes_;
}

elang::lang::type_info::type_symbol_table_ptr_type elang::lang::type_info::symbol_table() const{
	return nullptr;
}

std::string elang::lang::type_info::mangle() const{
	return (mangle_attributes() + mangle_name());
}

std::string elang::lang::type_info::mangle_attributes() const{
	std::string value;
	if (ELANG_IS(attributes_, attribute_type::const_))
		value = "K";

	if (ELANG_IS(attributes_, attribute_type::ref_))
		value += "R";
	else if (ELANG_IS(attributes_, attribute_type::vref))
		value += "U";

	return value;
}

unsigned __int64 elang::lang::type_info::align_address(unsigned __int64 value, const type_info &type) const{
	return value;
}

bool elang::lang::type_info::has_conversion_to(const type_info &type) const{
	return false;
}

bool elang::lang::type_info::is_const() const{
	return ELANG_IS(attributes_, attribute_type::const_);
}

bool elang::lang::type_info::is_ref() const{
	return ELANG_IS(attributes_, attribute_type::ref_);
}

bool elang::lang::type_info::is_vref() const{
	return ELANG_IS(attributes_, attribute_type::vref);
}

bool elang::lang::type_info::is_auto() const{
	return false;
}

bool elang::lang::type_info::is_primitive() const{
	return false;
}

bool elang::lang::type_info::is_void() const{
	return false;
}

bool elang::lang::type_info::is_bool() const{
	return false;
}

bool elang::lang::type_info::is_char() const{
	return false;
}

bool elang::lang::type_info::is_wchar() const{
	return false;
}

bool elang::lang::type_info::is_numeric() const{
	return false;
}

bool elang::lang::type_info::is_integral() const{
	return false;
}

bool elang::lang::type_info::is_unsigned_integral() const{
	return false;
}

bool elang::lang::type_info::is_float() const{
	return false;
}

bool elang::lang::type_info::is_function() const{
	return false;
}

bool elang::lang::type_info::is_array() const{
	return false;
}

bool elang::lang::type_info::is_pointer() const{
	return false;
}

bool elang::lang::type_info::is_null_pointer() const{
	return false;
}

bool elang::lang::type_info::is_user() const{
	return false;
}
