#pragma once

#ifndef ELANG_LANG_USER_TYPE_INFO_H
#define ELANG_LANG_USER_TYPE_INFO_H

#include "lang_extended_type_symbol_table.h"

namespace elang::lang{
	class user_type_info : public type_info{
	public:
		explicit user_type_info(type_symbol_table_ptr_type symbol_table, attribute_type attributes = attribute_type::nil);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual type_symbol_table_ptr_type symbol_table() const override;

		virtual std::size_t size() const override;

		virtual std::string mangle_name() const override;

		virtual unsigned __int64 align_address(unsigned __int64 value, const type_info &type) const override;

		virtual int score(const type_info &type) const override;

		virtual bool has_conversion_to(const type_info &type) const override;

		virtual bool is_user() const override;

	protected:
		type_symbol_table_ptr_type symbol_table_;
	};
}

#endif /* !ELANG_LANG_USER_TYPE_INFO_H */
