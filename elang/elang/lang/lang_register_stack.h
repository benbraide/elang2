#pragma once

#ifndef ELANG_LANG_REGISTER_STACK_H
#define ELANG_LANG_REGISTER_STACK_H

#include <vector>
#include <memory>

#include "../common/error.h"
#include "../memory/memory_register_table.h"

namespace elang::lang{
	class register_stack{
	public:
		class item{
		public:
			item(register_stack &stack, memory::memory_register &value)
				: stack_(&stack), value_(&value){}

			~item(){
				stack_->push(*value_);
			}

			memory::memory_register &value() const{
				return *value_;
			}

		private:
			register_stack *stack_;
			memory::memory_register *value_;
		};

		typedef std::shared_ptr<item> item_ptr_type;
		typedef std::vector<memory::memory_register *> list_type;

		struct info{
			list_type list;
			list_type used;
		};

		register_stack();

		item_ptr_type pop_integral(std::size_t size);

		item_ptr_type pop_float(std::size_t size);

		void push(memory::memory_register &reg);

	private:
		item_ptr_type pop_(std::size_t size, info &info);

		void push_(memory::memory_register &reg, info &info);

		memory::register_table table_;
		info integral_info_;
		info float_info_;
	};
}

#endif /* !ELANG_LANG_REGISTER_STACK_H */
