#pragma once

#ifndef ELANG_LANG_FUNCTION_TYPE_INFO_H
#define ELANG_LANG_FUNCTION_TYPE_INFO_H

#include <vector>

#include "lang_type_info.h"

namespace elang::lang{
	class function_type_info : public type_info{
	public:
		typedef std::vector<ptr_type> ptr_list_type;

		explicit function_type_info(ptr_type return_type);

		function_type_info(ptr_type return_type, ptr_list_type &&parameter_types);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual std::size_t size() const override;

		virtual std::string mangle_name() const override;

		virtual int score(const type_info &type) const override;

		virtual bool is_function() const override;

		virtual std::string mangle_parameters() const;

		virtual int call_score(const std::vector<ptr_type> &parameter_types) const;

		virtual ptr_type return_type() const;

		virtual const ptr_list_type &parameter_types() const;

	protected:
		ptr_type return_type_;
		ptr_list_type parameter_types_;
	};
}

#endif /* !ELANG_LANG_FUNCTION_TYPE_INFO_H */
