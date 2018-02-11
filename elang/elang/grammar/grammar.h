#pragma once

#ifndef ELANG_GRAMMAR_H
#define ELANG_GRAMMAR_H

#include "ast.h"

#define ELANG_GRAMMAR_RULE_NAME(n) n
#define ELANG_GRAMMAR_RULE_NAME_STR(n) #n
#define ELANG_GRAMMAR_RULE_QNAME(n) elang::grammar::ELANG_GRAMMAR_RULE_NAME(n)

#define ELANG_GRAMMAR_DECLARE_RULE(name)\
boost::spirit::x3::rule<class ELANG_GRAMMAR_RULE_NAME(name), ELANG_AST_QNAME(name)> const ELANG_GRAMMAR_RULE_NAME(name) = ELANG_GRAMMAR_RULE_NAME_STR(name);

#define ELANG_GRAMMAR_DECLARE_RULE2(name, ast)\
boost::spirit::x3::rule<class ELANG_GRAMMAR_RULE_NAME(name), ELANG_AST_QNAME(ast)> const ELANG_GRAMMAR_RULE_NAME(name) = ELANG_GRAMMAR_RULE_NAME_STR(name);

#define ELANG_GRAMMAR_DECLARE_NO_AST_RULE(name)\
boost::spirit::x3::rule<class ELANG_GRAMMAR_RULE_NAME(name)> const ELANG_GRAMMAR_RULE_NAME(name) = ELANG_GRAMMAR_RULE_NAME_STR(name);

#define ELANG_GRAMMAR_RULE_DEF(name) auto const ELANG_AST_JOIN(ELANG_GRAMMAR_RULE_NAME(name), _def)

namespace elang::grammar{
	namespace x3 = boost::spirit::x3;

	template <typename value_type>
	struct elang_real_policies : x3::real_policies<value_type>{
		static bool const expect_dot = true;
		static bool const allow_leading_dot = true;
		static bool const allow_trailing_dot = true;
	};

	using float_parser = x3::real_parser<float, elang_real_policies<float>>;
	using double_parser = x3::real_parser<double, elang_real_policies<double>>;
	using long_double_parser = x3::real_parser<long double, elang_real_policies<long double>>;

	float_parser const elang_float = {};
	double_parser const elang_double = {};
	long_double_parser const elang_long_double = {};

	float_parser const elang_f32 = {};
	double_parser const elang_f64 = {};
	long_double_parser const elang_f128 = {};

	template <typename target_type>
	using elang_hex_parser = x3::uint_parser<target_type, 16>;

	template <typename target_type>
	using elang_oct_parser = x3::uint_parser<target_type, 8>;

	template <typename target_type>
	using elang_bin_parser = x3::uint_parser<target_type, 2>;

	ELANG_GRAMMAR_DECLARE_RULE(elang_identifier);
	ELANG_GRAMMAR_RULE_DEF(elang_identifier) = (&x3::char_("$_A-Za-z") >> x3::lexeme[+x3::char_("$_A-Za-z0-9")]);

	class utils{
	public:
		template <typename rule_type>
		static auto keyword(rule_type &&rule){
			return x3::lexeme[std::forward<rule_type>(rule) >> !x3::char_("$_A-Za-z0-9")];
		};

		static auto keyword(const char *value){
			return keyword(x3::lit(value));
		}
	};

	BOOST_SPIRIT_DEFINE(ELANG_GRAMMAR_RULE_NAME(elang_identifier));
}

#endif /* !ELANG_GRAMMAR_H */
