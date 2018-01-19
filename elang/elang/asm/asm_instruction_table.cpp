#include "asm_instruction_table.h"

void elang::easm::instruction_table::add(section_id section){
	active_section_ = &section_map_[section];
}

void elang::easm::instruction_table::add(const std::string &label){
	if (active_section_ == nullptr)
		throw common::error::asm_no_active_section;

	if (label_map_.find(label) == label_map_.end()){
		label_map_[label] = label_info{ active_section_->offset };
		active_section_->labels.push_back(&label_map_[label]);
	}
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

void elang::easm::instruction_table::set_start_label(const std::string &label){
	start_label_ = label;
}

void elang::easm::instruction_table::set_stack_size(std::size_t size){
	stack_size_ = size;
}

void elang::easm::instruction_table::encode(char *buffer, std::size_t size, std::size_t offset){
	static std::vector<section_id> order({
		section_id::rodata,
		section_id::data,
		section_id::text
	});

	common::binary_output_writer writer(buffer, size, offset);
	for (auto id : order){
		auto section = section_map_.find(id);
		if (section == section_map_.end())
			continue;//Section not present

		for (auto label : section->second.labels){//Update labels
			label->offset += offset;
			label->offset_ptr = &label->offset;
		}

		for (auto instruction : section->second.instructions)
			instruction->encode(writer, offset);
	}
}

std::size_t elang::easm::instruction_table::size() const{
	std::size_t value = 0;
	for (auto &section : section_map_)
		value += section.second.offset;
	return value;
}

std::size_t elang::easm::instruction_table::stack_size() const{
	return stack_size_;
}

unsigned __int64 elang::easm::instruction_table::start_address() const{
	if (start_label_.empty())
		return 0u;

	auto entry = label_map_.find(start_label_);
	return ((entry == label_map_.end() || (entry->second.offset_ptr == nullptr)) ? 0u : entry->second.offset);
}

elang::memory::memory_register *elang::easm::instruction_table::find_register(const std::string &name) const{
	return reg_tbl_.find(name);
}

unsigned __int64 *&elang::easm::instruction_table::find_label(const std::string &name){
	return label_map_[name].offset_ptr;
}
