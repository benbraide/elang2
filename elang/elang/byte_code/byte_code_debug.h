#pragma once

#ifndef ELANG_BYTE_CODE_DEBUG_H
#define ELANG_BYTE_CODE_DEBUG_H

#include <string>

#include "../common/output_writer.h"

#include "byte_code_id.h"

namespace elang::byte_code{
	class debug{
	public:
		explicit debug(common::output_writer &writer);

		void log(id id);

		void log(const std::string &str);

		void log_operand(const std::string &str);

		static std::string convert_id(id id);

		static thread_local debug *debugger;

	private:
		common::output_writer &writer_;
		unsigned int op_count_;
		bool first_instruction_;
	};
}

#endif /* !ELANG_BYTE_CODE_DEBUG_H */
