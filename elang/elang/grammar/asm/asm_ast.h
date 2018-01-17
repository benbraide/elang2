#pragma once

#ifndef ELANG_ASM_AST_H
#define ELANG_ASM_AST_H

#include "../../asm/asm_immediate_instruction_operand.h"
#include "../../asm/asm_label_instruction_operand.h"
#include "../../asm/asm_memory_instruction_operand.h"
#include "../../asm/asm_offset_instruction_operand.h"
#include "../../asm/asm_position_instruction_operand.h"
#include "../../asm/asm_register_instruction_operand.h"
#include "../../asm/asm_string_instruction_operand.h"
#include "../../asm/asm_uninitialized_instruction_operand.h"

#include "../../asm/asm_arithmetic_instruction.h"
#include "../../asm/asm_call_instruction.h"
#include "../../asm/asm_cmp_instruction.h"
#include "../../asm/asm_cnvt_instruction.h"
#include "../../asm/asm_jmp_instruction.h"
#include "../../asm/asm_lea_instruction.h"
#include "../../asm/asm_loop_instruction.h"
#include "../../asm/asm_mov_instruction.h"
#include "../../asm/asm_set_instruction.h"
#include "../../asm/asm_stack_instruction.h"
#include "../../asm/asm_test_instruction.h"

#include "../../asm/asm_section_id.h"

#include "../ast.h"

namespace elang::grammar{
	ELANG_AST_DECLARE_SINGLE(asm_uninitialized_value, char)

	ELANG_AST_DECLARE_SINGLE(asm_integral_value, __int64)
	ELANG_AST_DECLARE_SINGLE(asm_float_value, long double)

	ELANG_AST_DECLARE_SINGLE(asm_string, std::string)

	ELANG_AST_DECLARE_SINGLE_WPOS(asm_section, elang::easm::section_id)
	ELANG_AST_DECLARE_SINGLE_WPOS(asm_label, ELANG_AST_NAME(elang_identifier))

	using asm_offset_item_variant = boost::variant<ELANG_AST_NAME(asm_integral_value), ELANG_AST_NAME(elang_identifier)>;

	ELANG_AST_DECLARE_PAIR_WPOS(asm_offset_item, byte_code::operand_info::offset_op_type, asm_offset_item_variant)
	ELANG_AST_DECLARE_PAIR(asm_offset, asm_offset_item_variant, std::vector<ELANG_AST_NAME(asm_offset_item)>)
	ELANG_AST_DECLARE_SINGLE_WPOS(asm_offset_explicit, ELANG_AST_NAME(asm_offset))
	ELANG_AST_DECLARE_PAIR_WPOS(asm_memory, boost::optional<std::size_t>, ELANG_AST_NAME(asm_offset))

	ELANG_AST_DECLARE_SINGLE_VARIANT(
		asm_operand,
		ELANG_AST_NAME(asm_uninitialized_value),
		ELANG_AST_NAME(asm_integral_value),
		ELANG_AST_NAME(asm_float_value),
		ELANG_AST_NAME(asm_offset_explicit),
		ELANG_AST_NAME(asm_string),
		ELANG_AST_NAME(elang_identifier),
		ELANG_AST_NAME(asm_memory)
	)

	enum class asm_instruction_id{
		nop,
		ret,
		push,
		pop,
		int_,
		call,
		jmp,
		jz,
		jnz,
		je,
		jne,
		jle,
		jl,
		jnle,
		jnl,
		jge,
		jg,
		jnge,
		jng,
		setz,
		setnz,
		sete,
		setne,
		setle,
		setl,
		setnle,
		setnl,
		setge,
		setg,
		setnge,
		setng,
		loop,
		inc,
		dec,
		not,
		lea,
		mov,
		add,
		sub,
		mult,
		div,
		mod,
		and_,
		xor_,
		or_,
		sal,
		sar,
		test,
		cmp,
		db,
		dw,
		dd,
		dq,
		df,
		dz,
	};

	ELANG_AST_DECLARE_PAIR_WPOS(asm_instruction, asm_instruction_id, std::vector<ELANG_AST_NAME(asm_operand)>)
	ELANG_AST_DECLARE_PAIR_WPOS(asm_times_instruction, unsigned int, ELANG_AST_NAME(asm_instruction))

	ELANG_AST_DECLARE_SINGLE_VARIANT(
		asm_instruction_set_value,
		ELANG_AST_NAME(asm_section),
		ELANG_AST_NAME(asm_label),
		ELANG_AST_NAME(asm_instruction),
		ELANG_AST_NAME(asm_times_instruction)
	)

	ELANG_AST_DECLARE_SINGLE(asm_instruction_set, std::vector<ELANG_AST_NAME(asm_instruction_set_value)>)
}

ELANG_AST_ADAPT_SINGLE(asm_uninitialized_value)

ELANG_AST_ADAPT_SINGLE(asm_integral_value)
ELANG_AST_ADAPT_SINGLE(asm_float_value)

ELANG_AST_ADAPT_SINGLE(asm_string)

ELANG_AST_ADAPT_SINGLE(asm_section)
ELANG_AST_ADAPT_SINGLE(asm_label)

ELANG_AST_ADAPT_PAIR(asm_offset_item)
ELANG_AST_ADAPT_PAIR(asm_offset)
ELANG_AST_ADAPT_SINGLE(asm_offset_explicit)
ELANG_AST_ADAPT_PAIR(asm_memory)

ELANG_AST_ADAPT_SINGLE(asm_operand);

ELANG_AST_ADAPT_PAIR(asm_instruction)
ELANG_AST_ADAPT_PAIR(asm_times_instruction)

ELANG_AST_ADAPT_SINGLE(asm_instruction_set_value)
ELANG_AST_ADAPT_SINGLE(asm_instruction_set)

#endif /* !ELANG_ASM_AST_H */
