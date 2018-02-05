#pragma once

#ifndef ELANG_LANG_TYPE_SYMBOL_TABLE_H
#define ELANG_LANG_TYPE_SYMBOL_TABLE_H

#include "lang_symbol_table.h"

namespace elang::lang{
	class type_symbol_table : public symbol_table{
	public:
		using symbol_table::compute_relative_offset;

		type_symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual std::string mangle() const override;

		virtual void add(const variable_entry_info &variable) override;

		virtual unsigned __int64 compute_relative_offset(const variable_entry_info &var) const override;

		virtual unsigned __int64 align_address(unsigned __int64 value, const type_symbol_table &table) const;

		virtual std::size_t size() const;

		virtual bool is_base(const type_symbol_table &value) const;

	protected:
		std::size_t size_;
	};

	class union_type_symbol_table : public type_symbol_table{
	public:
		using type_symbol_table::compute_offset;

		union_type_symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual void add(const variable_entry_info &variable) override;

		virtual unsigned __int64 compute_offset(const variable_entry_info &var) const override;
	};

	class enum_type_symbol_table : public type_symbol_table{
	public:
		using type_symbol_table::compute_offset;

		enum_type_symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual void add(const variable_entry_info &variable) override;

		virtual unsigned __int64 compute_offset(const variable_entry_info &var) const override;
	};
}

#endif /* !ELANG_LANG_TYPE_SYMBOL_TABLE_H */
