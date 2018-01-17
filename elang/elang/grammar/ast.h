#pragma once

#ifndef ELANG_AST_H
#define ELANG_AST_H

#include <string>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/adapted/mpl.hpp>

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#define ELANG_AST_JOIN_(x, y) x ## y
#define ELANG_AST_JOIN(x, y) ELANG_AST_JOIN_(x, y)

#define ELANG_AST_NAME(n) ELANG_AST_JOIN(n, _ast)
#define ELANG_AST_QNAME(n) elang::grammar::ELANG_AST_NAME(n)

#define ELANG_AST_WPOS boost::spirit::x3::position_tagged

#define ELANG_AST_DECLARE_SINGLE(name, type)				\
struct ELANG_AST_NAME(name){								\
	typedef type value_type;								\
	type value;												\
};

#define ELANG_AST_DECLARE_SINGLE_WPOS(name, type)			\
struct ELANG_AST_NAME(name) : ELANG_AST_WPOS{				\
	typedef type value_type;								\
	value_type value;										\
};

#define ELANG_AST_DECLARE_SINGLE_VARIANT(name, ...)			\
struct ELANG_AST_NAME(name){								\
	typedef boost::variant<__VA_ARGS__> value_type;			\
	value_type value;										\
};

#define ELANG_AST_DECLARE_SINGLE_VARIANT_WPOS(name, ...)	\
struct ELANG_AST_NAME(name) : ELANG_AST_WPOS{				\
	typedef boost::variant<__VA_ARGS__> value_type;			\
	value_type value;										\
};

#define ELANG_AST_DECLARE_PAIR(name, type1, type2)			\
struct ELANG_AST_NAME(name){								\
	typedef type1 first_value_type;							\
	typedef type2 second_value_type;						\
	type1 first;											\
	type2 second;											\
};

#define ELANG_AST_DECLARE_PAIR_WPOS(name, type1, type2)		\
struct ELANG_AST_NAME(name) : ELANG_AST_WPOS{				\
	typedef type1 first_value_type;							\
	typedef type2 second_value_type;						\
	type1 first;											\
	type2 second;											\
};

#define ELANG_AST_ADAPT_SINGLE(name)						\
BOOST_FUSION_ADAPT_STRUCT(									\
	ELANG_AST_QNAME(name),									\
	(ELANG_AST_QNAME(name)::value_type, value)				\
)

#define ELANG_AST_ADAPT_PAIR(name)							\
BOOST_FUSION_ADAPT_STRUCT(									\
	ELANG_AST_QNAME(name),									\
	(ELANG_AST_QNAME(name)::first_value_type, first)		\
	(ELANG_AST_QNAME(name)::second_value_type, second)		\
)

namespace elang::grammar{
	ELANG_AST_DECLARE_SINGLE(elang_identifier, std::string)
}

ELANG_AST_ADAPT_SINGLE(elang_identifier)

#endif /* !ELANG_AST_H */
