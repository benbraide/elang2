#pragma once

#ifndef ELANG_MEMORY_REGISTER_TABLE_H
#define ELANG_MEMORY_REGISTER_TABLE_H

#include <unordered_map>

#include "../common/macro.h"

#include "memory_register.h"

namespace elang::memory{
	enum class register_flag : unsigned int{
		nil				= (0 << 0x0000),
		zero			= (1 << 0x0000),
	};

	class register_table{
	public:
		typedef std::shared_ptr<memory_register> register_ptr_type;
		typedef std::unordered_map<std::string, register_ptr_type> map_type;
		typedef std::unordered_map<std::size_t, memory_register *> index_map_type;

		register_table();

		memory_register *find(const std::string &key) const;

		memory_register *find(std::size_t index) const;

		void set_flag(register_flag flag);

		void clear_flag(register_flag flag);

		bool has_flag(register_flag flag) const;

		std::size_t index(const memory_register &reg) const;

	private:
		void add_(const std::string &name, const std::string &alias, const std::string &_32, const std::string &_16,
			const std::string &low, const std::string &high, unsigned __int64 &offset, unsigned __int64 &index);

		void add_qword_(const std::string &prefix, int from, int to, unsigned __int64 &offset, unsigned __int64 &index);

		void add_float_(const std::string &prefix, int from, int to, unsigned __int64 &offset, unsigned __int64 &index);

		map_type map_;
		index_map_type index_map_;
		register_flag flags_;
		std::unique_ptr<__int64[]> data_;
	};

	ELANG_MAKE_OPERATORS(register_flag);
}

#endif /* !ELANG_MEMORY_REGISTER_TABLE_H */
