#include "byte_code_debug.h"

elang::byte_code::debug::debug(common::output_writer &writer)
	: writer_(writer), op_count_(0), first_instruction_(true){
	debugger = this;
}

void elang::byte_code::debug::log(id id){
	op_count_ = 0;
	if (first_instruction_)
		first_instruction_ = false;
	else//Add line
		writer_.write("\n");

	writer_.write(convert_id(id));
	writer_.write(" ");
}

void elang::byte_code::debug::log(const std::string &str){
	writer_.write(str);
}

void elang::byte_code::debug::log_operand(const std::string &str){
	if (op_count_++ > 0u)
		writer_.write(", ");
	writer_.write(str);
}

std::string elang::byte_code::debug::convert_id(id id){
	switch (id){
	case byte_code::id::nop: return "nop";
	case byte_code::id::pushb: return "pushb";
	case byte_code::id::pushw: return "pushw";
	case byte_code::id::pushd: return "pushd";
	case byte_code::id::pushq: return "pushq";
	case byte_code::id::pushf: return "pushf";
	case byte_code::id::popb: return "popb";
	case byte_code::id::popw: return "popw";
	case byte_code::id::popd: return "popd";
	case byte_code::id::popq: return "popq";
	case byte_code::id::popf: return "popf";
	case byte_code::id::movb: return "movb";
	case byte_code::id::movw: return "movw";
	case byte_code::id::movd: return "movd";
	case byte_code::id::movq: return "movq";
	case byte_code::id::movf: return "movf";
	case byte_code::id::cnvtb: return "cnvtb";
	case byte_code::id::cnvtw: return "cnvtw";
	case byte_code::id::cnvtd: return "cnvtd";
	case byte_code::id::cnvtq: return "cnvtq";
	case byte_code::id::cnvtf: return "cnvtf";
	case byte_code::id::leab: return "leab";
	case byte_code::id::leaw: return "leaw";
	case byte_code::id::lead: return "lead";
	case byte_code::id::leaq: return "leaq";
	case byte_code::id::syscall: return "syscall";
	case byte_code::id::callb: return "callb";
	case byte_code::id::callw: return "callw";
	case byte_code::id::calld: return "calld";
	case byte_code::id::callq: return "callq";
	case byte_code::id::enter: return "enter";
	case byte_code::id::leave: return "leave";
	case byte_code::id::ret: return "ret";
	case byte_code::id::jmpb: return "jmpb";
	case byte_code::id::jmpw: return "jmpw";
	case byte_code::id::jmpd: return "jmpd";
	case byte_code::id::jmpq: return "jmpq";
	case byte_code::id::cjmpb: return "cjmpb";
	case byte_code::id::cjmpw: return "cjmpw";
	case byte_code::id::cjmpd: return "cjmpd";
	case byte_code::id::cjmpq: return "cjmpq";
	case byte_code::id::setb: return "setb";
	case byte_code::id::setw: return "setw";
	case byte_code::id::setd: return "setd";
	case byte_code::id::setq: return "setq";
	case byte_code::id::loopb: return "loopb";
	case byte_code::id::loopw: return "loopw";
	case byte_code::id::loopd: return "loopd";
	case byte_code::id::loopq: return "loopq";
	case byte_code::id::incb: return "incb";
	case byte_code::id::incw: return "incw";
	case byte_code::id::incd: return "incd";
	case byte_code::id::incq: return "incq";
	case byte_code::id::decb: return "decb";
	case byte_code::id::decw: return "decw";
	case byte_code::id::decd: return "decd";
	case byte_code::id::decq: return "decq";
	case byte_code::id::addb: return "addb";
	case byte_code::id::addw: return "addw";
	case byte_code::id::addd: return "addd";
	case byte_code::id::addq: return "addq";
	case byte_code::id::addf: return "addf";
	case byte_code::id::subb: return "subb";
	case byte_code::id::subw: return "subw";
	case byte_code::id::subd: return "subd";
	case byte_code::id::subq: return "subq";
	case byte_code::id::subf: return "subf";
	case byte_code::id::multb: return "multb";
	case byte_code::id::multw: return "multw";
	case byte_code::id::multd: return "multd";
	case byte_code::id::multq: return "multq";
	case byte_code::id::multf: return "multf";
	case byte_code::id::divb: return "divb";
	case byte_code::id::divw: return "divw";
	case byte_code::id::divd: return "divd";
	case byte_code::id::divq: return "divq";
	case byte_code::id::divf: return "divf";
	case byte_code::id::modb: return "modb";
	case byte_code::id::modw: return "modw";
	case byte_code::id::modd: return "modd";
	case byte_code::id::modq: return "modq";
	case byte_code::id::andb: return "andb";
	case byte_code::id::andw: return "andw";
	case byte_code::id::andd: return "andd";
	case byte_code::id::andq: return "andq";
	case byte_code::id::xorb: return "xorb";
	case byte_code::id::xorw: return "xorw";
	case byte_code::id::xord: return "xord";
	case byte_code::id::xorq: return "xorq";
	case byte_code::id::orb: return "orb";
	case byte_code::id::orw: return "orw";
	case byte_code::id::ord: return "ord";
	case byte_code::id::orq: return "orq";
	case byte_code::id::salb: return "salb";
	case byte_code::id::salw: return "salw";
	case byte_code::id::sald: return "sald";
	case byte_code::id::salq: return "salq";
	case byte_code::id::sarb: return "sarb";
	case byte_code::id::sarw: return "sarw";
	case byte_code::id::sard: return "sard";
	case byte_code::id::sarq: return "sarq";
	case byte_code::id::testb: return "testb";
	case byte_code::id::testw: return "testw";
	case byte_code::id::testd: return "testd";
	case byte_code::id::testq: return "testq";
	case byte_code::id::notb: return "notb";
	case byte_code::id::notw: return "notw";
	case byte_code::id::notd: return "notd";
	case byte_code::id::notq: return "notq";
	case byte_code::id::cmpb: return "cmpb";
	case byte_code::id::cmpw: return "cmpw";
	case byte_code::id::cmpd: return "cmpd";
	case byte_code::id::cmpq: return "cmpq";
	case byte_code::id::cmpf: return "cmpf";
	case byte_code::id::db: return "db";
	case byte_code::id::dw: return "dw";
	case byte_code::id::dd: return "dd";
	case byte_code::id::dq: return "dq";
	case byte_code::id::df: return "df";
	default:
		break;
	}

	return "<bad_op_code>";
}

thread_local elang::byte_code::debug *elang::byte_code::debug::debugger = nullptr;
