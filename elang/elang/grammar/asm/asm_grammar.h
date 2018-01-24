#pragma once

#ifndef ELANG_ASM_GRAMMAR_H
#define ELANG_ASM_GRAMMAR_H

#include "../grammar.h"

#include "asm_ast.h"

namespace elang::grammar{
	struct asm_section_symbols : x3::symbols<elang::easm::section_id>{
		asm_section_symbols(){
			add
				(".rodata", elang::easm::section_id::rodata)
				(".data", elang::easm::section_id::data)
				(".text", elang::easm::section_id::text)
				;
		}
	} asm_section_symbols_;

	struct asm_type_symbols : x3::symbols<std::size_t>{
		asm_type_symbols(){
			add
				("byte",	1u)
				("word",	2u)
				("dword",	4u)
				("qword",	8u)
				("float",	10u)
				;
		}
	} asm_type_symbols_;

	struct asm_mnemonic_symbols : x3::symbols<asm_instruction_id>{
		asm_mnemonic_symbols(){
			add
				("nop", asm_instruction_id::nop)
				("ret", asm_instruction_id::ret)
				("push", asm_instruction_id::push)
				("pop", asm_instruction_id::pop)
				("syscall", asm_instruction_id::int_)
				("call", asm_instruction_id::call)
				("jmp", asm_instruction_id::jmp)
				("jz", asm_instruction_id::jz)
				("jnz", asm_instruction_id::jnz)
				("je", asm_instruction_id::je)
				("jne", asm_instruction_id::jne)
				("jle", asm_instruction_id::jle)
				("jl", asm_instruction_id::jl)
				("jnle", asm_instruction_id::jnle)
				("jnl", asm_instruction_id::jnl)
				("jge", asm_instruction_id::jge)
				("jg", asm_instruction_id::jg)
				("jnge", asm_instruction_id::jnge)
				("jng", asm_instruction_id::jng)
				("setz", asm_instruction_id::setz)
				("setnz", asm_instruction_id::setnz)
				("sete", asm_instruction_id::sete)
				("setne", asm_instruction_id::setne)
				("setle", asm_instruction_id::setle)
				("setl", asm_instruction_id::setl)
				("setnle", asm_instruction_id::setnle)
				("setnl", asm_instruction_id::setnl)
				("setge", asm_instruction_id::setge)
				("setg", asm_instruction_id::setg)
				("setnge", asm_instruction_id::setnge)
				("setng", asm_instruction_id::setng)
				("loop", asm_instruction_id::loop)
				("inc", asm_instruction_id::inc)
				("dec", asm_instruction_id::dec)
				("not", asm_instruction_id::not)
				("lea", asm_instruction_id::lea)
				("mov", asm_instruction_id::mov)
				("add", asm_instruction_id::add)
				("sub", asm_instruction_id::sub)
				("mult", asm_instruction_id::mult)
				("div", asm_instruction_id::div)
				("mod", asm_instruction_id::mod)
				("and", asm_instruction_id::and_)
				("xor", asm_instruction_id::xor_)
				("or", asm_instruction_id::or_)
				("sal", asm_instruction_id::sal)
				("sar", asm_instruction_id::sar)
				("test", asm_instruction_id::test)
				("cmp", asm_instruction_id::cmp)
				;
		}
	} asm_mnemonic_symbols_;

	struct asm_int_decl_mnemonic_symbols : x3::symbols<asm_instruction_id>{
		asm_int_decl_mnemonic_symbols(){
			add
				("db", asm_instruction_id::db)
				("dw", asm_instruction_id::dw)
				("dd", asm_instruction_id::dd)
				("dq", asm_instruction_id::dq)
				;
		}
	} asm_int_decl_mnemonic_symbols_;

	struct asm_flt_decl_mnemonic_symbols : x3::symbols<asm_instruction_id>{
		asm_flt_decl_mnemonic_symbols(){
			add
				("df", asm_instruction_id::df)
				;
		}
	} asm_flt_decl_mnemonic_symbols_;

	struct asm_offset_op_symbols : x3::symbols<elang::byte_code::operand_info::offset_op_type>{
		asm_offset_op_symbols(){
			add
				("+", elang::byte_code::operand_info::offset_op_type::add)
				("-", elang::byte_code::operand_info::offset_op_type::sub)
				;
		}
	} asm_offset_op_symbols_;

