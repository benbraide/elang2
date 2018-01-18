#include "byte_code/byte_code_translator.h"
#include "asm/asm_instruction_translator.h"

int main(){
	const int i = ((1 + 9 - (0 % 8)) / 8);
	const int j = ((4 + 9 - (3 % 8)) / 8);
	const int k = ((7 + 9 - (6 % 8)) / 8);
	const int l = ((8 + 9 - (7 % 8)) / 8);
	const int m = ((9 + 9 - (8 % 8)) / 8);
	const int n = ((11 + 9 - (10 % 8)) / 8);
	const int o = ((16 + 9 - (15 % 8)) / 8);
	const int p = ((20 + 9 - (19 % 8)) / 8);

	std::string source("section .text\nmov rax, 9\nadd [rsp + 45], rax");
	elang::easm::instruction_translator::translate(source.begin(), source.end());

	return 0;
}