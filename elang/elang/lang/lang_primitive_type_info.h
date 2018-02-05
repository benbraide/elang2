#pragma once

#ifndef ELANG_LANG_PRIMITIVE_TYPE_INFO_H
#define ELANG_LANG_PRIMITIVE_TYPE_INFO_H

#include "lang_type_info.h"

namespace elang::lang{
	class primitive_type_info : public type_info{
	public:
		enum class id_type{
			auto_,
			void_,
			bool_,
			char_,
			wchar_,
			int8_,
			int16_,
			int32_,
			int64_,
			float_,
			nullptr_,
		};

		enum class bool_type : char{
			false_,
			true_,
			indeterminate,
		};

		primitive_type_info(id_type id, attribute_type attributes = attribute_type::nil);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual std::size_t size() const override;

		virtual std::string mangle_name() const override;

		virtual int score(const type_info &type) const override;

		virtual bool has_conversion_to(const type_info &type) const override;

		virtual bool is_primitive() const override;

		virtual bool is_void() const override;

		virtual bool is_bool() const override;

		virtual bool is_numeric() const override;

		virtual bool is_integral() const override;

		virtual bool is_float() const override;

		virtual bool is_null_pointer() const override;

		virtual id_type id() const;

	protected:
		virtual int conversion_offset_(const primitive_type_info &type) const;

		id_type id_;
	};

	class unsigned_integral_type_info : public primitive_type_info{
	public:
		unsigned_integral_type_info(id_type id, attribute_type attributes = attribute_type::nil);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual std::string mangle_name() const override;

		virtual bool is_unsigned_integral() const override;
	};
}

#endif /* !ELANG_LANG_PRIMITIVE_TYPE_INFO_H */
