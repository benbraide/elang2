#pragma once

#ifndef ELANG_STREAM_OUTPUT_WRITER_H
#define ELANG_STREAM_OUTPUT_WRITER_H

#include "output_writer.h"

namespace elang::common{
	template <class stream_type>
	class stream_output_writer : public output_writer{
	public:
		using output_writer::write;

		stream_output_writer(stream_type &buffer)
			: buffer_(buffer){}

		virtual ~stream_output_writer() = default;

		virtual void write(const char *buffer, std::size_t size = 0) override{
			if (size != 0u || (size = strlen(buffer)) != 0u)
				buffer_.write(buffer, size);
		}

		virtual void write(const wchar_t *buffer, std::size_t size = 0) override{}

		virtual void write(__int8 value) override{
			buffer_ << value;
		}

		virtual void write(unsigned __int8 value) override{
			buffer_ << value;
		}

		virtual void write(__int16 value) override{
			buffer_ << value;
		}

		virtual void write(unsigned __int16 value) override{
			buffer_ << value;
		}

		virtual void write(__int32 value) override{
			buffer_ << value;
		}

		virtual void write(unsigned __int32 value) override{
			buffer_ << value;
		}

		virtual void write(__int64 value) override{
			buffer_ << value;
		}

		virtual void write(unsigned __int64 value) override{
			buffer_ << value;
		}

		virtual void write(long double value) override{
			buffer_ << value;
		}

	protected:
		stream_type &buffer_;
	};
}

#endif /* !ELANG_STREAM_OUTPUT_WRITER_H */
