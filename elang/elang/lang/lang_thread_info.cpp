#include "lang_thread_info.h"

thread_local elang::lang::register_stack elang::lang::thread_info::reg_stack;

thread_local elang::lang::label_store elang::lang::thread_info::lbl_store;

thread_local elang::easm::instruction_table elang::lang::thread_info::ins_table;

thread_local unsigned int elang::lang::thread_info::decltype_count = 0;

thread_local elang::lang::symbol_table elang::lang::thread_info::sym_table("", nullptr);
