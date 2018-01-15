#include "../byte_code/byte_code_translator.h"

void elang::memory::table::protect_from_access(const range_type &range){
	access_protected_ = range;
	if (next_address_ <= range.to)
		next_address_ = (range.to + 1);
}

bool elang::memory::table::is_access_protected(const range_type &range) const{
	if (/*elang::vm::runtime::is_inside_protected_mode()*/false)
		return false;//Inside protected mode

	return ((access_protected_.from <= range.from && range.from <= access_protected_.to) ||
		(access_protected_.from <= range.to && range.to <= access_protected_.to));
}

void elang::memory::table::protect_from_write(const range_type &range){
	write_protected_ = range;
}

bool elang::memory::table::is_write_protected(const range_type &range) const{
	return ((write_protected_.from <= range.from && range.from <= write_protected_.to) ||
		(write_protected_.from <= range.to && range.to <= write_protected_.to));
}

void elang::memory::table::deallocate(unsigned __int64 address){
	lock_guard_type lock_guard(lock_);
	return deallocate_(address, true);
}

unsigned __int64 elang::memory::table::reserve(std::size_t size){
	lock_guard_type lock_guard(lock_);
	return reserve_(size);
}

elang::memory::table::block_type *elang::memory::table::allocate(std::size_t size, unsigned __int64 address_hint){
	lock_guard_type lock_guard(lock_);
	return allocate_(size, address_hint);
}

elang::memory::table::block_type *elang::memory::table::reallocate(unsigned __int64 address, std::size_t size){
	lock_guard_type lock_guard(lock_);
	return reallocate_(address, size);
}

void elang::memory::table::set(unsigned __int64 address, char source, std::size_t count){
	shared_lock_guard_type lock_guard(lock_);
	set_(address, source, count);
}

void elang::memory::table::copy(unsigned __int64 destination, unsigned __int64 source, std::size_t size){
	shared_lock_guard_type lock_guard(lock_);
	copy_(destination, source, size);
}

void elang::memory::table::write(unsigned __int64 destination, const char *source, std::size_t size){
	shared_lock_guard_type lock_guard(lock_);
	write_(destination, source, size, true);
}

void elang::memory::table::read(unsigned __int64 source, char *destination, std::size_t size){
	shared_lock_guard_type lock_guard(lock_);
	read_(source, destination, size);
}

elang::memory::table::block_type *elang::memory::table::find(unsigned __int64 address){
	shared_lock_guard_type lock_guard(lock_);
	return find_(address);
}

elang::memory::table::block_type *elang::memory::table::find_nearest(unsigned __int64 address){
	shared_lock_guard_type lock_guard(lock_);
	return find_nearest_(address);
}

void elang::memory::table::add_available_(unsigned __int64 value, std::size_t size){
	if (size == 0u)
		return;

	auto entry = available_.begin();
	for (; entry != available_.end(); ++entry){
		if ((entry->first + entry->second) == value)
			break;//Previous in sequence
	}

	if (entry != available_.end()){//Merge with previous
		entry->second += size;
		value = entry->first;
		size = entry->second;
	}

	auto next = available_.find(value + size);
	if (next != available_.end()){//Merge with next
		entry->second += next->second;
		available_.erase(next);
	}
	else//Add entry
		available_[value] = size;

	if (!available_.empty()){
		auto last = std::prev(available_.end());
		if ((last->first + last->second) == next_address_){//Move next address backwards
			next_address_ = last->first;
			available_.erase(last);
		}
	}
}

unsigned __int64 elang::memory::table::find_available_(std::size_t size, unsigned __int64 match){
	for (auto &entry : available_){
		if (match != 0ull && entry.first != match){
			if (match < entry.first)
				break;
			continue;
		}

		if (size < entry.second){//Use required
			available_[entry.first + size] = (entry.second - size);
			return entry.first;
		}

		if (size == entry.second)
			return entry.first;
	}

	return 0ull;
}

void elang::memory::table::deallocate_(unsigned __int64 address, bool add_to_available){
	auto entry = find_raw_(address);
	if (entry == map_.end())//Invalid address
		throw common::error::memory_read_access_violation;

	if (add_to_available)//Add to available list
		add_available_(address, entry->second.actual_size);

	map_.erase(entry);
}

unsigned __int64 elang::memory::table::reserve_(std::size_t size){
	if (size == 0u)
		return 0ull;

	auto address = find_available_(size);
	if (address == 0ull){//Use next value
		if ((std::numeric_limits<unsigned __int64>::max() - size) < address)
			throw common::error::memory_out_of_space;//Out of address space

		address = next_address_;
		next_address_ += size;
	}
	else//Remove from list
		available_.erase(address);

	return address;
}

elang::memory::table::block_type *elang::memory::table::allocate_block_(std::size_t size, unsigned __int64 address){
	try{
		block_type block{
			address,						//Address
			size,							//Size
			size,							//Actual size
			std::make_unique<char[]>(size)	//Data
		};

		if (block.data.get() != nullptr)//Add entry
			return &(map_[address] = std::move(block));
	}
	catch (...){}

	throw common::error::memory_out_of_space;
}

