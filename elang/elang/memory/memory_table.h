#pragma once

#ifndef ELANG_MEMORY_TABLE_H
#define ELANG_MEMORY_TABLE_H

#include <map>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include "../common/error.h"

namespace elang::memory{
	class table{
	public:
		struct range_type{
			unsigned __int64 from;
			unsigned __int64 to;
		};

		struct block_type{
			unsigned __int64 address;
			std::size_t size;
			std::size_t actual_size;
			std::unique_ptr<char[]> data;
		};

		typedef std::unordered_map<unsigned __int64, block_type> map_type;
		typedef std::map<unsigned __int64, std::size_t> size_map_type;
		typedef map_type::iterator map_iterator_type;

		typedef std::shared_mutex lock_type;
		typedef std::lock_guard<lock_type> lock_guard_type;
		typedef std::shared_lock<lock_type> shared_lock_guard_type;

		void protect_from_access(const range_type &range);

		bool is_access_protected(const range_type &range) const;

		void protect_from_write(const range_type &range);

		bool is_write_protected(const range_type &range) const;

		void deallocate(unsigned __int64 address);

		unsigned __int64 reserve(std::size_t size);

		block_type *allocate(std::size_t size, unsigned __int64 address_hint = 0ull);

		block_type *reallocate(unsigned __int64 address, std::size_t size);

		void set(unsigned __int64 address, char source, std::size_t count);

		void copy(unsigned __int64 destination, unsigned __int64 source, std::size_t size);

		template <typename value_type>
		void write(unsigned __int64 destination, value_type value){
			return write(destination, reinterpret_cast<char *>(&value), sizeof(value_type));
		}

		template <typename value_type>
		void write(unsigned __int64 destination, const value_type *value, std::size_t count){
			return write(destination, reinterpret_cast<const char *>(value), (sizeof(value_type) * count));
		}

		void write(unsigned __int64 destination, const char *source, std::size_t size);

		template <typename input_type>
		void read_from_input(unsigned __int64 destination, input_type &source, std::size_t count){
			shared_lock_guard_type lock_guard(lock_);
			read_from_input_(destination, source, count);
		}

		template <typename target_type>
		target_type *read_bytes(unsigned __int64 source){
			auto block = find_nearest(source);
			return ((block == nullptr) ? nullptr : (target_type *)(block->data.get() + (source - block->address)));
		}

		template <typename target_type>
		target_type read(unsigned __int64 source){
			auto value = target_type();
			read(source, (char *)&value, sizeof(target_type));
			return value;
		}

		void read(unsigned __int64 source, char *destination, std::size_t size);

		template <typename output_type>
		void write_to_output(unsigned __int64 source, output_type &destination, std::size_t count){
			shared_lock_guard_type lock_guard(lock_);
			write_to_output_(source, destination, count);
		}

		block_type *find(unsigned __int64 address);

		block_type *find_nearest(unsigned __int64 address);

	private:
		void add_available_(unsigned __int64 value, std::size_t size);

		unsigned __int64 find_available_(std::size_t size, unsigned __int64 match = 0ull);

		void deallocate_(unsigned __int64 address, bool add_to_available);

		unsigned __int64 reserve_(std::size_t size);

		block_type *allocate_block_(std::size_t size, unsigned __int64 address);

		block_type *allocate_(std::size_t size, unsigned __int64 address_hint);

		block_type *reallocate_(unsigned __int64 address, std::size_t size);

		void set_(unsigned __int64 address, char source, std::size_t count);

		void copy_(unsigned __int64 destination, unsigned __int64 source, std::size_t size);

		void write_(unsigned __int64 destination, const char *source, std::size_t size, bool is_array);

		template <typename input_type>
		void read_from_input_(unsigned __int64 destination, input_type &source, std::size_t count){
			if (count == 0u)//Do nothing
				return;

			range_type range{ destination, (destination + (count - 1)) };
			if (is_access_protected(range))
				throw common::error::memory_read_access_violation;

			if (is_write_protected(range))
				throw common::error::memory_write_access_violation;

			block_type *block = nullptr;
			std::size_t available_size = 0u, min_size = 0u, ptr_index = 0u;

			while (count > 0u){
				if ((block = ((block == nullptr) ? find_nearest_(destination) : find_(destination))) != nullptr){//Valid block
					ptr_index = static_cast<std::size_t>(destination - block->address);
					available_size = (block->actual_size - ptr_index);
				}
				else//No next block
					throw common::error::memory_read_access_violation;

				min_size = (available_size < count) ? available_size : count;
				source.read(block->data.get() + ptr_index, min_size);

				destination += min_size;
				count -= min_size;
			}
		}

		void read_(unsigned __int64 source, char *destination, std::size_t size);

		template <typename output_type>
		void write_to_output_(unsigned __int64 source, output_type &destination, std::size_t count){
			block_type *block = nullptr;
			std::size_t available_size = 0u, min_size = 0u, ptr_index = 0u;

			while (count > 0u){
				if ((block = (block == nullptr) ? find_nearest_(source) : find_(source)) != nullptr){//Valid block
					ptr_index = static_cast<std::size_t>(source - block->address);
					available_size = (block->actual_size - ptr_index);
				}
				else//No next block
					throw common::error::memory_read_access_violation;

				min_size = (available_size < count) ? available_size : count;
				destination.write(block->data.get() + ptr_index, min_size);//Read block

				source += min_size;
				count -= min_size;
			}
		}

		map_iterator_type find_raw_(unsigned __int64 address);

		block_type *find_(unsigned __int64 address);

		block_type *find_nearest_(unsigned __int64 address);

		range_type access_protected_{ static_cast<unsigned __int64>(-1), static_cast<unsigned __int64>(-1) };
		range_type write_protected_{ static_cast<unsigned __int64>(-1), static_cast<unsigned __int64>(-1) };
		unsigned __int64 next_address_ = 0u;
		map_type map_;
		size_map_type available_;
		lock_type lock_;
	};
}

#endif /* !ELANG_MEMORY_TABLE_H */
