#include "lang_user_type_info.h"

elang::lang::user_type_info::user_type_info(type_symbol_table_ptr_type symbol_table, attribute_type attributes)
	: type_info(attributes), symbol_table_(symbol_table){}

elang::lang::type_info::ptr_type elang::lang::user_type_info::clone(attribute_type attributes) const{
	return std::make_shared<user_type_info>(symbol_table_, attributes);
}

elang::lang::type_info::type_symbol_table_ptr_type elang::lang::user_type_info::symbol_table() const{
	return symbol_table_;
}

std::size_t elang::lang::user_type_info::size() const{
	return (is_ref() ? sizeof(void *) : symbol_table_->size());
}

std::string elang::lang::user_type_info::mangle_name() const{
	return symbol_table_->mangle();
}

unsigned __int64 elang::lang::user_type_info::align_address(unsigned __int64 value, const type_info &type) const{
	auto symbol_table = type.symbol_table();
	return ((symbol_table == nullptr) ? value : symbol_table_->align_address(value, *symbol_table));
}

int elang::lang::user_type_info::score(const type_info &type) const{
	auto user_type = dynamic_cast<const user_type_info *>(&type);
	if (user_type == nullptr)//Type is not a user type
		return ELANG_TYPE_INFO_MIN_SCORE;

	if (user_type->symbol_table_ != symbol_table_){//Mismatch
		if (!user_type->symbol_table_->is_base(*symbol_table_))
			return ELANG_TYPE_INFO_MIN_SCORE;//This is not a base of type
		
		if (is_vref() || type.is_vref())
			return ELANG_TYPE_INFO_MIN_SCORE;

		if (is_ref()){
			if (!is_const() && !type.is_ref() || (type.is_ref() && type.is_const()))
				return ELANG_TYPE_INFO_MIN_SCORE;//'const ref' to 'ref' OR 'rval' to 'ref'
			return (type.is_ref() ? (ELANG_TYPE_INFO_MAX_SCORE - 1) : (ELANG_TYPE_INFO_MAX_SCORE - 2));
		}

		return (ELANG_TYPE_INFO_MAX_SCORE - 1);
	}

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
	auto symbol_table = type.symbol_table();
	return (symbol_table != nullptr && symbol_table_->is_base(*symbol_table));
}

bool elang::lang::user_type_info::is_user() const{
	return true;
}
