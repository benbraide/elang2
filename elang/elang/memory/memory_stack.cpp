#include "memory_stack.h"

elang::memory::stack::stack(size_type size)
	: size_(size), data_(new char[size]){
	ptr_ = data_.get();
}

char *elang::memory::stack::push(size_type size){
	return push(size, nullptr);
}

char *elang::memory::stack::push(size_type size, const char *buffer){
	if ((ptr_ + size) > (data_.get() + size_))
		throw common::error::stack_overflow;

	if (buffer != nullptr)//Copy data
		memcpy(ptr_, buffer, size);

	ptr_ += size;//Advance pointer
	return ptr_;
}

char *elang::memory::stack::push(const memory_register &reg){
	return push(reg.size(), reg.data());
}

char *elang::memory::stack::pop(size_type size){
	return pop(size, nullptr);
}

char *elang::memory::stack::pop(size_type size, char *buffer){
	if (ptr_ < (data_.get() + size))
		throw common::error::stack_underflow;

	ptr_ -= size;//Advance pointer
	if (buffer != nullptr)//Copy data
		memcpy(buffer, ptr_, size);

	return ptr_;
}

char *elang::memory::stack::pop(memory_register &reg){
	return pop(reg.size(), reg.data());
}

char *elang::memory::stack::ptr() const{
	return ptr_;
}
