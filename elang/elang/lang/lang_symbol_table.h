#pragma once

#ifndef ELANG_LANG_SYMBOL_TABLE_H
#define ELANG_LANG_SYMBOL_TABLE_H

#include <list>
#include <vector>
#include <variant>
#include <unordered_map>

#include "lang_function_type_info.h"

namespace elang::lang{
	class symbol_table{
	public:
		enum class entry_attribute_type : unsigned int{
			nil					= (0 << 0x0000),
			static_				= (1 << 0x0000),
			tls					= (1 << 0x0001),
			private_			= (1 << 0x0002),
			protected_			= (1 << 0x0003),
			undefined_			= (1 << 0x0004),
			static_const		= (1 << 0x0005),
		};

		struct variable_entry_info{
			symbol_table *parent;
			entry_attribute_type attributes;
			type_info::ptr_type type;
			std::string name;
		};

		struct function_entry_info : variable_entry_info{
			std::vector<variable_entry_info> parameters;
		};

		struct function_list_entry_info{
			std::unordered_map<std::string, function_entry_info> list;
		};

		typedef std::variant<variable_entry_info, function_list_entry_info> non_table_type;

		struct non_table_entry_info{
			bool holds_value;
			non_table_type value;
		};

		struct entry_info{
			std::shared_ptr<symbol_table> table;
			non_table_entry_info non_table;
		};

		typedef std::unordered_map<std::string, entry_info> map_type;
		typedef std::list<variable_entry_info *> variable_list_type;

		symbol_table(const std::string &name, symbol_table *parent, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual ~symbol_table();

		virtual symbol_table *parent() const;

		virtual const std::string &name() const;

		virtual std::string mangle() const;

		virtual entry_attribute_type attributes() const;

		virtual void add(std::shared_ptr<symbol_table> table);

		virtual void add(const variable_entry_info &variable);

		virtual void add(const function_entry_info &function);

		virtual void add_namespace(const std::string &name, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual void add_variable(const std::string &name, type_info::ptr_type type, entry_attribute_type attributes = entry_attribute_type::nil);

		virtual void add_function(const std::string &name, type_info::ptr_type return_type, std::vector<variable_entry_info> &&parameters,
			entry_attribute_type attributes = entry_attribute_type::nil);

		virtual entry_info *find(const std::string &name) const;

		virtual symbol_table *find_table(const std::string &name) const;

		virtual non_table_type *find_non_table(const std::string &name) const;

		virtual type_info *find_type(const std::string &name) const;

		virtual variable_entry_info *find_variable(const std::string &name) const;

		virtual function_list_entry_info *find_function(const std::string &name) const;

		virtual unsigned __int64 compute_offset(const symbol_table &table) const;

		virtual unsigned __int64 compute_offset(const variable_entry_info &var) const;

		virtual unsigned __int64 compute_relative_offset(const symbol_table &table) const;

		virtual unsigned __int64 compute_relative_offset(const variable_entry_info &var) const;

		static function_entry_info *match_function(const std::vector<function_entry_info> &list,
			const std::vector<type_info::ptr_type> &parameter_types);

	protected:
		virtual void add_function_(function_list_entry_info &list, const function_entry_info &info);

		std::string name_;
		symbol_table *parent_;
		entry_attribute_type attributes_;
		map_type map_;
		variable_list_type order_list_;
	};

	ELANG_MAKE_OPERATORS(symbol_table::entry_attribute_type)
}

#endif /* !ELANG_LANG_SYMBOL_TABLE_H */
