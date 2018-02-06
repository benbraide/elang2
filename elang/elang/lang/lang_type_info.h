#pragma once

#ifndef ELANG_LANG_TYPE_INFO_H
#define ELANG_LANG_TYPE_INFO_H

#include <string>
#include <memory>

#include "../common/macro.h"
#include "../common/error.h"

#define ELANG_TYPE_INFO_MIN_SCORE 0
#define ELANG_TYPE_INFO_MAX_SCORE 20

namespace elang::lang{
	class type_symbol_table;

	class type_info : public std::enable_shared_from_this<type_info>{
	public:
		typedef std::shared_ptr<type_info> ptr_type;
		typedef std::shared_ptr<type_symbol_table> type_symbol_table_ptr_type;

		enum class attribute_type : unsigned int{
			nil				= (0 << 0x0000),
			const_			= (1 << 0x0000),
			ref_			= (1 << 0x0001),
			vref			= (1 << 0x0002),
			optional		= (1 << 0x0003),
		};

		explicit type_info(attribute_type attributes);

		virtual ptr_type reflect();

		virtual ptr_type clone(attribute_type attributes) const = 0;

		virtual ptr_type resolve_auto(const type_info &type, bool is_pointer = false) const;

		virtual attribute_type attributes() const;

		virtual type_symbol_table_ptr_type symbol_table() const;

		virtual std::size_t size() const = 0;

		virtual std::string mangle() const;

		virtual std::string mangle_name() const = 0;

		virtual std::string mangle_attributes() const;

		virtual unsigned __int64 align_address(unsigned __int64 value, const type_info &type) const;

		virtual int score(const type_info &type) const = 0;

		virtual bool has_conversion_to(const type_info &type) const;

		virtual bool is_const() const;

		virtual bool is_ref() const;

		virtual bool is_vref() const;

		virtual bool is_auto() const;

		virtual bool is_primitive() const;

		virtual bool is_void() const;

		virtual bool is_bool() const;

		virtual bool is_numeric() const;

		virtual bool is_integral() const;

		virtual bool is_unsigned_integral() const;

		virtual bool is_float() const;

		virtual bool is_function() const;

		virtual bool is_array() const;

		virtual bool is_pointer() const;

		virtual bool is_null_pointer() const;

		virtual bool is_user() const;

	protected:
		attribute_type attributes_;
	};

	ELANG_MAKE_OPERATORS(type_info::attribute_type)
}

#endif /* !ELANG_LANG_TYPE_INFO_H */
