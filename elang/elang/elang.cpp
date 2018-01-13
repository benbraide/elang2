#include "byte_code/byte_code_arithmetic_instruction.h"
#include "byte_code/byte_code_call_instruction.h"
#include "byte_code/byte_code_cmp_instruction.h"
#include "byte_code/byte_code_jmp_instruction.h"
#include "byte_code/byte_code_lea_instruction.h"
#include "byte_code/byte_code_mov_instruction.h"
#include "byte_code/byte_code_set_instruction.h"
#include "byte_code/byte_code_stack_instruction.h"
#include "byte_code/byte_code_test_instruction.h"

int main(){
	elang::memory::register_table reg_tbl;

	auto rax = reg_tbl.find("rax");
	auto eax = reg_tbl.find("eax");
	auto mat = rax->match(1);

	auto i64 = reinterpret_cast<__int64 *>(rax->data());
	auto i32 = reinterpret_cast<__int32 *>(eax->data());

	rax->write(72);
	eax->write(54);

	return 0;
}