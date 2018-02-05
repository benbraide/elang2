#pragma once

#ifndef ELANG_LANG_EXTENDED_TYPE_SYMBOL_TABLE_H
#define ELANG_LANG_EXTENDED_TYPE_SYMBOL_TABLE_H

#include "lang_type_symbol_table.h"

namespace elang::lang{
	class extended_type_symbol_table : public type_symbol_table{
	public:
		typedef std::unordered_map<std::string, type_symbol_table *> type_map_type;
		typedef std::list<type_symbol_table *> type_list_type;

		extended_type_symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual symbol_table *find_table(const std::string &name) const override;

		virtual non_table_type *find_non_table(const std::string &name) const override;

		virtual unsigned __int64 compute_relative_offset(const symbol_table &table) const override;

		virtual unsigned __int64 compute_relative_offset(const variable_entry_info &var) const override;

		virtual unsigned __int64 align_address(unsigned __int64 value, const type_symbol_table &table) const override;

		virtual bool is_base(const type_symbol_table &value) const override;

		virtual void add_base(type_symbol_table &value);

	protected:
		type_map_type type_map_;
		type_list_type type_order_list_;
	};
}

#endif /* !ELANG_LANG_EXTENDED_TYPE_SYMBOL_TABLE_H */
