#include "byte_code_translator.h"

void elang::byte_code::translator::translate_file(const std::string &file, const std::string &dir){
	auto path = common::file_resource::resolve(dir, file, "ebcd");
	if (!boost::filesystem::exists(path))
		throw common::error::file_not_found;

	boost::iostreams::mapped_file source(path, boost::iostreams::mapped_file::readonly);
	if (!source.is_open())
		throw common::error::file_not_found;

	translate(source.const_begin());
}

void elang::byte_code::translator::translate(const char *ptr){
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

	memcpy(&write_protection_start, ptr, sizeof(unsigned __int64));
	ptr += sizeof(unsigned __int64);

	memcpy(&write_protection_end, ptr, sizeof(unsigned __int64));
	ptr += sizeof(unsigned __int64);

	unsigned __int64 start_address;
	memcpy(&start_address, ptr, sizeof(unsigned __int64));
	ptr += sizeof(unsigned __int64);

	memory::table mem_tbl;
	mem_tbl.protect_from_access(memory::table::range_type{});

	auto block = mem_tbl.allocate(module_size);
	mem_tbl.write(block->address, ptr, module_size);

	if (write_protection_end > 0u)//Protect range from writes
		mem_tbl.protect_from_write(memory::table::range_type{ write_protection_start, write_protection_end });

	running_main = true;
	translate(mem_tbl, start_address);
}

void elang::byte_code::translator::translate(memory::table &mem_tbl, unsigned __int64 entry){
	if (running_thread)
		return;//Already running

	if (translator::stack_size == 0u)//Use default size -- 1MB
		translator::stack_size = (1024 * 1024);

	memory::register_table reg_tbl;
	memory::stack stack(mem_tbl, translator::stack_size);

	reg_tbl.instruction_pointer()->write(entry);
	reg_tbl.base_pointer()->write(module_size);
	reg_tbl.stack_pointer()->write(module_size);

	translate_(mem_tbl, reg_tbl, stack);
}