	struct asm_integral_operator_symbols : x3::symbols<asm_operator_id>{
		asm_integral_operator_symbols(){
			add
				("+", asm_operator_id::add)
				("-", asm_operator_id::sub)
				("*", asm_operator_id::mult)
				("/", asm_operator_id::div)
				("%", asm_operator_id::mod)
				("&", asm_operator_id::and_)
				("|", asm_operator_id::xor_)
				("^", asm_operator_id::or_)
				("<<", asm_operator_id::sal)
				(">>", asm_operator_id::sar)
				;
		}
	} asm_integral_operator_symbols_;

	struct asm_float_operator_symbols : x3::symbols<asm_operator_id>{
		asm_float_operator_symbols(){
			add
				("+", asm_operator_id::add)
				("-", asm_operator_id::sub)
				("*", asm_operator_id::mult)
				("/", asm_operator_id::div)
				;
		}
	} asm_float_operator_symbols_;

	auto asm_int_expression_begin = [](auto &ctx){
		x3::_val(ctx) = boost::get<ELANG_AST_NAME(asm_integral_value)>(x3::_attr(ctx));
	};

	auto asm_flt_expression_begin = [](auto &ctx){
		x3::_val(ctx) = boost::get<ELANG_AST_NAME(asm_float_value)>(x3::_attr(ctx));
	};

