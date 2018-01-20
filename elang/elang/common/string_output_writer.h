#pragma once

#ifndef ELANG_STRING_OUTPUT_WRITER_H
#define ELANG_STRING_OUTPUT_WRITER_H

#include <string>

#include "output_writer.h"

namespace elang::common{
	class string_output_writer : public output_writer{
	public:
		using output_writer::write;

		string_output_writer(std::wstring &buffer)
			: buffer_(buffer){}

		virtual ~string_output_writer() = default;

		virtual void write(const char *buffer, std::size_t size = 0) override{
			if (size == 0u && (size = strlen(buffer)) == 0u)
				return;

			buffer_.reserve(buffer_.size() + size + 1);
			for (std::size_t index = 0; index < size; ++index)
				buffer_.append(1, buffer[index]);
		}

		virtual void write(const wchar_t *buffer, std::size_t size = 0) override{
			if (size == 0u)
				buffer_ += buffer;
			else//Known size
				buffer_.append(buffer, size);
		}

		virtual void write(__int8 value) override{
			write(std::to_wstring(value));
		}

		virtual void write(unsigned __int8 value) override{
			write(std::to_wstring(value));
		}

		virtual void write(__int16 value) override{
			write(std::to_wstring(value));
		}

		virtual void write(unsigned __int16 value) override{
			write(std::to_wstring(value));
		}

		virtual void write(__int32 value) override{
			write(std::to_wstring(value));
		}

		virtual void write(unsigned __int32 value) override{
			write(std::to_wstring(value));
		}

		virtual void write(__int64 value) override{
			write(std::to_wstring(value));
		}

		virtual void write(unsigned __int64 value) override{
			write(std::to_wstring(value));
		}

		virtual void write(long double value) override{
			write(std::to_wstring(value));
		}

	protected:
		std::wstring &buffer_;
	};
}

#endif /* !ELANG_STRING_OUTPUT_WRITER_H */
