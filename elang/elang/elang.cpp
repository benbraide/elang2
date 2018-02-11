#include <iostream>

#include "byte_code/byte_code_translator.h"
#include "asm/asm_instruction_translator.h"
#include "common/string_output_writer.h"

#include "grammar/lang/lang_literal_value_grammar.h"

#define ELANG_CHECK_GRAMMAR(n, b)\
ELANG_AST_QNAME(n) n ## out;\
boost::spirit::x3::phrase_parse(\
	b.begin(),\
	b.end(),\
	ELANG_GRAMMAR_RULE_QNAME(n),\
	boost::spirit::x3::space,\
	n ## out\
);

int main(){
	std::string buf;

	ELANG_CHECK_GRAMMAR(lang_char_literal_value, buf);
	ELANG_CHECK_GRAMMAR(lang_string_literal_value, buf);
	ELANG_CHECK_GRAMMAR(lang_non_escaped_char_literal_value, buf);
	ELANG_CHECK_GRAMMAR(lang_non_escaped_string_literal_value, buf);

	std::wstring log;
	elang::common::string_output_writer writer(log);
	elang::byte_code::debug dbg(writer);

	std::string source(".global main\nsection .text\nmain:\nmov rax, 9\nadd [rsp + 45], al\nmov ax, 0\nsyscall");

	elang::easm::instruction_translator::encode_file("basic", "basic");
	elang::byte_code::translator::translate_file("basic");

	return 0;
}