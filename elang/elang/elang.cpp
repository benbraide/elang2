#include "byte_code/byte_code_translator.h"

#include "grammar/asm/asm_grammar.h"

#define ELANG_SYNCHK(n)\
ELANG_AST_QNAME(n) n;\
\
boost::spirit::x3::phrase_parse(\
	source.begin(),\
	source.end(),\
	ELANG_GRAMMAR_RULE_QNAME(n),\
	ELANG_GRAMMAR_RULE_QNAME(asm_skip),\
	n\
);

int main(){
	std::string source("8test_string");

	ELANG_SYNCHK(elang_identifier);
	ELANG_SYNCHK(asm_string);
	ELANG_SYNCHK(asm_section);
	ELANG_SYNCHK(asm_label);
	ELANG_SYNCHK(asm_offset_item);
	ELANG_SYNCHK(asm_offset);
	ELANG_SYNCHK(asm_offset_explicit);
	ELANG_SYNCHK(asm_memory);
	ELANG_SYNCHK(asm_operand);
	ELANG_SYNCHK(asm_instruction);
	ELANG_SYNCHK(asm_times_instruction);
	ELANG_SYNCHK(asm_instruction_set_value);
	ELANG_SYNCHK(asm_instruction_set);

	return 0;
}