elang::memory::table::block_type *elang::memory::table::allocate_(std::size_t size, unsigned __int64 address_hint){
	if (size == 0u)
		throw common::error::memory_invalid_size;

	if ((map_.max_size() - map_.size()) < size)
		throw common::error::memory_out_of_space;//Out of address space

	auto merge = false;
	auto actual_size = ((size == 0u) ? 1u : size);

	if (address_hint == 0ull){//Determine address
		merge = true;
		if ((address_hint = find_available_(actual_size)) == 0ull){//Use next address
			if ((std::numeric_limits<unsigned __int64>::max() - actual_size) < address_hint)
				throw common::error::memory_out_of_space;//Out of address space

			address_hint = next_address_;
			next_address_ += actual_size;
		}
		else//Remove from list
			available_.erase(address_hint);
	}
	else if (find_nearest_(address_hint) == nullptr){
		if (next_address_ < (address_hint + actual_size))
			next_address_ = (address_hint + actual_size);
	}
	else
		throw common::error::memory_read_access_violation;

	try{
		return allocate_block_(size, address_hint);
	}
	catch (...){
		if (merge)//Return address to available list
			add_available_(address_hint, actual_size);
		throw;//Forward exception
	}

	return nullptr;
}

elang::memory::table::block_type *elang::memory::table::reallocate_(unsigned __int64 address, std::size_t size){
	auto block = find_nearest_(address);
	if (block == nullptr)
		throw common::error::memory_read_access_violation;

	if (block->size < size){//Expand
		if (block->actual_size < size){//Reallocate
			auto old_data = std::move(block->data);
			auto old_size = block->actual_size;

			deallocate_(block->address, true);
			block = allocate_(size, 0u);
			memcpy(block->data.get(), old_data.get(), old_size);//Copy data
		}
		else//Expand size
			block->size = size;
	}
	else if (size < block->size)//Shrink
		block->size = size;

	return block;
}

void elang::memory::table::set_(unsigned __int64 address, char source, std::size_t count){
	write_(address, &source, count, false);
}

void elang::memory::table::copy_(unsigned __int64 destination, unsigned __int64 source, std::size_t size){
	block_type *block = nullptr;
	std::size_t available_size = 0u, target_size = 0u, ptr_index = 0u;

	while (size > 0u){
		if ((block = (block == nullptr) ? find_nearest_(source) : find_(source)) != nullptr){//Next block
			ptr_index = static_cast<std::size_t>(source - block->address);
			available_size = (block->actual_size - ptr_index);
		}
		else//No next block
			throw common::error::memory_read_access_violation;

		target_size = (available_size < size) ? available_size : size;
		write_(destination, block->data.get() + ptr_index, target_size, true);

		source += target_size;
		destination += target_size;
		size -= target_size;
	}
}

void elang::memory::table::write_(unsigned __int64 destination, const char *source, std::size_t size, bool is_array){
	if (size == 0u)//Do nothing
		return;

	range_type range{ destination, (destination + (size - 1)) };
	if (is_access_protected(range))
		throw common::error::memory_read_access_violation;

	if (is_write_protected(range))
		throw common::error::memory_write_access_violation;

	block_type *block = nullptr;
	std::size_t available_size = 0u, min_size = 0u, ptr_index = 0u;

	while (size > 0u){
		if ((block = ((block == nullptr) ? find_nearest_(destination) : find_(destination))) != nullptr){//Valid block
			ptr_index = static_cast<std::size_t>(destination - block->address);
			available_size = (block->actual_size - ptr_index);
		}
		else//No next block
			throw common::error::memory_read_access_violation;

		min_size = (available_size < size) ? available_size : size;
		if (is_array){
			memcpy(block->data.get() + ptr_index, source, min_size);
			source += min_size;//Advance source pointer
		}
		else//Set applicable
			std::memset(block->data.get() + ptr_index, *source, min_size);

		destination += min_size;
		size -= min_size;
	}
}

void elang::memory::table::read_(unsigned __int64 source, char *destination, std::size_t size){
	block_type *block = nullptr;
	std::size_t available_size = 0u, min_size = 0u, ptr_index = 0u;

	while (size > 0u){
		if ((block = (block == nullptr) ? find_nearest_(source) : find_(source)) != nullptr){//Valid block
			ptr_index = static_cast<std::size_t>(source - block->address);
			available_size = (block->actual_size - ptr_index);
		}
		else//No next block
			throw common::error::memory_read_access_violation;

		min_size = (available_size < size) ? available_size : size;
		memcpy(destination, block->data.get() + ptr_index, min_size);//Read block

		destination += min_size;
		source += min_size;
		size -= min_size;
	}
}

elang::memory::table::map_iterator_type elang::memory::table::find_raw_(unsigned __int64 address){
	if (is_access_protected(range_type{ address, address }))
		throw common::error::memory_read_access_violation;
	return map_.find(address);
}

elang::memory::table::block_type *elang::memory::table::find_(unsigned __int64 address){
	auto entry = find_raw_(address);
	return ((entry == map_.end()) ? nullptr : &entry->second);
}

elang::memory::table::block_type *elang::memory::table::find_nearest_(unsigned __int64 address){
	if (is_access_protected(range_type{ address, address }))
		throw common::error::memory_read_access_violation;

	block_type *block = nullptr;
	for (auto &entry : map_){
		if (entry.first == address || (entry.first < address && address < (entry.first + entry.second.actual_size))){
			block = &entry.second;
			break;
		}
	}

	return block;
}
