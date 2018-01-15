#include "memory_stack.h"

elang::memory::stack::stack(table &tbl, size_type size)
	: block_(tbl.allocate(size)){
	ptr_ = block_->data.get();
}

char *elang::memory::stack::push(size_type size){
	return push(size, nullptr);
}

char *elang::memory::stack::push(size_type size, const char *buffer){
	if (static_cast<size_type>(block_->size - (ptr_ - block_->data.get())) < size)
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
	if (static_cast<size_type>(ptr_ - block_->data.get()) < size)
		throw common::error::stack_underflow;

	ptr_ -= size;//Advance pointer
	if (buffer != nullptr)//Copy data
		memcpy(buffer, ptr_, size);

	return ptr_;
}

char *elang::memory::stack::pop(memory_register &reg){
	return pop(reg.size(), reg.data());
}

elang::memory::stack::size_type elang::memory::stack::size() const{
	return block_->size;
}

char *elang::memory::stack::base() const{
	return block_->data.get();
}

char *elang::memory::stack::ptr() const{
	return ptr_;
}
