#pragma once

#ifndef ELANG_ERROR_H
#define ELANG_ERROR_H

namespace elang::common{
	enum class error{
		nil,
		memory_read_access_violation,
		memory_write_access_violation,
		memory_out_of_space,
		memory_invalid_size,
		register_not_found,
		stack_overflow,
		stack_underflow,
		byte_code_memory_read_access_violation,
		byte_code_memory_write_access_violation,
		byte_code_bad_source_operand_type,
		byte_code_bad_destination_operand_type,
		byte_code_unknown_operand_type,
		byte_code_bad_operand_size,
		byte_code_unknown_instruction,
		byte_code_syscall_unknown,
	};
}

#endif /* !ELANG_ERROR_H */
