#pragma once

#ifndef ELANG_OUTPUT_WRITER_H
#define ELANG_OUTPUT_WRITER_H

#include <string>

namespace elang::common{
	class output_writer{
	public:
		virtual ~output_writer() = default;

		virtual char *allocate(std::size_t size) = 0;

		virtual void write(const char *buffer, std::size_t size = 0){
			if (size == 0u)//Compute size
				size = strlen(buffer);

			if (size != 0u)//Copy bytes
				memcpy(allocate(size), buffer, size);
		}

		virtual void write(__int8 value){
			write((const char *)(&value), sizeof(__int8));
		}

		virtual void write(unsigned __int8 value){
			write((const char *)(&value), sizeof(unsigned __int8));
		}

		virtual void write(__int16 value){
			write((const char *)(&value), sizeof(__int16));
		}

		virtual void write(unsigned __int16 value){
			write((const char *)(&value), sizeof(unsigned __int16));
		}

		virtual void write(__int32 value){
			write((const char *)(&value), sizeof(__int32));
		}

		virtual void write(unsigned __int32 value){
			write((const char *)(&value), sizeof(unsigned __int32));
		}

		virtual void write(__int64 value){
			write((const char *)(&value), sizeof(__int64));
		}

		virtual void write(unsigned __int64 value){
			write((const char *)(&value), sizeof(unsigned __int64));
		}

		virtual void write(long double value){
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
	};
}

#endif /* !ELANG_OUTPUT_WRITER_H */
