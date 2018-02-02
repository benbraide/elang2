#include "lang_pointer_type_info.h"

elang::lang::pointer_type_info::pointer_type_info(ptr_type target, attribute_type attributes)
	: type_info(attributes), target_(target){}

elang::lang::type_info::ptr_type elang::lang::pointer_type_info::clone(attribute_type attributes) const{
	return std::make_shared<pointer_type_info>(target_, attributes);
}

std::size_t elang::lang::pointer_type_info::size() const{
	return sizeof(void *);
}

std::string elang::lang::pointer_type_info::mangle_name() const{
	return ("P" + target_->mangle());
}

int elang::lang::pointer_type_info::score(const type_info &type) const{
	auto pointer_type = dynamic_cast<const pointer_type_info *>(&type);
	if (pointer_type == nullptr)//Type is not a pointer
		return (type.has_conversion_to(*this) ? (ELANG_TYPE_INFO_MAX_SCORE - 2) : ELANG_TYPE_INFO_MIN_SCORE);

	if (!target_->is_const() && pointer_type->target_->is_const())
		return ELANG_TYPE_INFO_MIN_SCORE;//'constness' of targets must match

	if (target_->score(*pointer_type->target_) != ELANG_TYPE_INFO_MAX_SCORE){//Check for compatibility
		if (target_->is_void() && !pointer_type->target_->is_pointer() && !pointer_type->target_->is_array())
			return (ELANG_TYPE_INFO_MAX_SCORE - 2);
		return ELANG_TYPE_INFO_MIN_SCORE;
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

bool elang::lang::pointer_type_info::has_conversion_to(const type_info &type) const{
	return false;
}

bool elang::lang::pointer_type_info::is_pointer() const{
	return true;
}

elang::lang::type_info::ptr_type elang::lang::pointer_type_info::target() const{
	return target_;
}