#pragma once

#ifndef ELANG_LANG_LITERAL_VALUE_GRAMMAR_H
#define ELANG_LANG_LITERAL_VALUE_GRAMMAR_H

#include "../grammar.h"

#include "lang_literal_value_ast.h"

namespace elang::grammar{
	struct lang_integer_literal_suffix_symbols : x3::symbols<lang_integer_literal_suffix>{
		lang_integer_literal_suffix_symbols(){
			add
				("i8", lang_integer_literal_suffix::int8)
				("ui8", lang_integer_literal_suffix::uint8)
				("i16", lang_integer_literal_suffix::int16)
				("ui16", lang_integer_literal_suffix::uint16)
				("i32", lang_integer_literal_suffix::int32)
				("ui32", lang_integer_literal_suffix::uint32)
				("i64", lang_integer_literal_suffix::int64)
				("ui64", lang_integer_literal_suffix::uint64)
				("u", lang_integer_literal_suffix::uint32)
				;
		}
	} lang_integer_literal_suffix_symbols_;

	ELANG_GRAMMAR_DECLARE_RULE(lang_float_literal_value)
	ELANG_GRAMMAR_RULE_DEF(lang_float_literal_value) = elang_long_double;

	elang_hex_parser<__int64> const lang_long_long_hex = {};
	elang_oct_parser<__int64> const lang_long_long_oct = {};
	elang_bin_parser<__int64> const lang_long_long_bin = {};

	ELANG_GRAMMAR_DECLARE_RULE(lang_integer_literal_value)
	ELANG_GRAMMAR_RULE_DEF(lang_integer_literal_value) = x3::lexeme[(
		x3::lexeme["0x" >> lang_long_long_hex] |
		x3::lexeme["0b" >> lang_long_long_bin] |
		x3::lexeme["0" >> lang_long_long_oct] |
		x3::long_long
	) >> -x3::no_case[lang_integer_literal_suffix_symbols_]];

	ELANG_GRAMMAR_DECLARE_RULE(lang_char_literal_value)
	ELANG_GRAMMAR_RULE_DEF(lang_char_literal_value) = x3::lexeme[-x3::char_('L') >> "'" > *((x3::char_('\\') >> "'") | ~x3::char_("'")) > "'"];

	ELANG_GRAMMAR_DECLARE_RULE(lang_string_literal_value)
	ELANG_GRAMMAR_RULE_DEF(lang_string_literal_value) = x3::lexeme[-x3::char_('L') >> '"' > *((x3::char_('\\') >> '"') | ~x3::char_('"')) > '"'];

	ELANG_GRAMMAR_DECLARE_RULE(lang_non_escaped_char_literal_value)
	ELANG_GRAMMAR_RULE_DEF(lang_non_escaped_char_literal_value) = x3::lexeme['@' > -x3::char_('L') >> "'" > *(~x3::char_("'")) > "'"];

	ELANG_GRAMMAR_DECLARE_RULE(lang_non_escaped_string_literal_value)
	ELANG_GRAMMAR_RULE_DEF(lang_non_escaped_string_literal_value) = x3::lexeme['@' > -x3::char_('L') >> '"' > *(~x3::char_('"')) > '"'];

	ELANG_GRAMMAR_DECLARE_RULE(lang_literal_value)
	ELANG_GRAMMAR_RULE_DEF(lang_literal_value) = (
		ELANG_GRAMMAR_RULE_NAME(lang_float_literal_value) |
		ELANG_GRAMMAR_RULE_NAME(lang_integer_literal_value) |
		ELANG_GRAMMAR_RULE_NAME(lang_char_literal_value) |
		ELANG_GRAMMAR_RULE_NAME(lang_string_literal_value) |
		ELANG_GRAMMAR_RULE_NAME(lang_non_escaped_char_literal_value) |
		ELANG_GRAMMAR_RULE_NAME(lang_non_escaped_string_literal_value)
	);

	BOOST_SPIRIT_DEFINE(
		ELANG_GRAMMAR_RULE_NAME(lang_integer_literal_value),
		ELANG_GRAMMAR_RULE_NAME(lang_float_literal_value),
		ELANG_GRAMMAR_RULE_NAME(lang_char_literal_value),
		ELANG_GRAMMAR_RULE_NAME(lang_string_literal_value),
		ELANG_GRAMMAR_RULE_NAME(lang_non_escaped_char_literal_value),
		ELANG_GRAMMAR_RULE_NAME(lang_non_escaped_string_literal_value),
		ELANG_GRAMMAR_RULE_NAME(lang_literal_value)
	)
}

#endif /* !ELANG_LANG_LITERAL_VALUE_GRAMMAR_H */
