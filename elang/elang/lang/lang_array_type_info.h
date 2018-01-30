#pragma once

#ifndef ELANG_LANG_ARRAY_TYPE_INFO_H
#define ELANG_LANG_ARRAY_TYPE_INFO_H

#include "lang_pointer_type_info.h"

namespace elang::lang{
	class array_type_info : public type_info{
	public:
		array_type_info(ptr_type target, std::size_t count, attribute_type attributes);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual std::size_t size() const override;

		virtual std::string mangle_name() const override;

		virtual int score(const type_info &type) const override;

		virtual bool has_conversion_to(const type_info &type) const override;

		virtual bool is_array() const override;

		virtual ptr_type target() const;

	protected:
		ptr_type target_;
		std::size_t count_;
	};
}

#endif /* !ELANG_LANG_ARRAY_TYPE_INFO_H */
