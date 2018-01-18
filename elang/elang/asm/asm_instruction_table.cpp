#include "asm_instruction_table.h"

void elang::easm::instruction_table::add(section_id section){
	active_section_ = &section_map_[section];
}

void elang::easm::instruction_table::add(const std::string &label){
	if (active_section_ == nullptr)
		throw common::error::asm_no_active_section;

	if (label_map_.find(label) == label_map_.end())
		label_map_[label] = label_info{ active_section_, active_section_->offset };
	else
		throw common::error::asm_label_redefinition;
}

void elang::easm::instruction_table::add(iptr_type instruction){
	if (active_section_ == nullptr)
		throw common::error::asm_no_active_section;

	instruction->validate();
	active_section_->offset += instruction->encoded_size();
	active_section_->instructions.push_back(instruction);
}

std::size_t elang::easm::instruction_table::size() const{
	return 0u;
}
