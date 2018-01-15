#include "byte_code_translator.h"

void elang::byte_code::translator::translate_file(const std::string &path){
	
}

void elang::byte_code::translator::translate(char *ptr){
	if (running_main)
		return;//Already running

	memcpy(&module_size, ptr, sizeof(std::size_t));
	ptr += sizeof(std::size_t);

	std::size_t stack_size;
	memcpy(&stack_size, ptr, sizeof(std::size_t));
	ptr += sizeof(std::size_t);

	if (stack_size != 0u)
		translator::stack_size = stack_size;
	else if (translator::stack_size == 0u)//Use default size -- 1MB
		translator::stack_size = (1024 * 1024);

	memcpy(&write_protection_start, ptr, sizeof(__int64));
	ptr += sizeof(__int64);

	memcpy(&write_protection_end, ptr, sizeof(__int64));
	ptr += sizeof(__int64);

	__int64 start_address;
	memcpy(&start_address, ptr, sizeof(__int64));
	ptr += sizeof(__int64);

	running_main = true;
	translate(ptr, start_address);
}

void elang::byte_code::translator::translate(char *base_ptr, __int64 entry){
	if (running_thread)
		return;//Already running

	if (translator::stack_size == 0u)//Use default size -- 1MB
		translator::stack_size = (1024 * 1024);

	memory::register_table reg_tbl;
	memory::stack stack(translator::stack_size);

	reg_tbl.instruction_pointer()->write(entry);
	reg_tbl.base_pointer()->write(module_size);
	reg_tbl.stack_pointer()->write(module_size);

	translate_(base_ptr, reg_tbl, stack);
}

