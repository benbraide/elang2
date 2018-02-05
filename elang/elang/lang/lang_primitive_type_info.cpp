#include "lang_primitive_type_info.h"

elang::lang::primitive_type_info::primitive_type_info(id_type id, attribute_type attributes)
	: type_info(attributes), id_(id){}

elang::lang::type_info::ptr_type elang::lang::primitive_type_info::clone(attribute_type attributes) const{
	return std::make_shared<primitive_type_info>(id_, attributes);
}

std::size_t elang::lang::primitive_type_info::size() const{
	if (is_ref())
		return sizeof(void *);

	switch (id_){
	case id_type::bool_:
		return sizeof(bool_type);
	case id_type::char_:
		return sizeof(char);
	case id_type::wchar_:
		return sizeof(wchar_t);
	case id_type::int8_:
		return sizeof(__int8);
	case id_type::int16_:
		return sizeof(__int16);
	case id_type::int32_:
		return sizeof(__int32);
	case id_type::int64_:
		return sizeof(__int64);
	case id_type::float_:
		return sizeof(long double);
	case id_type::nullptr_:
		return sizeof(nullptr);
	default:
		break;
	}

	throw common::error::lang_bad_type;
}

std::string elang::lang::primitive_type_info::mangle_name() const{
	switch (id_){
	case id_type::auto_:
		return "a";
	case id_type::void_:
		return "v";
	case id_type::bool_:
		return "b";
	case id_type::char_:
		return "c";
	case id_type::wchar_:
		return "w";
	case id_type::int8_:
		return "g";
	case id_type::int16_:
		return "s";
	case id_type::int32_:
		return "i";
	case id_type::int64_:
		return "l";
	case id_type::float_:
		return "f";
	case id_type::nullptr_:
		return "n";
	default:
		break;
	}

	throw common::error::lang_bad_type;
}

int elang::lang::primitive_type_info::score(const type_info &type) const{
	if (is_auto())//Accepts any type
		return ELANG_TYPE_INFO_MAX_SCORE;

	auto primitive_type = dynamic_cast<const primitive_type_info *>(&type);
	if (primitive_type == nullptr)//Type is not primitive
		return (type.has_conversion_to(*this) ? (ELANG_TYPE_INFO_MAX_SCORE - 2) : ELANG_TYPE_INFO_MIN_SCORE);

	if (type.is_vref())//Types must match
		return ((is_vref() && (is_const() || !type.is_const()) && id_ == primitive_type->id_) ? ELANG_TYPE_INFO_MAX_SCORE : ELANG_TYPE_INFO_MIN_SCORE);

	if (is_ref()){//Type must be a ref
		if (!is_const() && (!type.is_ref() || type.is_const()))
			return ELANG_TYPE_INFO_MIN_SCORE;//'const ref' to 'ref' OR 'rval' to 'ref'

		if (id_ == primitive_type->id_)//Same type
			return (type.is_ref() ? ELANG_TYPE_INFO_MAX_SCORE : (ELANG_TYPE_INFO_MAX_SCORE - 1));

		return (is_const() ? (ELANG_TYPE_INFO_MAX_SCORE - conversion_offset_(*primitive_type)) : ELANG_TYPE_INFO_MIN_SCORE);
	}

	if (id_ == primitive_type->id_)//Same type
		return (type.is_ref() ? (ELANG_TYPE_INFO_MAX_SCORE - 1) : ELANG_TYPE_INFO_MAX_SCORE);

	return (ELANG_TYPE_INFO_MAX_SCORE - conversion_offset_(*primitive_type));
}

bool elang::lang::primitive_type_info::has_conversion_to(const type_info &type) const{
	if (is_vref() || (type.is_ref() && !type.is_const()))//Same type required
		return false;

	if (is_numeric() && type.is_numeric())
		return true;//Numeric conversion

	if (is_null_pointer() && type.is_pointer())
		return true;//Null pointer to pointer

	return false;
}

bool elang::lang::primitive_type_info::is_primitive() const{
	return true;
}

bool elang::lang::primitive_type_info::is_void() const{
	return (id_ == id_type::void_);
}

bool elang::lang::primitive_type_info::is_bool() const{
	return (id_ == id_type::bool_);
}

bool elang::lang::primitive_type_info::is_numeric() const{
	switch (id_){
	case id_type::int8_:
	case id_type::int16_:
	case id_type::int32_:
	case id_type::int64_:
	case id_type::float_:
		return true;
	default:
		break;
	}

	return false;
}

bool elang::lang::primitive_type_info::is_integral() const{
	switch (id_){
	case id_type::int8_:
	case id_type::int16_:
	case id_type::int32_:
	case id_type::int64_:
		return true;
	default:
		break;
	}

	return false;
}

bool elang::lang::primitive_type_info::is_float() const{
	return (id_ == id_type::float_);
}

bool elang::lang::primitive_type_info::is_null_pointer() const{
	return (id_ == id_type::nullptr_);
}

elang::lang::primitive_type_info::id_type elang::lang::primitive_type_info::id() const{
	return id_;
}

int elang::lang::primitive_type_info::conversion_offset_(const primitive_type_info &type) const{
	if (!is_numeric())//Only numeric types can be converted
		return ELANG_TYPE_INFO_MAX_SCORE;

	if (id_ < type.id_)
		return 4;

	if (id_ != id_type::float_ && type.id_ != id_type::float_)
		return 2;//Integer <--> Integer

	return 3;//Integer <--> Float
}

elang::lang::unsigned_integral_type_info::unsigned_integral_type_info(id_type id, attribute_type attributes)
	: primitive_type_info(id, attributes){}

elang::lang::type_info::ptr_type elang::lang::unsigned_integral_type_info::clone(attribute_type attributes) const{
	return std::make_shared<unsigned_integral_type_info>(id_, attributes);
}

std::string elang::lang::unsigned_integral_type_info::mangle_name() const{
	switch (id_){
	case id_type::int8_:
		return "d";
	case id_type::int16_:
		return "e";
	case id_type::int32_:
		return "u";
	case id_type::int64_:
		return "q";
	default:
		break;
	}

	throw common::error::lang_bad_type;
}

bool elang::lang::unsigned_integral_type_info::is_unsigned_integral() const{
	return true;
}
