#pragma once

#ifndef ELANG_MEMORY_REGISTER_TABLE_H
#define ELANG_MEMORY_REGISTER_TABLE_H

#include <unordered_map>

#include "memory_register.h"

namespace elang::memory{
	class register_table{
	public:
		typedef std::shared_ptr<memory_register> register_ptr_type;
		typedef std::unordered_map<std::string, register_ptr_type> map_type;

		register_table();

		memory_register *find(const std::string &key) const;

	private:
		void add_(const std::string &name, const std::string &alias, const std::string &_32,
			const std::string &_16, const std::string &low, const std::string &high);

		void add_qword_(const std::string &prefix, int from, int to);

		void add_float_(const std::string &prefix, int from, int to);

		map_type map_;
	};
}

#endif /* !ELANG_MEMORY_REGISTER_TABLE_H */