	auto asm_integral_expr = [](auto &ctx){
		switch (boost::fusion::at<boost::mpl::int_<0>>(x3::_attr(ctx))){
		case asm_operator_id::sub:
			x3::_val(ctx).value = (x3::_val(ctx).value -
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::mult:
			x3::_val(ctx).value = (x3::_val(ctx).value *
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::div:
			x3::_val(ctx).value = (x3::_val(ctx).value /
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::mod:
			x3::_val(ctx).value = (x3::_val(ctx).value %
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::and_:
			x3::_val(ctx).value = (x3::_val(ctx).value &
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::xor_:
			x3::_val(ctx).value = (x3::_val(ctx).value ^
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::or_:
			x3::_val(ctx).value = (x3::_val(ctx).value |
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::sal:
			x3::_val(ctx).value = (x3::_val(ctx).value <<
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::sar:
			x3::_val(ctx).value = (x3::_val(ctx).value >>
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		default:
			x3::_val(ctx).value = (x3::_val(ctx).value +
				boost::get<ELANG_AST_NAME(asm_integral_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		}
	};

	auto asm_float_expr = [](auto &ctx){
		switch (boost::fusion::at<boost::mpl::int_<0>>(x3::_attr(ctx))){
		case asm_operator_id::sub:
			x3::_val(ctx).value = (x3::_val(ctx).value -
				boost::get<ELANG_AST_NAME(asm_float_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::mult:
			x3::_val(ctx).value = (x3::_val(ctx).value *
				boost::get<ELANG_AST_NAME(asm_float_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		case asm_operator_id::div:
			x3::_val(ctx).value = (x3::_val(ctx).value /
				boost::get<ELANG_AST_NAME(asm_float_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		default:
			x3::_val(ctx).value = (x3::_val(ctx).value +
				boost::get<ELANG_AST_NAME(asm_float_value)>(boost::fusion::at<boost::mpl::int_<1>>(x3::_attr(ctx))).value);
			break;
		}
	};

	ELANG_GRAMMAR_DECLARE_RULE(asm_uninitialized_value)
	ELANG_GRAMMAR_RULE_DEF(asm_uninitialized_value) = x3::char_('?');

	ELANG_GRAMMAR_DECLARE_RULE(asm_float_value)
	ELANG_GRAMMAR_RULE_DEF(asm_float_value) = long_double_;

	hex_parser<__int64> const asm_long_long_hex = {};
	oct_parser<__int64> const asm_long_long_oct = {};
	bin_parser<__int64> const asm_long_long_bin = {};

	ELANG_GRAMMAR_DECLARE_RULE(asm_integral_value)
	ELANG_GRAMMAR_RULE_DEF(asm_integral_value) = (
		x3::lexeme["0x" >> asm_long_long_hex] |
		x3::lexeme["0b" >> asm_long_long_bin] |
		x3::lexeme["0" >> asm_long_long_oct] |
		x3::long_long
	);

	ELANG_GRAMMAR_DECLARE_RULE2(asm_integral_expression, asm_integral_value)
	ELANG_GRAMMAR_DECLARE_RULE2(asm_integral_group, asm_integral_value)

	ELANG_GRAMMAR_RULE_DEF(asm_integral_group) = ('(' > ELANG_GRAMMAR_RULE_NAME(asm_integral_expression) > ')');
	ELANG_GRAMMAR_RULE_DEF(asm_integral_expression) = (
		(ELANG_GRAMMAR_RULE_NAME(asm_integral_group) | ELANG_GRAMMAR_RULE_NAME(asm_integral_value))[asm_int_expression_begin] >>
		*(
			asm_integral_operator_symbols_ >
			(ELANG_GRAMMAR_RULE_NAME(asm_integral_group) | ELANG_GRAMMAR_RULE_NAME(asm_integral_value))
		)[asm_integral_expr]
	);

	ELANG_GRAMMAR_DECLARE_RULE2(asm_float_expression, asm_float_value)
	ELANG_GRAMMAR_DECLARE_RULE2(asm_float_group, asm_float_value)

	ELANG_GRAMMAR_RULE_DEF(asm_float_group) = ('(' > ELANG_GRAMMAR_RULE_NAME(asm_float_expression) > ')');
	ELANG_GRAMMAR_RULE_DEF(asm_float_expression) = (
		(ELANG_GRAMMAR_RULE_NAME(asm_float_group) | ELANG_GRAMMAR_RULE_NAME(asm_float_value))[asm_flt_expression_begin] >>
		*(
			asm_float_operator_symbols_ >
			(ELANG_GRAMMAR_RULE_NAME(asm_float_group) | ELANG_GRAMMAR_RULE_NAME(asm_float_value))
		)[asm_float_expr]
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_string)
	ELANG_GRAMMAR_RULE_DEF(asm_string) = ("'" > x3::lexeme[*((x3::char_('\\') >> "'") | ~x3::char_("'"))] > "'");

	ELANG_GRAMMAR_DECLARE_RULE(asm_section)
	ELANG_GRAMMAR_RULE_DEF(asm_section) = x3::no_case[(utils::keyword("section") > asm_section_symbols_ > (x3::eol | x3::eoi))];

	ELANG_GRAMMAR_DECLARE_RULE(asm_label)
	ELANG_GRAMMAR_RULE_DEF(asm_label) = x3::lexeme[ELANG_GRAMMAR_RULE_NAME(elang_identifier) > ":" > (x3::eol | x3::eoi)];

	ELANG_GRAMMAR_DECLARE_RULE(asm_offset_item)
	ELANG_GRAMMAR_RULE_DEF(asm_offset_item) = (
		asm_offset_op_symbols_ > (ELANG_GRAMMAR_RULE_NAME(asm_integral_expression) | ELANG_GRAMMAR_RULE_NAME(elang_identifier))
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_offset)
	ELANG_GRAMMAR_RULE_DEF(asm_offset) = (
		(ELANG_GRAMMAR_RULE_NAME(asm_integral_expression) | ELANG_GRAMMAR_RULE_NAME(elang_identifier))
		>> *ELANG_GRAMMAR_RULE_NAME(asm_offset_item)
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_offset_explicit)
	ELANG_GRAMMAR_RULE_DEF(asm_offset_explicit) = (x3::no_case[utils::keyword("offset")] > ELANG_GRAMMAR_RULE_NAME(asm_offset));

	ELANG_GRAMMAR_DECLARE_RULE(asm_memory)
	ELANG_GRAMMAR_RULE_DEF(asm_memory) = (-x3::no_case[asm_type_symbols_] >> '[' > -x3::no_case[utils::keyword("offset")] > ELANG_GRAMMAR_RULE_NAME(asm_offset) > ']');

	ELANG_GRAMMAR_DECLARE_RULE(asm_operand)
	ELANG_GRAMMAR_RULE_DEF(asm_operand) = (
		ELANG_GRAMMAR_RULE_NAME(asm_uninitialized_value) |
		ELANG_GRAMMAR_RULE_NAME(asm_string) |
		ELANG_GRAMMAR_RULE_NAME(asm_memory) |
		ELANG_GRAMMAR_RULE_NAME(asm_offset_explicit) |
		ELANG_GRAMMAR_RULE_NAME(elang_identifier) |
		ELANG_GRAMMAR_RULE_NAME(asm_float_value) |
		ELANG_GRAMMAR_RULE_NAME(asm_integral_value)
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_instruction)
	ELANG_GRAMMAR_RULE_DEF(asm_instruction) = (
		utils::keyword(x3::no_case[asm_mnemonic_symbols_]) >> -(ELANG_GRAMMAR_RULE_NAME(asm_operand) % ",") > (x3::eol | x3::eoi)
	);

	ELANG_GRAMMAR_DECLARE_RULE2(asm_int_decl_instruction, asm_instruction)
	ELANG_GRAMMAR_RULE_DEF(asm_int_decl_instruction) = (
		utils::keyword(x3::no_case[asm_int_decl_mnemonic_symbols_]) > (ELANG_GRAMMAR_RULE_NAME(asm_offset) % ",") > (x3::eol | x3::eoi)
	);

	ELANG_GRAMMAR_DECLARE_RULE2(asm_flt_decl_instruction, asm_instruction)
	ELANG_GRAMMAR_RULE_DEF(asm_flt_decl_instruction) = (
		utils::keyword(x3::no_case[asm_flt_decl_mnemonic_symbols_]) > (ELANG_GRAMMAR_RULE_NAME(asm_float_expression) % ",") > (x3::eol | x3::eoi)
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_equ_instruction)
	ELANG_GRAMMAR_RULE_DEF(asm_equ_instruction) = (
		ELANG_GRAMMAR_RULE_NAME(elang_identifier) >> x3::no_case[utils::keyword("equ")] >
		ELANG_GRAMMAR_RULE_NAME(asm_offset) > (x3::eol | x3::eoi)
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_times_instruction)
	ELANG_GRAMMAR_RULE_DEF(asm_times_instruction) = (
		x3::no_case[utils::keyword("times")] > x3::uint_ >
		(ELANG_GRAMMAR_RULE_NAME(asm_instruction) | ELANG_GRAMMAR_RULE_NAME(asm_int_decl_instruction) | ELANG_GRAMMAR_RULE_NAME(asm_flt_decl_instruction))
	);

	ELANG_GRAMMAR_DECLARE_RULE2(asm_decl_times_instruction, asm_times_instruction)
	ELANG_GRAMMAR_RULE_DEF(asm_decl_times_instruction) = (
		x3::no_case[utils::keyword("times")] > x3::uint_ >
		(ELANG_GRAMMAR_RULE_NAME(asm_int_decl_instruction) | ELANG_GRAMMAR_RULE_NAME(asm_flt_decl_instruction))
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_named_decl)
	ELANG_GRAMMAR_RULE_DEF(asm_named_decl) = (
		ELANG_GRAMMAR_RULE_NAME(elang_identifier) >>
		(ELANG_GRAMMAR_RULE_NAME(asm_decl_times_instruction) | ELANG_GRAMMAR_RULE_NAME(asm_int_decl_instruction) |
			ELANG_GRAMMAR_RULE_NAME(asm_flt_decl_instruction))
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_stack)
	ELANG_GRAMMAR_RULE_DEF(asm_stack) = (x3::no_case[utils::keyword(".stack")] > x3::uint64 > (x3::eol | x3::eoi));

	ELANG_GRAMMAR_DECLARE_RULE(asm_global)
	ELANG_GRAMMAR_RULE_DEF(asm_global) = (x3::no_case[utils::keyword(".global")] > ELANG_GRAMMAR_RULE_NAME(elang_identifier) > (x3::eol | x3::eoi));

	ELANG_GRAMMAR_DECLARE_RULE(asm_dzero)
	ELANG_GRAMMAR_RULE_DEF(asm_dzero) = (x3::no_case[utils::keyword(".zero")] > x3::uint64 > (x3::eol | x3::eoi));

	ELANG_GRAMMAR_DECLARE_RULE(asm_dstring)
	ELANG_GRAMMAR_RULE_DEF(asm_dstring) = (x3::no_case[utils::keyword(".string")] > ELANG_GRAMMAR_RULE_NAME(asm_string) > (x3::eol | x3::eoi));

	ELANG_GRAMMAR_DECLARE_RULE(asm_instruction_set_value)
	ELANG_GRAMMAR_RULE_DEF(asm_instruction_set_value) = (
		ELANG_GRAMMAR_RULE_NAME(asm_section) |
		ELANG_GRAMMAR_RULE_NAME(asm_times_instruction) |
		ELANG_GRAMMAR_RULE_NAME(asm_named_decl) |
		ELANG_GRAMMAR_RULE_NAME(asm_equ_instruction) |
		ELANG_GRAMMAR_RULE_NAME(asm_stack) |
		ELANG_GRAMMAR_RULE_NAME(asm_global) |
		ELANG_GRAMMAR_RULE_NAME(asm_dzero) |
		ELANG_GRAMMAR_RULE_NAME(asm_dstring) |
		ELANG_GRAMMAR_RULE_NAME(asm_instruction) |
		ELANG_GRAMMAR_RULE_NAME(asm_int_decl_instruction) |
		ELANG_GRAMMAR_RULE_NAME(asm_flt_decl_instruction) |
		ELANG_GRAMMAR_RULE_NAME(asm_label)
	);

	ELANG_GRAMMAR_DECLARE_RULE(asm_instruction_set)
	ELANG_GRAMMAR_RULE_DEF(asm_instruction_set) = *((x3::eol >> x3::attr(false)) | ELANG_GRAMMAR_RULE_NAME(asm_instruction_set_value));

	ELANG_GRAMMAR_DECLARE_NO_AST_RULE(asm_skip)
	ELANG_GRAMMAR_RULE_DEF(asm_skip) = ((x3::space - x3::eol) | (';' >> *x3::omit[(x3::char_ - x3::eol)]));

	BOOST_SPIRIT_DEFINE(
		ELANG_GRAMMAR_RULE_NAME(asm_uninitialized_value),
		ELANG_GRAMMAR_RULE_NAME(asm_float_value),
		ELANG_GRAMMAR_RULE_NAME(asm_integral_value),
		ELANG_GRAMMAR_RULE_NAME(asm_integral_group),
		ELANG_GRAMMAR_RULE_NAME(asm_integral_expression),
		ELANG_GRAMMAR_RULE_NAME(asm_float_group),
		ELANG_GRAMMAR_RULE_NAME(asm_float_expression),
		ELANG_GRAMMAR_RULE_NAME(asm_string),
		ELANG_GRAMMAR_RULE_NAME(asm_section),
		ELANG_GRAMMAR_RULE_NAME(asm_label),
		ELANG_GRAMMAR_RULE_NAME(asm_offset_item),
		ELANG_GRAMMAR_RULE_NAME(asm_offset),
		ELANG_GRAMMAR_RULE_NAME(asm_offset_explicit),
		ELANG_GRAMMAR_RULE_NAME(asm_memory),
		ELANG_GRAMMAR_RULE_NAME(asm_operand),
		ELANG_GRAMMAR_RULE_NAME(asm_instruction),
		ELANG_GRAMMAR_RULE_NAME(asm_int_decl_instruction),
		ELANG_GRAMMAR_RULE_NAME(asm_flt_decl_instruction),
		ELANG_GRAMMAR_RULE_NAME(asm_equ_instruction),
		ELANG_GRAMMAR_RULE_NAME(asm_times_instruction),
		ELANG_GRAMMAR_RULE_NAME(asm_decl_times_instruction),
		ELANG_GRAMMAR_RULE_NAME(asm_named_decl),
		ELANG_GRAMMAR_RULE_NAME(asm_instruction_set_value),
		ELANG_GRAMMAR_RULE_NAME(asm_instruction_set),
		ELANG_GRAMMAR_RULE_NAME(asm_stack),
		ELANG_GRAMMAR_RULE_NAME(asm_global),
		ELANG_GRAMMAR_RULE_NAME(asm_dzero),
		ELANG_GRAMMAR_RULE_NAME(asm_dstring),
		ELANG_GRAMMAR_RULE_NAME(asm_skip)
	)
}

#endif /* !ELANG_ASM_GRAMMAR_H */
