#pragma once

#ifndef ELANG_INPUT_READER_H
#define ELANG_INPUT_READER_H

#include <string>

namespace elang::common{
	class input_reader{
	public:
		virtual ~input_reader() = default;

		virtual void read(char *buffer, std::size_t size) = 0;

		virtual void read(wchar_t *buffer, std::size_t size) = 0;

		virtual void read(__int8 &value) = 0;

		virtual void read(unsigned __int8 &value) = 0;

		virtual void read(__int16 &value) = 0;

		virtual void read(unsigned __int16 &value) = 0;

		virtual void read(__int32 &value) = 0;

		virtual void read(unsigned __int32 &value) = 0;

		virtual void read(__int64 &value) = 0;

		virtual void read(unsigned __int64 &value) = 0;

		virtual void read(long double &value) = 0;
	};
}

#endif /* !ELANG_INPUT_READER_H */
