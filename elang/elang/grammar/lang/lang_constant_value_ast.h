#pragma once

#ifndef ELANG_LANG_CONSTANT_VALUE_AST_H
#define ELANG_LANG_CONSTANT_VALUE_AST_H

#include "../../lang/lang_type_store.h"
#include "../../lang/lang_operand_info.h"

#include "../ast.h"

namespace elang::grammar{
	ELANG_AST_DECLARE_SINGLE(lang_constant_value, common::constant_value)

	struct lang_constant_value_ast_visitor{
		std::shared_ptr<lang::operand_info> operator ()(ELANG_AST_NAME(lang_constant_value) &ast) const{
			auto value = std::make_shared<lang::operand_info>();
			if (ast.value == common::constant_value::nullptr_)
				value->type = lang::type_store::nullptr_type;
			else//Boolean
				value->type = lang::type_store::bool_type;

			value->is_primitive_constant = true;
			value->value = ast.value;

			return value;
		}
	};
}

ELANG_AST_ADAPT_SINGLE(lang_constant_value)

#endif /* !ELANG_LANG_CONSTANT_VALUE_AST_H */
