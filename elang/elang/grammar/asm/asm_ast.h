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
#include "../../asm/asm_decl_instruction.h"
#include "../../asm/asm_jmp_instruction.h"
#include "../../asm/asm_lea_instruction.h"
#include "../../asm/asm_loop_instruction.h"
#include "../../asm/asm_mov_instruction.h"
#include "../../asm/asm_set_instruction.h"
#include "../../asm/asm_stack_instruction.h"
#include "../../asm/asm_test_instruction.h"

#include "../../asm/asm_instruction_table.h"

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

	ELANG_AST_DECLARE_SINGLE_WPOS(asm_stack, unsigned __int64)
	ELANG_AST_DECLARE_SINGLE_WPOS(asm_global, ELANG_AST_NAME(elang_identifier))

	ELANG_AST_DECLARE_SINGLE_WPOS(asm_dzero, unsigned __int64)
	ELANG_AST_DECLARE_SINGLE_WPOS(asm_dstring, ELANG_AST_NAME(asm_string))

	ELANG_AST_DECLARE_SINGLE_VARIANT(
		asm_instruction_set_value,
		ELANG_AST_NAME(asm_section),
		ELANG_AST_NAME(asm_label),
		ELANG_AST_NAME(asm_instruction),
		ELANG_AST_NAME(asm_times_instruction),
		ELANG_AST_NAME(asm_stack),
		ELANG_AST_NAME(asm_global),
		ELANG_AST_NAME(asm_dzero),
		ELANG_AST_NAME(asm_dstring),
		bool
	)

	ELANG_AST_DECLARE_SINGLE(asm_instruction_set, std::vector<ELANG_AST_NAME(asm_instruction_set_value)>)

	class asm_ast_visitor{
	public:
		explicit asm_ast_visitor(easm::instruction_table &table)
			: table_(table){}

		void operator ()(bool) const{}

		void operator ()(ELANG_AST_NAME(asm_dstring) &ast) const{
			std::vector<std::shared_ptr<elang::easm::instruction_operand_object>> operands({ operator()(ast.value) });
			table_.add(std::make_shared<elang::easm::db_instruction>(std::move(operands)));
		}

		void operator ()(ELANG_AST_NAME(asm_dzero) &ast) const{

		}

		void operator ()(ELANG_AST_NAME(asm_global) &ast) const{
			table_.set_start_label(ast.value.value);
		}

		void operator ()(ELANG_AST_NAME(asm_stack) &ast) const{
			table_.set_stack_size(ast.value);
		}

		void operator ()(ELANG_AST_NAME(asm_times_instruction) &ast) const{
			for (auto count = 0u; count < ast.first; ++count)
				operator()(ast.second);
		}

		void operator ()(ELANG_AST_NAME(asm_instruction) &ast) const{
			table_.add(instruction(ast, *this));
		}

		void operator ()(ELANG_AST_NAME(asm_label) &ast) const{
			table_.add(ast.value.value);
		}

		void operator ()(ELANG_AST_NAME(asm_section) &ast) const{
			table_.add(ast.value);
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(asm_operand) &ast) const{
			return boost::apply_visitor(*this, ast.value);
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(asm_memory) &ast) const{
			return std::make_shared<elang::easm::memory_instruction_operand>(operator()(ast.second), ast.first.value_or(0u));
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(elang_identifier) &ast) const{
			if (ast.value.size() <= 5u){//Try register
				auto value = ast.value;
				for (auto &c : value)
					c = tolower(c);

				auto reg = table_.find_register(ast.value);
				if (reg != nullptr)
					return std::make_shared<elang::easm::register_instruction_operand>(*reg);
			}
			
			return std::make_shared<elang::easm::label_instruction_operand>(table_.find_label(ast.value));
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(asm_string) &ast) const{
			return std::make_shared<elang::easm::string_instruction_operand>(std::move(ast.value));
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(asm_offset_explicit) &ast) const{
			return operator()(ast.value);
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(asm_offset) &ast) const{
			std::vector<elang::easm::offset_instruction_operand::item_info> list;

			list.reserve(ast.second.size() + 1);
			list.push_back(elang::easm::offset_instruction_operand::item_info{ 
				byte_code::operand_info::offset_op_type::add,
				boost::apply_visitor(*this, ast.first)
			});

			for (auto &item : ast.second)
				list.push_back(elang::easm::offset_instruction_operand::item_info{ item.first, boost::apply_visitor(*this, item.second) });

			return std::make_shared<elang::easm::offset_instruction_operand>(std::move(list));
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(asm_float_value) &ast) const{
			return std::make_shared<elang::easm::immediate_instruction_operand<long double>>(ast.value);
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(asm_integral_value) &ast) const{
			return std::make_shared<elang::easm::immediate_instruction_operand<__int64>>(ast.value);
		}

		std::shared_ptr<elang::easm::instruction_operand_object> operator ()(ELANG_AST_NAME(asm_uninitialized_value) &ast) const{
			return std::make_shared<elang::easm::uninitialized_instruction_operand>();
		}

		static void visit(ELANG_AST_NAME(asm_instruction_set) &ast, easm::instruction_table &table){
			asm_ast_visitor visitor(table);
			for (auto &item : ast.value)
				boost::apply_visitor(visitor, item.value);
		}

		static std::shared_ptr<elang::easm::instruction_object> instruction(ELANG_AST_NAME(asm_instruction) &ast, const asm_ast_visitor &inst){
			std::vector<std::shared_ptr<elang::easm::instruction_operand_object>> operands;
			if (!ast.second.empty())
				operands.reserve(ast.second.size());

			for (auto &operand : ast.second)
				operands.push_back(inst.operator()(operand));

			switch (ast.first){
			case asm_instruction_id::nop:
				return std::make_shared<elang::easm::nop_instruction>(std::move(operands));
			case asm_instruction_id::ret:
				return std::make_shared<elang::easm::ret_instruction>(std::move(operands));
			case asm_instruction_id::push:
				return std::make_shared<elang::easm::push_instruction>(std::move(operands));
			case asm_instruction_id::pop:
				return std::make_shared<elang::easm::pop_instruction>(std::move(operands));
			case asm_instruction_id::int_:
				return std::make_shared<elang::easm::syscall_instruction>(std::move(operands));
			case asm_instruction_id::call:
				return std::make_shared<elang::easm::call_instruction>(std::move(operands));
			case asm_instruction_id::jmp:
				return std::make_shared<elang::easm::jmp_instruction>(std::move(operands));
			case asm_instruction_id::jz:
				return std::make_shared<elang::easm::jz_instruction>(std::move(operands));
			case asm_instruction_id::jnz:
				return std::make_shared<elang::easm::jnz_instruction>(std::move(operands));
			case asm_instruction_id::je:
				return std::make_shared<elang::easm::je_instruction>(std::move(operands));
			case asm_instruction_id::jne:
				return std::make_shared<elang::easm::jne_instruction>(std::move(operands));
			case asm_instruction_id::jle:
				return std::make_shared<elang::easm::jle_instruction>(std::move(operands));
			case asm_instruction_id::jl:
				return std::make_shared<elang::easm::jl_instruction>(std::move(operands));
			case asm_instruction_id::jnle:
				return std::make_shared<elang::easm::jnle_instruction>(std::move(operands));
			case asm_instruction_id::jnl:
				return std::make_shared<elang::easm::jnl_instruction>(std::move(operands));
			case asm_instruction_id::jge:
				return std::make_shared<elang::easm::jge_instruction>(std::move(operands));
			case asm_instruction_id::jg:
				return std::make_shared<elang::easm::jg_instruction>(std::move(operands));
			case asm_instruction_id::jnge:
				return std::make_shared<elang::easm::jnge_instruction>(std::move(operands));
			case asm_instruction_id::jng:
				return std::make_shared<elang::easm::jng_instruction>(std::move(operands));
			case asm_instruction_id::loop:
				return std::make_shared<elang::easm::loop_instruction>(std::move(operands));
			case asm_instruction_id::inc:
				return std::make_shared<elang::easm::inc_instruction>(std::move(operands));
			case asm_instruction_id::dec:
				return std::make_shared<elang::easm::dec_instruction>(std::move(operands));
			case asm_instruction_id::not:
				return std::make_shared<elang::easm::not_instruction>(std::move(operands));
			case asm_instruction_id::lea:
				return std::make_shared<elang::easm::lea_instruction>(std::move(operands));
			case asm_instruction_id::mov:
				return std::make_shared<elang::easm::mov_instruction>(std::move(operands));
			case asm_instruction_id::add:
				return std::make_shared<elang::easm::add_instruction>(std::move(operands));
			case asm_instruction_id::sub:
				return std::make_shared<elang::easm::sub_instruction>(std::move(operands));
			case asm_instruction_id::mult:
				return std::make_shared<elang::easm::mult_instruction>(std::move(operands));
			case asm_instruction_id::div:
				return std::make_shared<elang::easm::div_instruction>(std::move(operands));
			case asm_instruction_id::mod:
				return std::make_shared<elang::easm::mod_instruction>(std::move(operands));
			case asm_instruction_id::and_:
				return std::make_shared<elang::easm::and_instruction>(std::move(operands));
			case asm_instruction_id::xor_:
				return std::make_shared<elang::easm::xor_instruction>(std::move(operands));
			case asm_instruction_id::or_:
				return std::make_shared<elang::easm::or_instruction>(std::move(operands));
			case asm_instruction_id::sal:
				return std::make_shared<elang::easm::sal_instruction>(std::move(operands));
			case asm_instruction_id::sar:
				return std::make_shared<elang::easm::sar_instruction>(std::move(operands));
			case asm_instruction_id::test:
				return std::make_shared<elang::easm::test_instruction>(std::move(operands));
			case asm_instruction_id::cmp:
				return std::make_shared<elang::easm::cmp_instruction>(std::move(operands));
			case asm_instruction_id::db:
				return std::make_shared<elang::easm::db_instruction>(std::move(operands));
			case asm_instruction_id::dw:
				return std::make_shared<elang::easm::dw_instruction>(std::move(operands));
			case asm_instruction_id::dd:
				return std::make_shared<elang::easm::dd_instruction>(std::move(operands));
			case asm_instruction_id::dq:
				return std::make_shared<elang::easm::dq_instruction>(std::move(operands));
			case asm_instruction_id::df:
				return std::make_shared<elang::easm::df_instruction>(std::move(operands));
			//case asm_instruction_id::dz:
				//return std::make_shared<elang::easm::dz_instruction>(std::move(operands));
			default:
				break;
			}

			return nullptr;
		}

	private:
		easm::instruction_table &table_;
	};
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

ELANG_AST_ADAPT_SINGLE(asm_stack)
ELANG_AST_ADAPT_SINGLE(asm_global)

ELANG_AST_ADAPT_SINGLE(asm_dzero)
ELANG_AST_ADAPT_SINGLE(asm_dstring)

ELANG_AST_ADAPT_SINGLE(asm_instruction_set_value)
ELANG_AST_ADAPT_SINGLE(asm_instruction_set)

#endif /* !ELANG_ASM_AST_H */
