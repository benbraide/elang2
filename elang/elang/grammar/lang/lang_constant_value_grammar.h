#pragma once

#ifndef ELANG_LANG_CONSTANT_VALUE_GRAMMAR_H
#define ELANG_LANG_CONSTANT_VALUE_GRAMMAR_H

#include "../grammar.h"

#include "lang_constant_value_ast.h"

namespace elang::grammar{
	struct lang_constant_value_symbols : x3::symbols<common::constant_value>{
		lang_constant_value_symbols(){
			add
				("false", common::constant_value::false_)
				("true", common::constant_value::true_)
				("#ind", common::constant_value::indeterminate)
				("nullptr", common::constant_value::nullptr_)
				;
		}
	} lang_constant_value_symbols_;

	ELANG_GRAMMAR_DECLARE_RULE(lang_constant_value)
	ELANG_GRAMMAR_RULE_DEF(lang_constant_value) = utils::keyword(lang_constant_value_symbols_);

	BOOST_SPIRIT_DEFINE(
		ELANG_GRAMMAR_RULE_NAME(lang_constant_value)
	)
}

#endif /* !ELANG_LANG_CONSTANT_VALUE_GRAMMAR_H */
