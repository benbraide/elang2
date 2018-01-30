#include "lang_function_type_info.h"

elang::lang::function_type_info::function_type_info(ptr_type return_type)
	: type_info(attribute_type::nil), return_type_(return_type){}

elang::lang::function_type_info::function_type_info(ptr_type return_type, ptr_list_type &&parameter_types)
	: type_info(attribute_type::nil), return_type_(return_type), parameter_types_(std::move(parameter_types)){}

elang::lang::type_info::ptr_type elang::lang::function_type_info::clone(attribute_type attributes) const{
	auto parameter_types = parameter_types_;
	return std::make_shared<function_type_info>(return_type_, std::move(parameter_types));
}

std::size_t elang::lang::function_type_info::size() const{
	throw common::error::lang_bad_type;
}

std::string elang::lang::function_type_info::mangle_name() const{
	if (parameter_types_.empty())
		return ("F0" + return_type_->mangle() + "v");

	std::string parameters;
	for (auto parameter_type : parameter_types_)
		parameters += parameter_type->mangle();

	return ("F" + std::to_string(parameter_types_.size()) + return_type_->mangle() + parameters);
}

int elang::lang::function_type_info::score(const type_info &type) const{
	auto function_type = dynamic_cast<const function_type_info *>(&type);
	if (function_type == nullptr)//Type is not a pointer
		return (type.has_conversion_to(*this) ? (ELANG_TYPE_INFO_MAX_SCORE - 2) : ELANG_TYPE_INFO_MIN_SCORE);

	if (parameter_types_.size() != function_type->parameter_types_.size())
		return ELANG_TYPE_INFO_MIN_SCORE;//Parameter types count must match

	if (!function_type->return_type_->is_auto() && return_type_->score(*function_type->return_type_) != ELANG_TYPE_INFO_MAX_SCORE)
		return ELANG_TYPE_INFO_MIN_SCORE;//Return types must match

	auto lparam = parameter_types_.begin(), rparam = function_type->parameter_types_.begin();
	for (; lparam != parameter_types_.end(); ++lparam, ++rparam){
		if ((!(*rparam)->is_auto() || (*lparam)->is_variadic()) && (*lparam)->score(**rparam) != ELANG_TYPE_INFO_MAX_SCORE)
			return ELANG_TYPE_INFO_MIN_SCORE;//Parameter types must match
	}

	return ELANG_TYPE_INFO_MAX_SCORE;
}

bool elang::lang::function_type_info::is_function() const{
	return true;
}

elang::lang::type_info::ptr_type elang::lang::function_type_info::return_type() const{
	return return_type_;
}

const elang::lang::function_type_info::ptr_list_type &elang::lang::function_type_info::parameter_types() const{
	return parameter_types_;
}
