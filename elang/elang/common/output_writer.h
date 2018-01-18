#pragma once

#ifndef ELANG_OUTPUT_WRITER_H
#define ELANG_OUTPUT_WRITER_H

#include <string>

namespace elang::common{
	class output_writer{
	public:
		virtual ~output_writer() = default;

		virtual void write(const char *buffer, std::size_t size = 0) = 0;

		virtual void write(const wchar_t *buffer, std::size_t size = 0) = 0;

		virtual void write(__int8 value) = 0;

		virtual void write(unsigned __int8 value) = 0;

		virtual void write(__int16 value) = 0;

		virtual void write(unsigned __int16 value) = 0;

		virtual void write(__int32 value) = 0;

		virtual void write(unsigned __int32 value) = 0;

		virtual void write(__int64 value) = 0;

		virtual void write(unsigned __int64 value) = 0;

		virtual void write(long double value) = 0;
	};
}

#endif /* !ELANG_OUTPUT_WRITER_H */
