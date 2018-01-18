#pragma once

#ifndef ELANG_BINARY_OUTPUT_WRITER_H
#define ELANG_BINARY_OUTPUT_WRITER_H

#include "error.h"
#include "output_writer.h"

namespace elang::common{
	class binary_output_writer : public output_writer{
	public:
		binary_output_writer(char *start, std::size_t size, std::size_t offset = 0)
			: start_(start), current_(start + offset), size_(size){}

		virtual ~binary_output_writer() = default;

		virtual void write(const char *buffer, std::size_t size = 0) override{
			if (size == 0u)//Compute size
				size = strlen(buffer);

			if (size != 0u)//Copy bytes
				memcpy(allocate(size), buffer, size);
		}

		virtual void write(const wchar_t *buffer, std::size_t size = 0) override{
			if (size == 0u)//Compute size
				size = wcslen(buffer);

			if (size != 0u)//Copy bytes
				memcpy(allocate(size * sizeof(wchar_t)), buffer, (size * sizeof(wchar_t)));
		}

		virtual void write(__int8 value) override{
			write((const char *)(&value), sizeof(__int8));
		}

		virtual void write(unsigned __int8 value) override{
			write((const char *)(&value), sizeof(unsigned __int8));
		}

		virtual void write(__int16 value) override{
			write((const char *)(&value), sizeof(__int16));
		}

		virtual void write(unsigned __int16 value) override{
			write((const char *)(&value), sizeof(unsigned __int16));
		}

		virtual void write(__int32 value) override{
			write((const char *)(&value), sizeof(__int32));
		}

		virtual void write(unsigned __int32 value) override{
			write((const char *)(&value), sizeof(unsigned __int32));
		}

		virtual void write(__int64 value) override{
			write((const char *)(&value), sizeof(__int64));
		}

		virtual void write(unsigned __int64 value) override{
			write((const char *)(&value), sizeof(unsigned __int64));
		}

		virtual void write(long double value) override{
			write((const char *)(&value), sizeof(long double));
		}

		template <typename value_type>
		void write(const value_type &value){
			write((const char *)(&value), sizeof(value_type));
		}

		template <typename value_type>
		void write(const value_type *buffer, std::size_t size){
			write(reinterpret_cast<const char *>(buffer), (sizeof(value_type) * size));
		}

		virtual char *allocate(std::size_t size){
			if ((size_ - (current_ - start_)) < size)
				throw error::write_buffer_overflow;

			auto old = current_;
			current_ += size;//Advance pointer

			return old;
		}

	protected:
		char *start_;
		char *current_;
		std::size_t size_;
	};
}

#endif /* !ELANG_BINARY_OUTPUT_WRITER_H */
