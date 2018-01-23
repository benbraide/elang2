#pragma once

#ifndef ELANG_STREAM_INPUT_READER_H
#define ELANG_STREAM_INPUT_READER_H

#include "input_reader.h"

namespace elang::common{
	template <class stream_type>
	class stream_input_reader : public input_reader{
	public:
		using input_reader::read;

		stream_input_reader(stream_type &buffer)
			: buffer_(buffer){}

		virtual ~stream_input_reader() = default;

		virtual void read(char *buffer, std::size_t size) override{
			buffer_.read(buffer, size);
		}

		virtual void read(wchar_t *buffer, std::size_t size) override{}

		virtual void read(__int8 &value) override{
			buffer_ >> value;
		}

		virtual void read(unsigned __int8 &value) override{
			buffer_ >> value;
		}

		virtual void read(__int16 &value) override{
			buffer_ >> value;
		}

		virtual void read(unsigned __int16 &value) override{
			buffer_ >> value;
		}

		virtual void read(__int32 &value) override{
			buffer_ >> value;
		}

		virtual void read(unsigned __int32 &value) override{
			buffer_ >> value;
		}

		virtual void read(__int64 &value) override{
			buffer_ >> value;
		}

		virtual void read(unsigned __int64 &value) override{
			buffer_ >> value;
		}

		virtual void read(long double &value) override{
			buffer_ >> value;
		}

	protected:
		stream_type &buffer_;
	};
}

#endif /* !ELANG_STREAM_INPUT_READER_H */
