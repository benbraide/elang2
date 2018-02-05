#pragma once

#ifndef ELANG_ERROR_H
#define ELANG_ERROR_H

namespace elang::common{
	enum class error{
		nil,
		file_not_found,
		file_not_opened,
		write_buffer_overflow,
		no_input_reader,
		no_output_writer,
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
		asm_bad_operand_type,
		asm_bad_operand_size,
		asm_bad_operand_count,
		asm_bad_contant_value,
		asm_too_many_expression_operands,
		asm_label_not_found,
		asm_label_redefinition,
		asm_ambiguous_instruction,
		asm_no_active_section,
		asm_parse_error,
		asm_no_start_address,
		lang_out_of_registers,
		lang_bad_operand,
		lang_bad_type,
		lang_bad_base_type,
		lang_symbol_exists,
		lang_base_type_exists,
		lang_unmatched_call,
		lang_ambiguous_call,
		lang_division_by_zero,
		lang_invalid_operation,
	};
}

#endif /* !ELANG_ERROR_H */