void elang::byte_code::translator::translate_(char *base_ptr, memory::register_table &reg_tbl, memory::stack &stack){
	running_thread = true;
	translator::stack = &stack;

	while (running_thread && running_main){
		switch (extract_id_(reg_tbl)){
		case id::nop:
			translate_instruction_<id::nop>(base_ptr, reg_tbl, stack);
			break;
		case id::pushb:
			translate_instruction_<id::pushb>(base_ptr, reg_tbl, stack);
			break;
		case id::pushw:
			translate_instruction_<id::pushw>(base_ptr, reg_tbl, stack);
			break;
		case id::pushd:
			translate_instruction_<id::pushd>(base_ptr, reg_tbl, stack);
			break;
		case id::pushq:
			translate_instruction_<id::pushq>(base_ptr, reg_tbl, stack);
			break;
		case id::pushf:
			translate_instruction_<id::pushf>(base_ptr, reg_tbl, stack);
			break;
		case id::popb:
			translate_instruction_<id::popb>(base_ptr, reg_tbl, stack);
			break;
		case id::popw:
			translate_instruction_<id::popw>(base_ptr, reg_tbl, stack);
			break;
		case id::popd:
			translate_instruction_<id::popd>(base_ptr, reg_tbl, stack);
			break;
		case id::popq:
			translate_instruction_<id::popq>(base_ptr, reg_tbl, stack);
			break;
		case id::popf:
			translate_instruction_<id::popf>(base_ptr, reg_tbl, stack);
			break;
		case id::movb:
			translate_instruction_<id::movb>(base_ptr, reg_tbl, stack);
			break;
		case id::movw:
			translate_instruction_<id::movw>(base_ptr, reg_tbl, stack);
			break;
		case id::movd:
			translate_instruction_<id::movd>(base_ptr, reg_tbl, stack);
			break;
		case id::movq:
			translate_instruction_<id::movq>(base_ptr, reg_tbl, stack);
			break;
		case id::movf:
			translate_instruction_<id::movf>(base_ptr, reg_tbl, stack);
			break;
		case id::leab:
			translate_instruction_<id::leab>(base_ptr, reg_tbl, stack);
			break;
		case id::leaw:
			translate_instruction_<id::leaw>(base_ptr, reg_tbl, stack);
			break;
		case id::lead:
			translate_instruction_<id::lead>(base_ptr, reg_tbl, stack);
			break;
		case id::leaq:
			translate_instruction_<id::leaq>(base_ptr, reg_tbl, stack);
			break;
		case id::syscall:
			translate_instruction_<id::syscall>(base_ptr, reg_tbl, stack);
			break;
		case id::callb:
			translate_instruction_<id::callb>(base_ptr, reg_tbl, stack);
			break;
		case id::callw:
			translate_instruction_<id::callw>(base_ptr, reg_tbl, stack);
			break;
		case id::calld:
			translate_instruction_<id::calld>(base_ptr, reg_tbl, stack);
			break;
		case id::callq:
			translate_instruction_<id::callq>(base_ptr, reg_tbl, stack);
			break;
		case id::enter:
			translate_instruction_<id::enter>(base_ptr, reg_tbl, stack);
			break;
		case id::leave:
			translate_instruction_<id::leave>(base_ptr, reg_tbl, stack);
			break;
		case id::ret:
			translate_instruction_<id::ret>(base_ptr, reg_tbl, stack);
			break;
		case id::jmpb:
			translate_instruction_<id::jmpb>(base_ptr, reg_tbl, stack);
			break;
		case id::jmpw:
			translate_instruction_<id::jmpw>(base_ptr, reg_tbl, stack);
			break;
		case id::jmpd:
			translate_instruction_<id::jmpd>(base_ptr, reg_tbl, stack);
			break;
		case id::jmpq:
			translate_instruction_<id::jmpq>(base_ptr, reg_tbl, stack);
			break;
		case id::cjmpb:
			translate_instruction_<id::cjmpb>(base_ptr, reg_tbl, stack);
			break;
		case id::cjmpw:
			translate_instruction_<id::cjmpw>(base_ptr, reg_tbl, stack);
			break;
		case id::cjmpd:
			translate_instruction_<id::cjmpd>(base_ptr, reg_tbl, stack);
			break;
		case id::cjmpq:
			translate_instruction_<id::cjmpq>(base_ptr, reg_tbl, stack);
			break;
		case id::setb:
			translate_instruction_<id::setb>(base_ptr, reg_tbl, stack);
			break;
		case id::setw:
			translate_instruction_<id::setw>(base_ptr, reg_tbl, stack);
			break;
		case id::setd:
			translate_instruction_<id::setd>(base_ptr, reg_tbl, stack);
			break;
		case id::setq:
			translate_instruction_<id::setq>(base_ptr, reg_tbl, stack);
			break;
		case id::loopb:
			translate_instruction_<id::loopb>(base_ptr, reg_tbl, stack);
			break;
		case id::loopw:
			translate_instruction_<id::loopw>(base_ptr, reg_tbl, stack);
			break;
		case id::loopd:
			translate_instruction_<id::loopd>(base_ptr, reg_tbl, stack);
			break;
		case id::loopq:
			translate_instruction_<id::loopq>(base_ptr, reg_tbl, stack);
			break;
		case id::incb:
			translate_instruction_<id::incb>(base_ptr, reg_tbl, stack);
			break;
		case id::incw:
			translate_instruction_<id::incw>(base_ptr, reg_tbl, stack);
			break;
		case id::incd:
			translate_instruction_<id::incd>(base_ptr, reg_tbl, stack);
			break;
		case id::incq:
			translate_instruction_<id::incq>(base_ptr, reg_tbl, stack);
			break;
		case id::decb:
			translate_instruction_<id::decb>(base_ptr, reg_tbl, stack);
			break;
		case id::decw:
			translate_instruction_<id::decw>(base_ptr, reg_tbl, stack);
			break;
		case id::decd:
			translate_instruction_<id::decd>(base_ptr, reg_tbl, stack);
			break;
		case id::decq:
			translate_instruction_<id::decq>(base_ptr, reg_tbl, stack);
			break;
		case id::addb:
			translate_instruction_<id::addb>(base_ptr, reg_tbl, stack);
			break;
		case id::addw:
			translate_instruction_<id::addw>(base_ptr, reg_tbl, stack);
			break;
		case id::addd:
			translate_instruction_<id::addd>(base_ptr, reg_tbl, stack);
			break;
		case id::addq:
			translate_instruction_<id::addq>(base_ptr, reg_tbl, stack);
			break;
		case id::addf:
			translate_instruction_<id::addf>(base_ptr, reg_tbl, stack);
			break;
		case id::subb:
			translate_instruction_<id::subb>(base_ptr, reg_tbl, stack);
			break;
		case id::subw:
			translate_instruction_<id::subw>(base_ptr, reg_tbl, stack);
			break;
		case id::subd:
			translate_instruction_<id::subd>(base_ptr, reg_tbl, stack);
			break;
		case id::subq:
			translate_instruction_<id::subq>(base_ptr, reg_tbl, stack);
			break;
		case id::subf:
			translate_instruction_<id::subf>(base_ptr, reg_tbl, stack);
			break;
		case id::multb:
			translate_instruction_<id::multb>(base_ptr, reg_tbl, stack);
			break;
		case id::multw:
			translate_instruction_<id::multw>(base_ptr, reg_tbl, stack);
			break;
		case id::multd:
			translate_instruction_<id::multd>(base_ptr, reg_tbl, stack);
			break;
		case id::multq:
			translate_instruction_<id::multq>(base_ptr, reg_tbl, stack);
			break;
		case id::multf:
			translate_instruction_<id::multf>(base_ptr, reg_tbl, stack);
			break;
		case id::divb:
			translate_instruction_<id::divb>(base_ptr, reg_tbl, stack);
			break;
		case id::divw:
			translate_instruction_<id::divw>(base_ptr, reg_tbl, stack);
			break;
		case id::divd:
			translate_instruction_<id::divd>(base_ptr, reg_tbl, stack);
			break;
		case id::divq:
			translate_instruction_<id::divq>(base_ptr, reg_tbl, stack);
			break;
		case id::divf:
			translate_instruction_<id::divf>(base_ptr, reg_tbl, stack);
			break;
		case id::modb:
			translate_instruction_<id::modb>(base_ptr, reg_tbl, stack);
			break;
		case id::modw:
			translate_instruction_<id::modw>(base_ptr, reg_tbl, stack);
			break;
		case id::modd:
			translate_instruction_<id::modd>(base_ptr, reg_tbl, stack);
			break;
		case id::modq:
			translate_instruction_<id::modq>(base_ptr, reg_tbl, stack);
			break;
		case id::andb:
			translate_instruction_<id::andb>(base_ptr, reg_tbl, stack);
			break;
		case id::andw:
			translate_instruction_<id::andw>(base_ptr, reg_tbl, stack);
			break;
		case id::andd:
			translate_instruction_<id::andd>(base_ptr, reg_tbl, stack);
			break;
		case id::andq:
			translate_instruction_<id::andq>(base_ptr, reg_tbl, stack);
			break;
		case id::xorb:
			translate_instruction_<id::xorb>(base_ptr, reg_tbl, stack);
			break;
		case id::xorw:
			translate_instruction_<id::xorw>(base_ptr, reg_tbl, stack);
			break;
		case id::xord:
			translate_instruction_<id::xord>(base_ptr, reg_tbl, stack);
			break;
		case id::xorq:
			translate_instruction_<id::xorq>(base_ptr, reg_tbl, stack);
			break;
		case id::orb:
			translate_instruction_<id::orb>(base_ptr, reg_tbl, stack);
			break;
		case id::orw:
			translate_instruction_<id::orw>(base_ptr, reg_tbl, stack);
			break;
		case id::ord:
			translate_instruction_<id::ord>(base_ptr, reg_tbl, stack);
			break;
		case id::orq:
			translate_instruction_<id::orq>(base_ptr, reg_tbl, stack);
			break;
		case id::salb:
			translate_instruction_<id::salb>(base_ptr, reg_tbl, stack);
			break;
		case id::salw:
			translate_instruction_<id::salw>(base_ptr, reg_tbl, stack);
			break;
		case id::sald:
			translate_instruction_<id::sald>(base_ptr, reg_tbl, stack);
			break;
		case id::salq:
			translate_instruction_<id::salq>(base_ptr, reg_tbl, stack);
			break;
		case id::sarb:
			translate_instruction_<id::sarb>(base_ptr, reg_tbl, stack);
			break;
		case id::sarw:
			translate_instruction_<id::sarw>(base_ptr, reg_tbl, stack);
			break;
		case id::sard:
			translate_instruction_<id::sard>(base_ptr, reg_tbl, stack);
			break;
		case id::sarq:
			translate_instruction_<id::sarq>(base_ptr, reg_tbl, stack);
			break;
		case id::testb:
			translate_instruction_<id::testb>(base_ptr, reg_tbl, stack);
			break;
		case id::testw:
			translate_instruction_<id::testw>(base_ptr, reg_tbl, stack);
			break;
		case id::testd:
			translate_instruction_<id::testd>(base_ptr, reg_tbl, stack);
			break;
		case id::testq:
			translate_instruction_<id::testq>(base_ptr, reg_tbl, stack);
			break;
		case id::notb:
			translate_instruction_<id::notb>(base_ptr, reg_tbl, stack);
			break;
		case id::notw:
			translate_instruction_<id::notw>(base_ptr, reg_tbl, stack);
			break;
		case id::notd:
			translate_instruction_<id::notd>(base_ptr, reg_tbl, stack);
			break;
		case id::notq:
			translate_instruction_<id::notq>(base_ptr, reg_tbl, stack);
			break;
		case id::cmpb:
			translate_instruction_<id::cmpb>(base_ptr, reg_tbl, stack);
			break;
		case id::cmpw:
			translate_instruction_<id::cmpw>(base_ptr, reg_tbl, stack);
			break;
		case id::cmpd:
			translate_instruction_<id::cmpd>(base_ptr, reg_tbl, stack);
			break;
		case id::cmpq:
			translate_instruction_<id::cmpq>(base_ptr, reg_tbl, stack);
			break;
		case id::cmpf:
			translate_instruction_<id::cmpf>(base_ptr, reg_tbl, stack);
			break;
		default:
			throw common::error::byte_code_unknown_instruction;
			break;
		}
	}

	translator::stack = nullptr;
	running_thread = false;
}

elang::byte_code::id elang::byte_code::translator::extract_id_(memory::register_table &reg_tbl){
	auto ptr = reinterpret_cast<char *>(reg_tbl.instruction_pointer()->read<__int64>());
	reg_tbl.instruction_pointer()->write(reinterpret_cast<__int64>(ptr + 1));//Advance pointer
	return static_cast<id>(*(ptr));
}

bool elang::byte_code::translator::running_main = false;

thread_local bool elang::byte_code::translator::running_thread = false;

std::size_t elang::byte_code::translator::module_size;

std::size_t elang::byte_code::translator::stack_size = (1024 * 1024);//IMB default

__int64 elang::byte_code::translator::write_protection_start;

__int64 elang::byte_code::translator::write_protection_end;

thread_local elang::memory::stack *elang::byte_code::translator::stack = nullptr;