void elang::byte_code::translator::translate_(memory::table &mem_tbl, memory::register_table &reg_tbl, memory::stack &stack){
	running_thread = true;
	translator::stack = &stack;

	while (running_thread && running_main){
		switch (extract_id_(reg_tbl)){
		case id::nop:
			translate_instruction_<id::nop>(mem_tbl,reg_tbl, stack);
			break;
		case id::pushb:
			translate_instruction_<id::pushb>(mem_tbl,reg_tbl, stack);
			break;
		case id::pushw:
			translate_instruction_<id::pushw>(mem_tbl,reg_tbl, stack);
			break;
		case id::pushd:
			translate_instruction_<id::pushd>(mem_tbl,reg_tbl, stack);
			break;
		case id::pushq:
			translate_instruction_<id::pushq>(mem_tbl,reg_tbl, stack);
			break;
		case id::pushf:
			translate_instruction_<id::pushf>(mem_tbl,reg_tbl, stack);
			break;
		case id::popb:
			translate_instruction_<id::popb>(mem_tbl,reg_tbl, stack);
			break;
		case id::popw:
			translate_instruction_<id::popw>(mem_tbl,reg_tbl, stack);
			break;
		case id::popd:
			translate_instruction_<id::popd>(mem_tbl,reg_tbl, stack);
			break;
		case id::popq:
			translate_instruction_<id::popq>(mem_tbl,reg_tbl, stack);
			break;
		case id::popf:
			translate_instruction_<id::popf>(mem_tbl,reg_tbl, stack);
			break;
		case id::movb:
			translate_instruction_<id::movb>(mem_tbl,reg_tbl, stack);
			break;
		case id::movw:
			translate_instruction_<id::movw>(mem_tbl,reg_tbl, stack);
			break;
		case id::movd:
			translate_instruction_<id::movd>(mem_tbl,reg_tbl, stack);
			break;
		case id::movq:
			translate_instruction_<id::movq>(mem_tbl,reg_tbl, stack);
			break;
		case id::movf:
			translate_instruction_<id::movf>(mem_tbl,reg_tbl, stack);
			break;
		case id::cnvtb:
			translate_instruction_<id::cnvtb>(mem_tbl, reg_tbl, stack);
			break;
		case id::cnvtw:
			translate_instruction_<id::cnvtw>(mem_tbl, reg_tbl, stack);
			break;
		case id::cnvtd:
			translate_instruction_<id::cnvtd>(mem_tbl, reg_tbl, stack);
			break;
		case id::cnvtq:
			translate_instruction_<id::cnvtq>(mem_tbl, reg_tbl, stack);
			break;
		case id::cnvtf:
			translate_instruction_<id::cnvtf>(mem_tbl, reg_tbl, stack);
			break;
		case id::leab:
			translate_instruction_<id::leab>(mem_tbl,reg_tbl, stack);
			break;
		case id::leaw:
			translate_instruction_<id::leaw>(mem_tbl,reg_tbl, stack);
			break;
		case id::lead:
			translate_instruction_<id::lead>(mem_tbl,reg_tbl, stack);
			break;
		case id::leaq:
			translate_instruction_<id::leaq>(mem_tbl,reg_tbl, stack);
			break;
		case id::syscall:
			translate_instruction_<id::syscall>(mem_tbl,reg_tbl, stack);
			break;
		case id::callb:
			translate_instruction_<id::callb>(mem_tbl,reg_tbl, stack);
			break;
		case id::callw:
			translate_instruction_<id::callw>(mem_tbl,reg_tbl, stack);
			break;
		case id::calld:
			translate_instruction_<id::calld>(mem_tbl,reg_tbl, stack);
			break;
		case id::callq:
			translate_instruction_<id::callq>(mem_tbl,reg_tbl, stack);
			break;
		case id::enter:
			translate_instruction_<id::enter>(mem_tbl,reg_tbl, stack);
			break;
		case id::leave:
			translate_instruction_<id::leave>(mem_tbl,reg_tbl, stack);
			break;
		case id::ret:
			translate_instruction_<id::ret>(mem_tbl,reg_tbl, stack);
			break;
		case id::jmpb:
			translate_instruction_<id::jmpb>(mem_tbl,reg_tbl, stack);
			break;
		case id::jmpw:
			translate_instruction_<id::jmpw>(mem_tbl,reg_tbl, stack);
			break;
		case id::jmpd:
			translate_instruction_<id::jmpd>(mem_tbl,reg_tbl, stack);
			break;
		case id::jmpq:
			translate_instruction_<id::jmpq>(mem_tbl,reg_tbl, stack);
			break;
		case id::cjmpb:
			translate_instruction_<id::cjmpb>(mem_tbl,reg_tbl, stack);
			break;
		case id::cjmpw:
			translate_instruction_<id::cjmpw>(mem_tbl,reg_tbl, stack);
			break;
		case id::cjmpd:
			translate_instruction_<id::cjmpd>(mem_tbl,reg_tbl, stack);
			break;
		case id::cjmpq:
			translate_instruction_<id::cjmpq>(mem_tbl,reg_tbl, stack);
			break;
		case id::setb:
			translate_instruction_<id::setb>(mem_tbl,reg_tbl, stack);
			break;
		case id::setw:
			translate_instruction_<id::setw>(mem_tbl,reg_tbl, stack);
			break;
		case id::setd:
			translate_instruction_<id::setd>(mem_tbl,reg_tbl, stack);
			break;
		case id::setq:
			translate_instruction_<id::setq>(mem_tbl,reg_tbl, stack);
			break;
		case id::loopb:
			translate_instruction_<id::loopb>(mem_tbl,reg_tbl, stack);
			break;
		case id::loopw:
			translate_instruction_<id::loopw>(mem_tbl,reg_tbl, stack);
			break;
		case id::loopd:
			translate_instruction_<id::loopd>(mem_tbl,reg_tbl, stack);
			break;
		case id::loopq:
			translate_instruction_<id::loopq>(mem_tbl,reg_tbl, stack);
			break;
		case id::incb:
			translate_instruction_<id::incb>(mem_tbl,reg_tbl, stack);
			break;
		case id::incw:
			translate_instruction_<id::incw>(mem_tbl,reg_tbl, stack);
			break;
		case id::incd:
			translate_instruction_<id::incd>(mem_tbl,reg_tbl, stack);
			break;
		case id::incq:
			translate_instruction_<id::incq>(mem_tbl,reg_tbl, stack);
			break;
		case id::decb:
			translate_instruction_<id::decb>(mem_tbl,reg_tbl, stack);
			break;
		case id::decw:
			translate_instruction_<id::decw>(mem_tbl,reg_tbl, stack);
			break;
		case id::decd:
			translate_instruction_<id::decd>(mem_tbl,reg_tbl, stack);
			break;
		case id::decq:
			translate_instruction_<id::decq>(mem_tbl,reg_tbl, stack);
			break;
		case id::addb:
			translate_instruction_<id::addb>(mem_tbl,reg_tbl, stack);
			break;
		case id::addw:
			translate_instruction_<id::addw>(mem_tbl,reg_tbl, stack);
			break;
		case id::addd:
			translate_instruction_<id::addd>(mem_tbl,reg_tbl, stack);
			break;
		case id::addq:
			translate_instruction_<id::addq>(mem_tbl,reg_tbl, stack);
			break;
		case id::addf:
			translate_instruction_<id::addf>(mem_tbl,reg_tbl, stack);
			break;
		case id::subb:
			translate_instruction_<id::subb>(mem_tbl,reg_tbl, stack);
			break;
		case id::subw:
			translate_instruction_<id::subw>(mem_tbl,reg_tbl, stack);
			break;
		case id::subd:
			translate_instruction_<id::subd>(mem_tbl,reg_tbl, stack);
			break;
		case id::subq:
			translate_instruction_<id::subq>(mem_tbl,reg_tbl, stack);
			break;
		case id::subf:
			translate_instruction_<id::subf>(mem_tbl,reg_tbl, stack);
			break;
		case id::multb:
			translate_instruction_<id::multb>(mem_tbl,reg_tbl, stack);
			break;
		case id::multw:
			translate_instruction_<id::multw>(mem_tbl,reg_tbl, stack);
			break;
		case id::multd:
			translate_instruction_<id::multd>(mem_tbl,reg_tbl, stack);
			break;
		case id::multq:
			translate_instruction_<id::multq>(mem_tbl,reg_tbl, stack);
			break;
		case id::multf:
			translate_instruction_<id::multf>(mem_tbl,reg_tbl, stack);
			break;
		case id::divb:
			translate_instruction_<id::divb>(mem_tbl,reg_tbl, stack);
			break;
		case id::divw:
			translate_instruction_<id::divw>(mem_tbl,reg_tbl, stack);
			break;
		case id::divd:
			translate_instruction_<id::divd>(mem_tbl,reg_tbl, stack);
			break;
		case id::divq:
			translate_instruction_<id::divq>(mem_tbl,reg_tbl, stack);
			break;
		case id::divf:
			translate_instruction_<id::divf>(mem_tbl,reg_tbl, stack);
			break;
		case id::modb:
			translate_instruction_<id::modb>(mem_tbl,reg_tbl, stack);
			break;
		case id::modw:
			translate_instruction_<id::modw>(mem_tbl,reg_tbl, stack);
			break;
		case id::modd:
			translate_instruction_<id::modd>(mem_tbl,reg_tbl, stack);
			break;
		case id::modq:
			translate_instruction_<id::modq>(mem_tbl,reg_tbl, stack);
			break;
		case id::andb:
			translate_instruction_<id::andb>(mem_tbl,reg_tbl, stack);
			break;
		case id::andw:
			translate_instruction_<id::andw>(mem_tbl,reg_tbl, stack);
			break;
		case id::andd:
			translate_instruction_<id::andd>(mem_tbl,reg_tbl, stack);
			break;
		case id::andq:
			translate_instruction_<id::andq>(mem_tbl,reg_tbl, stack);
			break;
		case id::xorb:
			translate_instruction_<id::xorb>(mem_tbl,reg_tbl, stack);
			break;
		case id::xorw:
			translate_instruction_<id::xorw>(mem_tbl,reg_tbl, stack);
			break;
		case id::xord:
			translate_instruction_<id::xord>(mem_tbl,reg_tbl, stack);
			break;
		case id::xorq:
			translate_instruction_<id::xorq>(mem_tbl,reg_tbl, stack);
			break;
		case id::orb:
			translate_instruction_<id::orb>(mem_tbl,reg_tbl, stack);
			break;
		case id::orw:
			translate_instruction_<id::orw>(mem_tbl,reg_tbl, stack);
			break;
		case id::ord:
			translate_instruction_<id::ord>(mem_tbl,reg_tbl, stack);
			break;
		case id::orq:
			translate_instruction_<id::orq>(mem_tbl,reg_tbl, stack);
			break;
		case id::salb:
			translate_instruction_<id::salb>(mem_tbl,reg_tbl, stack);
			break;
		case id::salw:
			translate_instruction_<id::salw>(mem_tbl,reg_tbl, stack);
			break;
		case id::sald:
			translate_instruction_<id::sald>(mem_tbl,reg_tbl, stack);
			break;
		case id::salq:
			translate_instruction_<id::salq>(mem_tbl,reg_tbl, stack);
			break;
		case id::sarb:
			translate_instruction_<id::sarb>(mem_tbl,reg_tbl, stack);
			break;
		case id::sarw:
			translate_instruction_<id::sarw>(mem_tbl,reg_tbl, stack);
			break;
		case id::sard:
			translate_instruction_<id::sard>(mem_tbl,reg_tbl, stack);
			break;
		case id::sarq:
			translate_instruction_<id::sarq>(mem_tbl,reg_tbl, stack);
			break;
		case id::testb:
			translate_instruction_<id::testb>(mem_tbl,reg_tbl, stack);
			break;
		case id::testw:
			translate_instruction_<id::testw>(mem_tbl,reg_tbl, stack);
			break;
		case id::testd:
			translate_instruction_<id::testd>(mem_tbl,reg_tbl, stack);
			break;
		case id::testq:
			translate_instruction_<id::testq>(mem_tbl,reg_tbl, stack);
			break;
		case id::notb:
			translate_instruction_<id::notb>(mem_tbl,reg_tbl, stack);
			break;
		case id::notw:
			translate_instruction_<id::notw>(mem_tbl,reg_tbl, stack);
			break;
		case id::notd:
			translate_instruction_<id::notd>(mem_tbl,reg_tbl, stack);
			break;
		case id::notq:
			translate_instruction_<id::notq>(mem_tbl,reg_tbl, stack);
			break;
		case id::cmpb:
			translate_instruction_<id::cmpb>(mem_tbl,reg_tbl, stack);
			break;
		case id::cmpw:
			translate_instruction_<id::cmpw>(mem_tbl,reg_tbl, stack);
			break;
		case id::cmpd:
			translate_instruction_<id::cmpd>(mem_tbl,reg_tbl, stack);
			break;
		case id::cmpq:
			translate_instruction_<id::cmpq>(mem_tbl,reg_tbl, stack);
			break;
		case id::cmpf:
			translate_instruction_<id::cmpf>(mem_tbl,reg_tbl, stack);
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

std::size_t elang::byte_code::translator::stack_size = (1024 * 1024);//1MB default

unsigned __int64 elang::byte_code::translator::write_protection_start;

unsigned __int64 elang::byte_code::translator::write_protection_end;

thread_local elang::memory::stack *elang::byte_code::translator::stack = nullptr;
