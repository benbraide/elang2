#pragma once

#ifndef ELANG_MEMORY_STACK_H
#define ELANG_MEMORY_STACK_H

#include "../common/error.h"

#include "memory_register.h"

namespace elang::memory{
	class stack{
	public:
		typedef std::size_t size_type;

		explicit stack(size_type size);

		char *push(size_type size);

		char *push(size_type size, const char *buffer);

		char *push(const memory_register &reg);

		template <typename value_type>
		char *push_value(value_type value){
			return push(sizeof(value_type), reinterpret_cast<const char *>(&value));
		}

		char *pop(size_type size);

		char *pop(size_type size, char *buffer);

		char *pop(memory_register &reg);

		template <typename target_type>
		target_type pop_value(){
			auto value = target_type();
			pop(sizeof(target_type), reinterpret_cast<char *>(&value));
			return value;
		}

		char *ptr() const;

	private:
		size_type size_;
		std::unique_ptr<char[]> data_;
		char *ptr_;
	};
}

#endif /* !ELANG_MEMORY_STACK_H */
