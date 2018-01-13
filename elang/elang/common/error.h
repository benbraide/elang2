#pragma once

#ifndef ELANG_ERROR_H
#define ELANG_ERROR_H

namespace elang::common{
	enum class error{
		nil,
		register_not_found,
		stack_overflow,
		stack_underflow,
		byte_code_bad_source_operand_type,
		byte_code_bad_destination_operand_type,
		byte_code_unknown_operand_type,
	};
}

#endif /* !ELANG_ERROR_H */
