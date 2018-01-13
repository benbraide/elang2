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
			write(reinterpret_cast<const char *>(&value), sizeof(value_type));
		}

		void write(const char *value, size_type size = 0);

		template <typename value_type>
		void write(const value_type *value, size_type size){
			write(reinterpret_cast<const char *>(value), (size * sizeof(value_type)));
		}

		template <typename target_type>
		target_type read() const{
			auto target = target_type();
			read(reinterpret_cast<char *>(&target), sizeof(target_type));
			return target;
		}

		void read(char *value, size_type size = 0) const;

		template <typename value_type>
		void read(value_type *value, size_type size) const{
			read(reinterpret_cast<char *>(value), (size * sizeof(value_type)));
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
}

#endif /* !ELANG_MEMORY_REGISTER_H */
