#include "lang_array_type_info.h"

elang::lang::array_type_info::array_type_info(ptr_type target, std::size_t count, attribute_type attributes)
	: type_info(attributes), target_(target), count_(count){}

elang::lang::type_info::ptr_type elang::lang::array_type_info::clone(attribute_type attributes) const{
	return std::make_shared<array_type_info>(target_, count_, attributes);
}

std::size_t elang::lang::array_type_info::size() const{
	return (target_->size() * count_);
}

std::string elang::lang::array_type_info::mangle_name() const{
	return ("A" + std::to_string(count_) + target_->mangle());
}

int elang::lang::array_type_info::score(const type_info &type) const{
	auto array_type = dynamic_cast<const array_type_info *>(&type);
	if (array_type == nullptr)//Type is not a array
		return (type.has_conversion_to(*this) ? (ELANG_TYPE_INFO_MAX_SCORE - 2) : ELANG_TYPE_INFO_MIN_SCORE);

	if (!target_->is_const() && array_type->target_->is_const())
		return ELANG_TYPE_INFO_MIN_SCORE;//'constness' of targets must match

	return ((target_->score(*array_type->target_) == ELANG_TYPE_INFO_MAX_SCORE) ? ELANG_TYPE_INFO_MAX_SCORE : ELANG_TYPE_INFO_MIN_SCORE);
}

bool elang::lang::array_type_info::has_conversion_to(const type_info &type) const{
	if (is_vref() || (type.is_ref() && !type.is_const()))//Same type required
		return false;

	auto pointer_type = dynamic_cast<const pointer_type_info *>(&type);
	if (pointer_type == nullptr)
		return false;

	auto pointer_target = pointer_type->target();
	if (!target_->is_const() && pointer_target->is_const())
		return false;

	return (target_->is_void() && !pointer_target->is_pointer() && !pointer_target->is_array());
}

bool elang::lang::array_type_info::is_array() const{
	return true;
}

elang::lang::type_info::ptr_type elang::lang::array_type_info::target() const{
	return target_;
}
