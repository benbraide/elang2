#pragma once

#ifndef ELANG_MEMORY_REGISTER_H
#define ELANG_MEMORY_REGISTER_H

#include <string>
#include <memory>

namespace elang::memory{
	class memory_register{
	public:
		typedef std::size_t size_type;

		memory_register(memory_register *parent, const std::string &name, size_type size, char *data);

		virtual ~memory_register();

		memory_register(const memory_register &) = delete;
		
		memory_register &operator =(const memory_register &) = delete;

		template <typename writer_type>
		friend writer_type &operator <<(writer_type &writer, memory_register &reg){
			writer << reg.name_;
			return writer;
		}

		template <typename value_type>
		void write(value_type value){
			if (sizeof(value_type) < size_){//Copy and zero higher bits
				memcpy(data_, &value, sizeof(value_type));
				memset((data_ + sizeof(value_type)), 0, (size_ - sizeof(value_type)));
			}
			else//Copy applicable bits
				memcpy(data_, &value, size_);
		}

		template <typename target_type>
		target_type read() const{
			auto target = target_type();
			if (sizeof(&target) > size_){//Copy and zero higher bits
				memcpy(&target, data_, size_);
				memset(((&target) + size_), 0, (sizeof(target) - size_));
			}
			else//Copy applicable bits
				memcpy(&target, data_, size_);
		}

		virtual memory_register *match(size_type size) const;

		virtual memory_register *parent() const;

		virtual memory_register *low_child() const;

		virtual memory_register *high_child() const;

		const std::string &name() const;

		size_type size() const;

		char *data() const;

		virtual bool is_floating_point() const;

	protected:
		memory_register *parent_;
		memory_register *low_;
		memory_register *high_;
		std::string name_;
		size_type size_;
		char *data_;
	};

	template <class value_type>
	class typed_memory_register : public memory_register{
	public:
		typedef value_type value_type;

		explicit typed_memory_register(const std::string &name)
			: memory_register(nullptr, name, sizeof(value_type), typed_data_){
			std::memset(typed_data_, 0, sizeof(value_type));
		}

		virtual bool is_floating_point() const override{
			return std::is_floating_point_v<value_type>;
		}

	protected:
		char typed_data_[sizeof(value_type)];
	};
}

#endif /* !ELANG_MEMORY_REGISTER_H */
