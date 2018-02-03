#pragma once

#ifndef ELANG_LANG_REGISTER_STACK_H
#define ELANG_LANG_REGISTER_STACK_H

#include <vector>

#include "../common/error.h"
#include "../memory/memory_register_table.h"

namespace elang::lang{
	class register_stack{
	public:
		typedef std::vector<memory::memory_register *> list_type;

		struct info{
			list_type list;
			std::size_t index;
		};

		register_stack();

		memory::memory_register *pop_integral(std::size_t size);

		memory::memory_register *pop_float(std::size_t size);

		void push_integral();

		void push_float();

	private:
		memory::register_table table_;
		info integral_info_;
		info float_info_;
	};
}

#endif /* !ELANG_LANG_REGISTER_STACK_H */
