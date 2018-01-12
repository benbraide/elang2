#include "memory_register.h"

elang::memory::memory_register::memory_register(memory_register *parent, const std::string &name, size_type size, char *data)
	: parent_(parent), low_(nullptr), high_(nullptr), name_(name), size_(size), data_(data){
	if (parent_ != nullptr){//Update parent's children, if applicable
		if (parent_->low_ == nullptr)
			parent_->low_ = this;
		else if (parent_->high_ == nullptr)
			parent_->high_ = this;
	}
}

elang::memory::memory_register::~memory_register() = default;

elang::memory::memory_register *elang::memory::memory_register::match(size_type size) const{
	if (size < size_)//Match with low child
		return ((low_ == nullptr) ? nullptr : low_->match(size));

	if (size > size_)//Match with parent
		return ((parent_ == nullptr || size < parent_->size_) ? nullptr : parent_->match(size));

	return const_cast<memory_register *>(this);//Same size
}

elang::memory::memory_register *elang::memory::memory_register::parent() const{
	return parent_;
}

elang::memory::memory_register *elang::memory::memory_register::low_child() const{
	return low_;
}

elang::memory::memory_register *elang::memory::memory_register::high_child() const{
	return high_;
}

const std::string &elang::memory::memory_register::name() const{
	return name_;
}

elang::memory::memory_register::size_type elang::memory::memory_register::size() const{
	return size_;
}

char *elang::memory::memory_register::data() const{
	return data_;
}

bool elang::memory::memory_register::is_floating_point() const{
	return false;
}
