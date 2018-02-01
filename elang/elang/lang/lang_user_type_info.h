#pragma once

#ifndef ELANG_LANG_USER_TYPE_INFO_H
#define ELANG_LANG_USER_TYPE_INFO_H

#include "lang_symbol_table.h"

namespace elang::lang{
	class user_type_info : public type_info, public symbol_table{
	public:
		user_type_info(const std::string &name, symbol_table *parent, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual std::size_t size() const override;

		virtual std::string mangle_name() const override;

		virtual int score(const type_info &type) const override;

		virtual bool has_conversion_to(const type_info &type) const override;

		virtual bool is_user() const override;

		virtual void add(std::shared_ptr<symbol_table> table) override;

		virtual void add(const variable_entry_info &variable) override;

		virtual unsigned __int64 compute_offset(const symbol_table &table) const override;

		virtual unsigned __int64 compute_offset(const variable_entry_info &var) const override;

	protected:
		std::size_t size_ = 0;
	};

	class modified_user_type_info : public type_info{
	public:
		modified_user_type_info(ptr_type value, attribute_type attributes);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual std::size_t size() const override;

		virtual std::string mangle() const override;

		virtual std::string mangle_name() const override;

		virtual std::string mangle_attributes() const override;

		virtual int score(const type_info &type) const override;

		virtual bool has_conversion_to(const type_info &type) const override;

		virtual bool is_auto() const override;

		virtual bool is_primitive() const override;

		virtual bool is_void() const override;

		virtual bool is_bool() const override;

		virtual bool is_numeric() const override;

		virtual bool is_integral() const override;

		virtual bool is_unsigned_integral() const override;

		virtual bool is_float() const override;

		virtual bool is_function() const override;

		virtual bool is_array() const override;

		virtual bool is_pointer() const override;

		virtual bool is_null_pointer() const override;

		virtual bool is_user() const override;

		virtual ptr_type value() const;

	protected:
		ptr_type value_;
	};

	class union_user_type_info : public user_type_info{
	public:
		union_user_type_info(const std::string &name, symbol_table *parent, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual void add(const variable_entry_info &variable) override;

		virtual unsigned __int64 compute_offset(const variable_entry_info &var) const override;
	};

	class enum_user_type_info : public user_type_info{
	public:
		enum_user_type_info(const std::string &name, symbol_table *parent, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual ptr_type clone(attribute_type attributes) const override;

		virtual void add(const variable_entry_info &variable) override;

		virtual unsigned __int64 compute_offset(const variable_entry_info &var) const override;
	};
}

#endif /* !ELANG_LANG_USER_TYPE_INFO_H */
